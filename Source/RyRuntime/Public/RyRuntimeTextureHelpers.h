// Copyright 2020-2022 Solar Storm Interactive

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/LatentActionManager.h"

#include "RyRuntimeTextureHelpers.generated.h"

/** Used in blueprint latent function execution */
UENUM(BlueprintType)
enum class ERyTextureResourceResult : uint8
{
	Success,
    Failure
};

/**
 * 
 */
UCLASS()
class RYRUNTIME_API URyRuntimeTextureHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	// Tries to update a textures resource and returns a Success result of the resource was updated and valid.
	// Try to pass texture objects in which are valid and could have a resource associated or this function will fail by timing out after 20 seconds.
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|TextureHelpers", meta = (WorldContext = "WorldContextObject", Latent = "", LatentInfo = "LatentInfo", ExpandEnumAsExecs = "Result"))
    static void UpdateTextureResource(UObject* WorldContextObject, class UTexture* Texture, FLatentActionInfo LatentInfo, ERyTextureResourceResult& Result);
};
