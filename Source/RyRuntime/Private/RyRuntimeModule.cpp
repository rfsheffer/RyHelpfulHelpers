// Copyright 2020-2022 Solar Storm Interactive


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
