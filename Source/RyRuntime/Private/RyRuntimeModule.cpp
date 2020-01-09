// Copyright 2020 Sheffer Online Services. All Rights Reserved.

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
