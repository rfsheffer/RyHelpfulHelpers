// Copyright 2020-2022 Solar Storm Interactive

#pragma once

#include "CoreMinimal.h"
#include "K2Node.h"
#include "K2Node_LoadAssetPriority.generated.h"

class FBlueprintActionDatabaseRegistrar;
class UEdGraph;

UCLASS(MinimalAPI)
class UK2Node_LoadAssetPriority : public UK2Node
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
	virtual void ValidateNodeDuringCompilation(class FCompilerResultsLog& MessageLog) const override;
	// End of UK2Node interface

protected:
	virtual FName NativeFunctionName() const;

	virtual const FName& GetInputCategory() const;
	virtual const FName& GetInputPriorityCategory() const;
	virtual const FName& GetOutputCategory() const;

	virtual const FName& GetInputPinName() const;
	virtual const FName& GetInputPriorityPinName() const;
	virtual const FName& GetOutputPinName() const;
};
