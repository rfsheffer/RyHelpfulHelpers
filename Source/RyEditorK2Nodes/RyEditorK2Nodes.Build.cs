// Copyright 2020-2022 Sheffer Online Services.
// MIT License. See LICENSE for details.

using UnrealBuildTool;

public class RyEditorK2Nodes : ModuleRules
{
    public RyEditorK2Nodes(ReadOnlyTargetRules Target) : base(Target)
    {
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

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
                "BlueprintGraph",
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
                "AnimGraphRuntime",
				"AssetRegistry",
				"RenderCore",
				"UMGEditor",
				"RyRuntime",
				"DesktopPlatform",
				"MainFrame",
				"KismetCompiler",
			}
		);
		
#if UE_4_27_OR_LATER
	    PublicDependencyModuleNames.AddRange(
		    new string[]
		    {
			    "PhysicsCore",
		    }
	    );
#endif
    }
}
