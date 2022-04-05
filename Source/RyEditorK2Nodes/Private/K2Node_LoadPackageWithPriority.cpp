// Copyright 2020-2022 Sheffer Online Services.
// MIT License. See LICENSE for details.

#include "K2Node_LoadPackageWithPriority.h"
#include "UObject/UnrealType.h"
#include "EdGraph/EdGraphPin.h"
#include "RyRuntimeObjectHelpers.h"
#include "EdGraphSchema_K2.h"
#include "K2Node_CallFunction.h"
#include "K2Node_AssignmentStatement.h"
#include "K2Node_CustomEvent.h"
#include "K2Node_TemporaryVariable.h"
#include "K2Node_ExecutionSequence.h"
#include "KismetCompiler.h"
#include "BlueprintNodeSpawner.h"
#include "BlueprintActionDatabaseRegistrar.h"

#define LOCTEXT_NAMESPACE "K2Node_LoadPackageWithPriority"

void UK2Node_LoadPackageWithPriority::AllocateDefaultPins()
{
	CreatePin(EGPD_Input, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Execute);

	// The immediate continue pin
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Then);

	// The delayed completed pin, this used to be called Then
	CreatePin(EGPD_Output, UEdGraphSchema_K2::PC_Exec, UEdGraphSchema_K2::PN_Completed);

	CreatePin(EGPD_Input, GetInputCategory(), GetInputPinName());
	CreatePin(EGPD_Input, GetInputPriorityCategory(), GetInputPriorityPinName());
	CreatePin(EGPD_Input, GetInputBlockOnLoadCategory(), GetInputBlockOnLoadPinName());
	CreatePin(EGPD_Output, GetOutputPackageCategory(), UPackage::StaticClass(), GetOutputPackagePinName());
	GetRyAsyncLoadingResultEnum()->ConditionalPostLoad();
	CreatePin(EGPD_Output, GetOutputResultCategory(), GetRyAsyncLoadingResultEnum(), GetOutputResultPinName());
}

void UK2Node_LoadPackageWithPriority::ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins)
{
	Super::ReallocatePinsDuringReconstruction(OldPins);

	UEdGraphPin* OldThenPin = nullptr;
	UEdGraphPin* OldCompletedPin = nullptr;

	for (UEdGraphPin* CurrentPin : OldPins)
	{
		if (CurrentPin->PinName == UEdGraphSchema_K2::PN_Then)
		{
			OldThenPin = CurrentPin;
		}
		else if (CurrentPin->PinName == UEdGraphSchema_K2::PN_Completed)
		{
			OldCompletedPin = CurrentPin;
		}
	}

	if (OldThenPin && !OldCompletedPin)
	{
		// This is an old node from when Completed was called then, rename the node to Completed and allow normal rewire to take place
		OldThenPin->PinName = UEdGraphSchema_K2::PN_Completed;
	}
}

void UK2Node_LoadPackageWithPriority::PreloadRequiredAssets()
{
	PreloadObject(GetRyAsyncLoadingResultEnum());
	Super::PreloadRequiredAssets();
}

