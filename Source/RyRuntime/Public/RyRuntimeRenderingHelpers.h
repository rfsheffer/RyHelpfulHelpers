// Copyright 2020-2022 Solar Storm Interactive

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Runtime/Launch/Resources/Version.h"
#include "RyRuntimeRenderingHelpers.generated.h"

UENUM(Blueprintable)
enum class ERyShaderPlatform : uint8
{
	SP_INVALID,
	SP_PCD3D_SM5,
	SP_METAL,
	SP_METAL_MRT,
	SP_PCD3D_ES3_1,
	SP_OPENGL_PCES3_1,
	SP_METAL_SM5,
	SP_VULKAN_PCES3_1,
#if ENGINE_MAJOR_VERSION < 5
	SP_METAL_SM5_NOTESS,
#endif
	SP_VULKAN_SM5,
	SP_VULKAN_ES3_1_ANDROID,
	SP_METAL_MACES3_1,
	SP_OPENGL_ES3_1_ANDROID,
	SP_METAL_MRT_MAC,
#if ENGINE_MAJOR_VERSION < 5
	SP_VULKAN_SM5_LUMIN,
	SP_VULKAN_ES3_1_LUMIN,
#endif
	SP_METAL_TVOS,
	SP_METAL_MRT_TVOS,
	SP_VULKAN_SM5_ANDROID,
#if ENGINE_MAJOR_VERSION >= 5
	SP_PCD3D_SM6,
	SP_D3D_ES3_1_HOLOLENS,
#endif
};

UENUM(Blueprintable)
enum class ERyRenderingFeatureLevel : uint8
{
	INVALID,

	/** Feature level defined by the core capabilities of OpenGL ES3.1 & Metal/Vulkan. */
	ES3_1,

	/**
	 * Feature level defined by the capabilities of DX11 Shader Model 5.
	 *   Compute shaders with shared memory, group sync, UAV writes, integer atomics
	 *   Indirect drawing
	 *   Pixel shaders with UAV writes
	 *   Cubemap arrays
	 *   Read-only depth or stencil views (eg read depth buffer as SRV while depth test and stencil write)
	 * Tessellation is not considered part of Feature Level SM5 and has a separate capability flag.
	 */
	SM5
};

UENUM(Blueprintable)
enum class ERyShadingPath : uint8
{
	Invalid,
	Mobile,
	Deferred,
};

UENUM()
enum class ERyScreenShotMode : uint8
{
	/// Game window
	Game,
	/// Editor Active Window (Editor Only)
	EditorActiveWindow,
	/// Editor level viewport (Editor Only)
	EditorLevelViewport,
};

//---------------------------------------------------------------------------------------------------------------------
/**
  * Static Helper functions for rendering.
*/
UCLASS()
class RYRUNTIME_API URyRuntimeRenderingHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/// Returns the current shader platform.
	/// Could be useful for cases where your game uses a couple of shading platforms and things look a bit different
	/// per platform and you can use this to switch out graphical scenarios to compensate.
	UFUNCTION(BlueprintPure, Category = "RyRuntime|RenderingHelpers", meta = (WorldContext = "WorldContextObject"))
	static ERyShaderPlatform GetShaderPlatform(UObject* WorldContextObject);

	/// Returns the current rendering feature level.
	UFUNCTION(BlueprintPure, Category = "RyRuntime|RenderingHelpers", meta = (WorldContext = "WorldContextObject"))
	static ERyRenderingFeatureLevel GetRenderingFeatureLevel(UObject* WorldContextObject);

	/// Returns the current shading path
	UFUNCTION(BlueprintPure, Category = "RyRuntime|RenderingHelpers", meta = (WorldContext = "WorldContextObject"))
	static ERyShadingPath GetShadingPath(UObject* WorldContextObject);
	
	/// Takes a screenshot and continues when finished (check outSuccess to know if a screenshot was taken)
	/// @param requestedPathOut A file path (ex: D:\something\myimage.png) which will try to be used as the screenshots destination.
	///						    If this path cannot be used an alternative will be chosen using alternative naming and put in pathOut.
	/// @param screenshotMode What type of screenshot to take. 'Game' is the only mode which works for non-editor builds.
	/// @param pathOut The path to the screenshot. Could be different than requestedPathOut if there was a pathing conflict.
	/// @param outSuccess True if a screenshot was produced.
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|RenderingHelpers", meta = (Latent = "", LatentInfo = "LatentInfo", WorldContext = "WorldContextObject"))
	static void TakeScreenshot(UObject* WorldContextObject, FLatentActionInfo LatentInfo, const FString& requestedPathOut, const ERyScreenShotMode screenshotMode, FString& pathOut, bool &outSuccess);
};
