// Copyright 2020-2022 Sheffer Online Services.
// MIT License. See LICENSE for details.

#include "RyRuntimeModule.h"

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