void UK2Node_LoadPackageWithPriority::ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph)
{
	Super::ExpandNode(CompilerContext, SourceGraph);
	const UEdGraphSchema_K2* Schema = CompilerContext.GetSchema();
	check(Schema);
	bool bIsErrorFree = true;

	// Sequence node, defaults to two output pins
	UK2Node_ExecutionSequence* SequenceNode = CompilerContext.SpawnIntermediateNode<UK2Node_ExecutionSequence>(this, SourceGraph);
	SequenceNode->AllocateDefaultPins();

	// connect to input exe
	{
		UEdGraphPin* InputExePin = GetExecPin();
		UEdGraphPin* SequenceInputExePin = SequenceNode->GetExecPin();
		bIsErrorFree &= InputExePin && SequenceInputExePin && CompilerContext.MovePinLinksToIntermediate(*InputExePin, *SequenceInputExePin).CanSafeConnect();
	}

	// Create LoadPackagePriority function call
	UK2Node_CallFunction* CallLoadPackageNode = CompilerContext.SpawnIntermediateNode<UK2Node_CallFunction>(this, SourceGraph);
	CallLoadPackageNode->FunctionReference.SetExternalMember(NativeFunctionName(), URyRuntimeObjectHelpers::StaticClass());
	CallLoadPackageNode->AllocateDefaultPins(); 

	// connect load to first sequence pin
	{
		UEdGraphPin* CallFunctionInputExePin = CallLoadPackageNode->GetExecPin();
		UEdGraphPin* SequenceFirstExePin = SequenceNode->GetThenPinGivenIndex(0);
		bIsErrorFree &= SequenceFirstExePin && CallFunctionInputExePin && Schema->TryCreateConnection(CallFunctionInputExePin, SequenceFirstExePin);
	}

	// connect then to second sequence pin
	{
		UEdGraphPin* OutputThenPin = FindPin(UEdGraphSchema_K2::PN_Then);
		UEdGraphPin* SequenceSecondExePin = SequenceNode->GetThenPinGivenIndex(1);
		bIsErrorFree &= OutputThenPin && SequenceSecondExePin && CompilerContext.MovePinLinksToIntermediate(*OutputThenPin, *SequenceSecondExePin).CanSafeConnect();
	}

	// Create Local Variables
	UK2Node_TemporaryVariable* TempVarPackageOutput = CompilerContext.SpawnInternalVariable(this,
																					GetOutputPackageCategory(),
																					NAME_None, 
																					UPackage::StaticClass());
	UK2Node_TemporaryVariable* TempVarResultOutput = CompilerContext.SpawnInternalVariable(this,
																					GetOutputResultCategory(),
																					NAME_None,
																					GetRyAsyncLoadingResultEnum());

	// Create assign package node
	UK2Node_AssignmentStatement* AssignPackageNode = CompilerContext.SpawnIntermediateNode<UK2Node_AssignmentStatement>(this, SourceGraph);
	AssignPackageNode->AllocateDefaultPins();
	{
		UEdGraphPin* LoadedPackageVariablePin = TempVarPackageOutput->GetVariablePin();

		// connect local variable to assign node
		{
			UEdGraphPin* AssignLHSPPin = AssignPackageNode->GetVariablePin();
			bIsErrorFree &= AssignLHSPPin && LoadedPackageVariablePin && Schema->TryCreateConnection(AssignLHSPPin, LoadedPackageVariablePin);
		}

		// connect local variable to output
		{
			UEdGraphPin* OutputObjectPinPin = FindPin(GetOutputPackagePinName());
			bIsErrorFree &= LoadedPackageVariablePin && OutputObjectPinPin && CompilerContext.MovePinLinksToIntermediate(*OutputObjectPinPin, *LoadedPackageVariablePin).CanSafeConnect();
		}
	}

	// Create assign result node
	UK2Node_AssignmentStatement* AssignResultNode = CompilerContext.SpawnIntermediateNode<UK2Node_AssignmentStatement>(this, SourceGraph);
	AssignResultNode->AllocateDefaultPins();
	{
		UEdGraphPin* ResultVariablePin = TempVarResultOutput->GetVariablePin();

		// connect local variable to assign node
		{
			UEdGraphPin* AssignLHSPPin = AssignResultNode->GetVariablePin();
			bIsErrorFree &= AssignLHSPPin && ResultVariablePin && Schema->TryCreateConnection(AssignLHSPPin, ResultVariablePin);
		}

		// connect local variable to output
		{
			UEdGraphPin* OutputObjectPinPin = FindPin(GetOutputResultPinName());
			bIsErrorFree &= ResultVariablePin && OutputObjectPinPin && CompilerContext.MovePinLinksToIntermediate(*OutputObjectPinPin, *ResultVariablePin).CanSafeConnect();
		}
	}
	
	// connect to package name
	UEdGraphPin* CallFunctionPackagePathPin = CallLoadPackageNode->FindPin(GetInputPinName());
	{
		UEdGraphPin* PackagePathPin = FindPin(GetInputPinName());
		ensure(CallFunctionPackagePathPin);

		if (PackagePathPin && CallFunctionPackagePathPin)
		{
			if (PackagePathPin->LinkedTo.Num() > 0)
			{
				bIsErrorFree &= CompilerContext.MovePinLinksToIntermediate(*PackagePathPin, *CallFunctionPackagePathPin).CanSafeConnect();
			}
			else
			{
				// Copy literal value
				CallFunctionPackagePathPin->DefaultValue = PackagePathPin->DefaultValue;
			}
		}
		else
		{
			bIsErrorFree = false;
		}
	}

	// connect to priority
	UEdGraphPin* CallFunctionPriorityPin = CallLoadPackageNode->FindPin(GetInputPriorityPinName());
	{
		UEdGraphPin* PriorityPin = FindPin(GetInputPriorityPinName());
		ensure(CallFunctionPriorityPin);

		if (PriorityPin && CallFunctionPriorityPin)
		{
			if (PriorityPin->LinkedTo.Num() > 0)
			{
				bIsErrorFree &= CompilerContext.MovePinLinksToIntermediate(*PriorityPin, *CallFunctionPriorityPin).CanSafeConnect();
			}
			else
			{
				// Copy literal value
				CallFunctionPriorityPin->DefaultValue = PriorityPin->DefaultValue;
			}
		}
		else
		{
			bIsErrorFree = false;
		}
	}

	// connect to blockOnLoad
	UEdGraphPin* CallFunctionBlockOnLoadPin = CallLoadPackageNode->FindPin(GetInputBlockOnLoadPinName());
	{
		UEdGraphPin* BlockOnLoadPin = FindPin(GetInputBlockOnLoadPinName());
		ensure(CallFunctionBlockOnLoadPin);

		if (BlockOnLoadPin && CallFunctionBlockOnLoadPin)
		{
			if (BlockOnLoadPin->LinkedTo.Num() > 0)
			{
				bIsErrorFree &= CompilerContext.MovePinLinksToIntermediate(*BlockOnLoadPin, *CallFunctionBlockOnLoadPin).CanSafeConnect();
			}
			else
			{
				// Copy literal value
				CallFunctionBlockOnLoadPin->DefaultValue = BlockOnLoadPin->DefaultValue;
			}
		}
		else
		{
			bIsErrorFree = false;
		}
	}

	// Create OnLoadEvent
	const FName DelegateOnLoadedParamName(TEXT("OnLoaded"));
	UK2Node_CustomEvent* OnLoadEventNode = CompilerContext.SpawnIntermediateEventNode<UK2Node_CustomEvent>(this, CallFunctionPackagePathPin, SourceGraph);
	OnLoadEventNode->CustomFunctionName = *FString::Printf(TEXT("OnLoaded_%s"), *CompilerContext.GetGuid(this));
	OnLoadEventNode->AllocateDefaultPins();
	{
		UFunction* LoadPackageFunction = CallLoadPackageNode->GetTargetFunction();
		FDelegateProperty* OnLoadDelegateProperty = LoadPackageFunction ? FindFProperty<FDelegateProperty>(LoadPackageFunction, DelegateOnLoadedParamName) : nullptr;
		UFunction* OnLoadedSignature = OnLoadDelegateProperty ? OnLoadDelegateProperty->SignatureFunction : nullptr;
		ensure(OnLoadedSignature);
		for (TFieldIterator<FProperty> PropIt(OnLoadedSignature); PropIt && (PropIt->PropertyFlags & CPF_Parm); ++PropIt)
		{
			const FProperty* Param = *PropIt;
			if (!Param->HasAnyPropertyFlags(CPF_OutParm) || Param->HasAnyPropertyFlags(CPF_ReferenceParm))
			{
				FEdGraphPinType PinType;
				bIsErrorFree &= Schema->ConvertPropertyToPinType(Param, /*out*/ PinType);
				bIsErrorFree &= (nullptr != OnLoadEventNode->CreateUserDefinedPin(Param->GetFName(), PinType, EGPD_Output));
			}
		}
	}

	// connect delegate to OnLoaded parameter
	{
		UEdGraphPin* CallFunctionDelegatePin = CallLoadPackageNode->FindPin(DelegateOnLoadedParamName);
		ensure(CallFunctionDelegatePin);
		UEdGraphPin* EventDelegatePin = OnLoadEventNode->FindPin(UK2Node_CustomEvent::DelegateOutputName);
		bIsErrorFree &= CallFunctionDelegatePin && EventDelegatePin && Schema->TryCreateConnection(CallFunctionDelegatePin, EventDelegatePin);
	}

	// Connect delegate results to assignment pins
	{
		// connect loaded package from event to assign
		{
			UEdGraphPin* LoadedPackageEventPin = OnLoadEventNode->FindPin(GetOutputPackagePinName());
			ensure(LoadedPackageEventPin);
			UEdGraphPin* AssignRHSPPin = AssignPackageNode->GetValuePin();
			bIsErrorFree &= AssignRHSPPin && LoadedPackageEventPin && Schema->TryCreateConnection(LoadedPackageEventPin, AssignRHSPPin);
		}
	
		// connect loaded result from event to assign
		{
			UEdGraphPin* ResultEventPin = OnLoadEventNode->FindPin(GetOutputResultPinName());
			ensure(ResultEventPin);
			UEdGraphPin* AssignRHSPPin = AssignResultNode->GetValuePin();
			bIsErrorFree &= AssignRHSPPin && ResultEventPin && Schema->TryCreateConnection(ResultEventPin, AssignRHSPPin);
		}
	}

	// connect assign exec input to event output
	{
		// Event to assign package
		UEdGraphPin* OnLoadEventThenPin = OnLoadEventNode->FindPin(UEdGraphSchema_K2::PN_Then);
		UEdGraphPin* AssignPackageExePin = AssignPackageNode->GetExecPin();
		bIsErrorFree &= AssignPackageExePin && OnLoadEventThenPin && Schema->TryCreateConnection(AssignPackageExePin, OnLoadEventThenPin);

		// assign package to assign result
		UEdGraphPin* AssignPackageThenPin = AssignPackageNode->GetThenPin();
		UEdGraphPin* AssignResultExePin = AssignResultNode->GetExecPin();
		bIsErrorFree &= AssignResultExePin && AssignPackageThenPin && Schema->TryCreateConnection(AssignResultExePin, AssignPackageThenPin);

		// assign result to completed
		UEdGraphPin* OutputCompletedPin = FindPin(UEdGraphSchema_K2::PN_Completed);
		UEdGraphPin* AssignOutputExePin = AssignResultNode->GetThenPin();
		bIsErrorFree &= OutputCompletedPin && AssignOutputExePin && CompilerContext.MovePinLinksToIntermediate(*OutputCompletedPin, *AssignOutputExePin).CanSafeConnect();
	}

	if (!bIsErrorFree)
	{
		CompilerContext.MessageLog.Error(*LOCTEXT("InternalConnectionError", "K2Node_LoadPackageWithPriority: Internal connection error. @@").ToString(), this);
	}

	BreakAllNodeLinks();
}

