// Copyright 2020-2023 Solar Storm Interactive

#include "RyEditorHelpers.h"
#include "Runtime/Launch/Resources/Version.h"
#include "Engine/Selection.h"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
URyEditorHelpers::URyEditorHelpers(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{

}

template<class T>
static bool IsEditorLevelActor(T* Actor)
{
	bool bResult = false;
	if (Actor && !Actor->IsPendingKill())
	{
		const UWorld* World = Actor->GetWorld();
		if (World && World->WorldType == EWorldType::Editor)
		{
			bResult = true;
		}
	}
	return bResult;
}

TArray<AActor*> URyEditorHelpers::GetSelectedLevelActorsExt(bool editorActors)
{
	TGuardValue<bool> UnattendedScriptGuard(GIsRunningUnattendedScript, true);

	TArray<AActor*> Result;

	for (FSelectionIterator Iter(*GEditor->GetSelectedActors()); Iter; ++Iter)
	{
		AActor* Actor = Cast<AActor>(*Iter);
		if(editorActors)
		{
			if (IsEditorLevelActor(Actor))
			{
				Result.Add(Actor);
			}
		}
		else
		{
			if (!IsEditorLevelActor(Actor))
			{
				Result.Add(Actor);
			}
		}
	}

	return Result;
}
