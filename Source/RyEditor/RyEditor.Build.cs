// Copyright 2020-2023 Solar Storm Interactive


using UnrealBuildTool;

public class RyEditor : ModuleRules
{
    public RyEditor(ReadOnlyTargetRules Target) : base(Target)
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
		
#if UE_4_26_OR_LATER
	    PublicDependencyModuleNames.AddRange(
		    new string[]
		    {
			    "PhysicsCore",
		    }
	    );
#endif
    }
}
