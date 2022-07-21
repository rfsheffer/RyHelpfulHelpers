// Copyright 2020-2022 Solar Storm Interactive


using UnrealBuildTool;
using System.IO;

public class RyRuntime : ModuleRules
{
    public RyRuntime(ReadOnlyTargetRules Target) : base(Target)
    {
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
            }
        );
        
        if (Target.Type == TargetRules.TargetType.Editor)
        {
	        PrivateDependencyModuleNames.Add("UnrealEd");
        }
    }
}
