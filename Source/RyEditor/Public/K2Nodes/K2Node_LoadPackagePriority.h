// Copyright 2020-2021 Sheffer Online Services.
// MIT License. See LICENSE for details.

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_LoadPackagePriority.generated.h"

class FBlueprintActionDatabaseRegistrar;
class UEdGraph;

UCLASS(MinimalAPI)
class UK2Node_LoadPackagePriority : public UK2Node
{
	GENERATED_BODY()
public:
	// UEdGraphNode interface
	virtual void AllocateDefaultPins() override;
	virtual FText GetTooltipText() const override;
	virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	virtual bool IsCompatibleWithGraph(const UEdGraph* TargetGraph) const override;
	// End of UEdGraphNode interface

	// UK2Node interface
	virtual bool IsNodePure() const override { return false; }
	virtual void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	virtual FName GetCornerIcon() const override;
	virtual void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	virtual FText GetMenuCategory() const override;
	virtual bool NodeCausesStructuralBlueprintChange() const override { return true; }
	virtual void ReallocatePinsDuringReconstruction(TArray<UEdGraphPin*>& OldPins) override;
	// End of UK2Node interface

	protected:
	virtual FName NativeFunctionName() const;

	virtual const FName& GetInputCategory() const;
	virtual const FName& GetInputPriorityCategory() const;
	virtual const FName& GetInputBlockOnLoadCategory() const;
	virtual const FName& GetOutputPackageCategory() const;
	virtual const FName& GetOutputResultCategory() const;

	virtual const FName& GetInputPinName() const;
	virtual const FName& GetInputPriorityPinName() const;
	virtual const FName& GetInputBlockOnLoadPinName() const;
	virtual const FName& GetOutputPackagePinName() const;
	virtual const FName& GetOutputResultPinName() const;
};
