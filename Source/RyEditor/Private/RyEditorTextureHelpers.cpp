// Copyright 2012-2024 Interdimensional Games, Inc. All Rights Reserved.


#include "RyEditorTextureHelpers.h"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyEditorTextureHelpers::SetTextureMaximumTextureSize(UTexture2D* texture, const int32 newMaxSize)
{
	if(texture)
	{
		texture->MaxTextureSize = newMaxSize;
		texture->MarkPackageDirty();
		texture->PostEditChange();
	}
}
