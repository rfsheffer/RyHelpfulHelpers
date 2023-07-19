// Copyright 2020-2023 Solar Storm Interactive


using UnrealBuildTool;
using System.IO;

public class RyRuntime : ModuleRules
{
    public RyRuntime(ReadOnlyTargetRules Target) : base(Target)
    {
	    //OptimizeCode = CodeOptimization.Never;
	    PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateIncludePaths.Add("RyRuntime/ThirdParty/AHEasing");

		PrivateIncludePaths.AddRange(
            new string[]
            {
                "RyRuntime/Private",
            }
        );
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"HeadMountedDisplay",
				"SlateCore",
				"InputCore",
				"AudioCaptureCore",
			}
		);
        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Slate",
                "Engine",
				"NavigationSystem",
				"AIModule",
				"UMG",
				"ApplicationCore",
				"Voice",
				"EngineSettings",
				"RHI",
				"RenderCore",
				"PakFile",
            }
        );

        if (Target.bBuildEditor)
        {
	        PrivateDependencyModuleNames.AddRange(
		        new string[]
		        {
			        "UnrealEd",
		        }
		    );
        }
    }
}
