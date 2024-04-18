// Copyright 2012-2024 Interdimensional Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "RyEditorTextureHelpers.generated.h"

/**
 * 
 */
UCLASS()
class RYEDITOR_API URyEditorTextureHelpers : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	/** Set the maximum resolution for generated textures. A value of 0 means the maximum size for the format on each platform. */
	UFUNCTION(BlueprintCallable, Category = "RyRuntime|TextureHelpers")
	static void SetTextureMaximumTextureSize(class UTexture2D* texture, const int32 newMaxSize);
};
