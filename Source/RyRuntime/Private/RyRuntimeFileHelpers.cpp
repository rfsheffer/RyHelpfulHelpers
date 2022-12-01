// Copyright 2020-2022 Solar Storm Interactive

#include "RyRuntimeFileHelpers.h"
#include "RyRuntimeModule.h"
#include "HAL/PlatformFileManager.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "GenericPlatform/GenericPlatformApplicationMisc.h"
#include "Runtime/Launch/Resources/Version.h"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyFileHandle::BeginDestroy()
{
	UObject::BeginDestroy();
	if(Handle)
	{
		delete Handle;
		Handle = nullptr;
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyFileHandle::Seek(const int64 newPosition)
{
	if(!Handle)
	{
		return false;
	}

	return Handle->Seek(newPosition);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyFileHandle::SeekFromEnd(const int64 numBytes)
{
	if(!Handle)
	{
		return false;
	}

	return Handle->SeekFromEnd(numBytes);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyFileHandle::SeekToStart()
{
	if(!Handle)
	{
		return false;
	}

	return Handle->Seek(0);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyFileHandle::SeekToEnd()
{
	if(!Handle)
	{
		return false;
	}

	return Handle->SeekFromEnd(0);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyFileHandle::Read(TArray<uint8>& bytesTo, const int64 numBytes)
{
	if(!Handle || !CanRead)
	{
		return false;
	}

	const int64 numToRead = FMath::Min(Handle->Size() - Handle->Tell(), numBytes);
	if(numToRead <= 0)
	{
		return false;
	}
	
	bytesTo.SetNum(numToRead);
	return Handle->Read(bytesTo.GetData(), bytesTo.Num());
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyFileHandle::Write(const TArray<uint8>& bytesOut)
{
	if(!Handle || bytesOut.Num() == 0 || !CanWrite)
	{
		return false;
	}

	return Handle->Write(bytesOut.GetData(), bytesOut.Num());
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
int64 URyFileHandle::Size() const
{
	if(!Handle)
	{
		return 0;
	}

	return Handle->Size();
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyFileHandle::Close()
{
	if(Handle)
	{
		delete Handle;
		Handle = nullptr;
	}
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
URyFileHandle* URyRuntimeFileHelpers::OpenFileHandle(UObject* outer, const FString filePath, const bool forRead, const bool forWrite, bool& success)
{
	IFileHandle* fileHandle = nullptr;
	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
	if(forRead)
	{
		fileHandle = platformFile.OpenRead(*filePath, forWrite);
	}
	else if(forWrite)
	{
		fileHandle = platformFile.OpenWrite(*filePath, false, forRead);
	}
	
	URyFileHandle* handle = nullptr;
	if(fileHandle)
	{
		handle = NewObject<URyFileHandle>(outer);
		if(handle)
		{
			handle->Handle = fileHandle;
			handle->CanRead = forRead;
			handle->CanWrite = forWrite;
			success = true;
		}
	}

	return handle;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeFileHelpers::WriteBytesToFile(const FString filePath, const bool allowOverwrite, const TArray<uint8>& bytesOut)
{
	if(FPaths::FileExists(filePath) && !allowOverwrite)
	{
		return false;
	}

	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
	IFileHandle* fileHandle = platformFile.OpenWrite(*filePath);
	if(fileHandle)
	{
		const bool writeSuccess = fileHandle->Write(bytesOut.GetData(), bytesOut.Num());
		delete fileHandle;
		return writeSuccess;
	}
	
	return false;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeFileHelpers::ReadAllBytesFromFile(const FString filePath, TArray<uint8>& bytesIn)
{
	if(!FPaths::FileExists(filePath))
	{
		return false;
	}

	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
	IFileHandle* fileHandle = platformFile.OpenRead(*filePath);
	if(fileHandle)
	{
		bytesIn.SetNum(fileHandle->Size());
		const bool readSuccess = fileHandle->Read(bytesIn.GetData(), bytesIn.Num());
		delete fileHandle;
		return readSuccess;
	}
	
	return false;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeFileHelpers::ReadBytesFromFile(const FString filePath, TArray<uint8>& bytesIn, const int64 offset, const int64 numBytes)
{
	if(!FPaths::FileExists(filePath))
	{
		return false;
	}

	IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
	IFileHandle* fileHandle = platformFile.OpenRead(*filePath);
	if(fileHandle)
	{
		if(!fileHandle->Seek(offset))
		{
			delete fileHandle;
			return false;
		}
		
		const int64 numToRead = FMath::Min(fileHandle->Size() - fileHandle->Tell(), numBytes);
		if(numToRead <= 0)
		{
			delete fileHandle;
			return false;
		}

		bytesIn.SetNum(numToRead);
		const bool readSuccess = fileHandle->Read(bytesIn.GetData(), bytesIn.Num());
		delete fileHandle;
		return readSuccess;
	}
	
	return false;
}