FText UK2Node_LoadPackageWithPriority::GetTooltipText() const
{
	return FText(LOCTEXT("UK2Node_LoadPackageWithPriorityGetTooltipText", "Asynchronously loads a package by path and returns the package object if the load succeeds."));
}

FText UK2Node_LoadPackageWithPriority::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return FText(LOCTEXT("UK2Node_LoadPackageWithPriorityGetNodeTitle", "Async Load Package With Priority"));
}

bool UK2Node_LoadPackageWithPriority::IsCompatibleWithGraph(const UEdGraph* TargetGraph) const
{
	bool bIsCompatible = false;
	// Can only place events in ubergraphs and macros (other code will help prevent macros with latents from ending up in functions), and basicasync task creates an event node:
	EGraphType GraphType = TargetGraph->GetSchema()->GetGraphType(TargetGraph);
	if (GraphType == EGraphType::GT_Ubergraph || GraphType == EGraphType::GT_Macro)
	{
		bIsCompatible = true;
	}
	return bIsCompatible && Super::IsCompatibleWithGraph(TargetGraph);
}

FName UK2Node_LoadPackageWithPriority::GetCornerIcon() const
{
	return TEXT("Graph.Latent.LatentIcon");
}

void UK2Node_LoadPackageWithPriority::GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const
{
	// actions get registered under specific object-keys; the idea is that 
	// actions might have to be updated (or deleted) if their object-key is  
	// mutated (or removed)... here we use the node's class (so if the node 
	// type disappears, then the action should go with it)
	UClass* ActionKey = GetClass();
	// to keep from needlessly instantiating a UBlueprintNodeSpawner, first   
	// check to make sure that the registrar is looking for actions of this type
	// (could be regenerating actions for a specific asset, and therefore the 
	// registrar would only accept actions corresponding to that asset)
	if (ActionRegistrar.IsOpenForRegistration(ActionKey))
	{
		UBlueprintNodeSpawner* NodeSpawner = UBlueprintNodeSpawner::Create(GetClass());
		check(NodeSpawner != nullptr);

		ActionRegistrar.AddBlueprintAction(ActionKey, NodeSpawner);
	}
}

