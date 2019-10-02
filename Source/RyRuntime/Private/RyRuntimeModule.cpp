// Copyright 2019 Ryan Sheffer. All Rights Reserved.

#include "RyRuntimePrivatePCH.h"

#define LOCTEXT_NAMESPACE "RyRuntimeModule"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void FRyRuntimeModule::StartupModule()
{
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void FRyRuntimeModule::ShutdownModule()
{
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FRyRuntimeModule, RyRuntime)
DEFINE_LOG_CATEGORY(LogRyRuntime);
