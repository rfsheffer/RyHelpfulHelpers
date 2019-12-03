// Copyright 2020 Sheffer Online Services. All Rights Reserved.

using UnrealBuildTool;

public class RyEditor : ModuleRules
{
    public RyEditor(ReadOnlyTargetRules Target) : base(Target)
    {
		PrivatePCHHeaderFile = "RyEditorPrivatePCH.h";

		PrivateIncludePaths.AddRange(
			new string[] 
            {
				"RyEditor/Private",
            }
		);
        PublicDependencyModuleNames.AddRange(
			new string[]
			{
                "PropertyEditor",
                "EditorStyle",
                "LevelSequence",
            }
		);
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
                "Core",
                "CoreUObject",
                "Slate",
                "SlateCore",
                "Engine",
                "InputCore",
                "UnrealEd",
                "AnimGraph",
                "BlueprintGraph",
                "AnimGraphRuntime"
            }
		);
	}
}