FText UK2Node_LoadPackageWithPriority::GetMenuCategory() const
{
	return FText(LOCTEXT("UK2Node_LoadPackageWithPriorityGetMenuCategory", "Utilities"));
}

const FName& UK2Node_LoadPackageWithPriority::GetInputCategory() const
{
	return UEdGraphSchema_K2::PC_String;
}

const FName& UK2Node_LoadPackageWithPriority::GetInputPriorityCategory() const
{
	return UEdGraphSchema_K2::PC_Int;
}

const FName& UK2Node_LoadPackageWithPriority::GetInputBlockOnLoadCategory() const
{
	return UEdGraphSchema_K2::PC_Boolean;
}

const FName& UK2Node_LoadPackageWithPriority::GetOutputPackageCategory() const
{
	return UEdGraphSchema_K2::PC_Object;
}

const FName& UK2Node_LoadPackageWithPriority::GetOutputResultCategory() const
{
	return UEdGraphSchema_K2::PC_Byte;
}

const FName& UK2Node_LoadPackageWithPriority::GetInputPinName() const
{
	static const FName InputAssetPinName("PackagePath");
	return InputAssetPinName;
}

const FName& UK2Node_LoadPackageWithPriority::GetInputPriorityPinName() const
{
	static const FName InputAssetPriorityPinName("Priority");
	return InputAssetPriorityPinName;
}

