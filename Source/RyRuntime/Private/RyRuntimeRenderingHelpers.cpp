// Copyright 2020-2021 Sheffer Online Services.
// MIT License. See LICENSE for details.

#include "RyRuntimeRenderingHelpers.h"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
ERyShaderPlatform URyRuntimeRenderingHelpers::GetShaderPlatform(UObject* WorldContextObject)
{
	if (!FApp::CanEverRender())
	{
		// Returning early to avoid warnings about missing resources that are expected when CanEverRender is false.
		return ERyShaderPlatform::SP_INVALID;
	}
	
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if(!World || !World->Scene)
	{
		return ERyShaderPlatform::SP_INVALID;
	}

	switch(World->Scene->GetShaderPlatform())
	{
	case SP_PCD3D_SM5:
		return ERyShaderPlatform::SP_PCD3D_SM5;
	case SP_METAL:
		return ERyShaderPlatform::SP_METAL;
	case SP_METAL_MRT:
		return ERyShaderPlatform::SP_METAL_MRT;
	case SP_PCD3D_ES3_1:
		return ERyShaderPlatform::SP_PCD3D_ES3_1;
	case SP_OPENGL_PCES3_1:
		return ERyShaderPlatform::SP_OPENGL_PCES3_1;
	case SP_METAL_SM5:
		return ERyShaderPlatform::SP_METAL_SM5;
	case SP_VULKAN_PCES3_1:
		return ERyShaderPlatform::SP_VULKAN_PCES3_1;
	case SP_METAL_SM5_NOTESS:
		return ERyShaderPlatform::SP_METAL_SM5_NOTESS;
	case SP_VULKAN_SM5:
		return ERyShaderPlatform::SP_VULKAN_SM5;
	case SP_VULKAN_ES3_1_ANDROID:
		return ERyShaderPlatform::SP_VULKAN_ES3_1_ANDROID;
	case SP_METAL_MACES3_1:
		return ERyShaderPlatform::SP_METAL_MACES3_1;
	case SP_OPENGL_ES3_1_ANDROID:
		return ERyShaderPlatform::SP_OPENGL_ES3_1_ANDROID;
	case SP_METAL_MRT_MAC:
		return ERyShaderPlatform::SP_METAL_MRT_MAC;
	case SP_VULKAN_SM5_LUMIN:
		return ERyShaderPlatform::SP_VULKAN_SM5_LUMIN;
	case SP_VULKAN_ES3_1_LUMIN:
		return ERyShaderPlatform::SP_VULKAN_ES3_1_LUMIN;
	case SP_METAL_TVOS:
		return ERyShaderPlatform::SP_METAL_TVOS;
	case SP_METAL_MRT_TVOS:
		return ERyShaderPlatform::SP_METAL_MRT_TVOS;
	case SP_VULKAN_SM5_ANDROID:
		return ERyShaderPlatform::SP_VULKAN_SM5_ANDROID;
	default: ;
	}

	return ERyShaderPlatform::SP_INVALID;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
ERyRenderingFeatureLevel URyRuntimeRenderingHelpers::GetRenderingFeatureLevel(UObject* WorldContextObject)
{
	if (!FApp::CanEverRender())
	{
		// Returning early to avoid warnings about missing resources that are expected when CanEverRender is false.
		return ERyRenderingFeatureLevel::INVALID;
	}
	
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if(!World || !World->Scene)
	{
		return ERyRenderingFeatureLevel::INVALID;
	}

	switch(World->Scene->GetFeatureLevel())
	{
	case ERHIFeatureLevel::ES3_1:
		return ERyRenderingFeatureLevel::ES3_1;
	case ERHIFeatureLevel::SM5:
		return ERyRenderingFeatureLevel::SM5;
	default: ;
	}

	return ERyRenderingFeatureLevel::INVALID;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
ERyShadingPath URyRuntimeRenderingHelpers::GetShadingPath(UObject* WorldContextObject)
{
	if (!FApp::CanEverRender())
	{
		// Returning early to avoid warnings about missing resources that are expected when CanEverRender is false.
		return ERyShadingPath::Invalid;
	}
	
	UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull);
	if(!World || !World->Scene)
	{
		return ERyShadingPath::Invalid;
	}

	switch(World->Scene->GetShadingPath())
	{
	case EShadingPath::Mobile:
		return ERyShadingPath::Mobile;
	case EShadingPath::Deferred:
		return ERyShadingPath::Deferred;
	default: ;
	}

	return ERyShadingPath::Invalid;
}