const FName& UK2Node_LoadPackageWithPriority::GetInputBlockOnLoadPinName() const
{
	static const FName InputAssetBlockOnLoadPinName("BlockOnLoad");
	return InputAssetBlockOnLoadPinName;
}

const FName& UK2Node_LoadPackageWithPriority::GetOutputPackagePinName() const
{
	static const FName OutputObjectPinName("LoadedPackage");
	return OutputObjectPinName;
}

const FName& UK2Node_LoadPackageWithPriority::GetOutputResultPinName() const
{
	static const FName OutputResultPinName("Result");
	return OutputResultPinName;
}

UEnum* UK2Node_LoadPackageWithPriority::GetRyAsyncLoadingResultEnum()
{
	if(!RyAsyncLoadingResultEnum)
	{
		RyAsyncLoadingResultEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("ERyAsyncLoadingResult"), true);
		check(RyAsyncLoadingResultEnum);
		PreloadObject(RyAsyncLoadingResultEnum);
	}

	return RyAsyncLoadingResultEnum;
}

FName UK2Node_LoadPackageWithPriority::NativeFunctionName() const
{
	return GET_FUNCTION_NAME_CHECKED(URyRuntimeObjectHelpers, LoadPackagePriority);
}


#undef LOCTEXT_NAMESPACE
