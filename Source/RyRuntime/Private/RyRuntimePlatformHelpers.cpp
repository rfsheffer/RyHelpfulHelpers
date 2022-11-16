// Copyright 2020-2022 Solar Storm Interactive

#include "RyRuntimePlatformHelpers.h"
#include "RyRuntimeModule.h"
#include "HAL/PlatformFileManager.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "GenericPlatform/GenericPlatformApplicationMisc.h"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
class RyBasePlatformFileFunctor : public IPlatformFile::FDirectoryVisitor
{
public:

    RyBasePlatformFileFunctor(ERyIterateDirectoryOut OutType) 
        : outOption(OutType)
        , dirLevel(0)
    {
    }

    virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override
    {
        bool wasFiltered = true;
        if(outOption == ERyIterateDirectoryOut::FilesAndDirectories ||
           (outOption == ERyIterateDirectoryOut::DirectoriesOnly && bIsDirectory) ||
           (outOption == ERyIterateDirectoryOut::FilesOnly && !bIsDirectory))
        {
            wasFiltered = DoFilter(FilenameOrDirectory);
            if(!wasFiltered)
            {
                paths.Add(FilenameOrDirectory);
            }
        }

        const bool shouldVisit = DoVisit(FilenameOrDirectory, bIsDirectory, wasFiltered);
        if(shouldVisit && bIsDirectory)
        {
            dirs.Add(FilenameOrDirectory);
        }

        return shouldVisit;
    }

    virtual bool DoVisit(const FString& FileOrDirectoryPath, const bool IsDirectory, const bool WasFiltered){ return true; }
    virtual bool DoFilter(const FString& PathString) { return false; }

    TArray<FString> dirs;
    TArray<FString> paths;
    ERyIterateDirectoryOut outOption;
    int32 dirLevel;
};

//---------------------------------------------------------------------------------------------------------------------
/**
*/
class RyNativePlatformFileFunctor : public RyBasePlatformFileFunctor
{
public:

    virtual bool DoVisit(const FString& FileOrDirectoryPath, const bool IsDirectory, const bool WasFiltered) override
    {
        if(visitor)
        {
            return visitor(FileOrDirectoryPath, IsDirectory, WasFiltered, dirLevel);
        }
        return RyBasePlatformFileFunctor::DoVisit(FileOrDirectoryPath, IsDirectory, WasFiltered);
    }
    virtual bool DoFilter(const FString& PathString) override
    {
        if(filter)
        {
            return filter(PathString);
        }
        return RyBasePlatformFileFunctor::DoFilter(PathString);
    }

    RyNativePlatformFileFunctor(RyNativeVisitorSig visitorIn, RyNativeFileFilterSig filterIn, ERyIterateDirectoryOut OutType)
        : RyBasePlatformFileFunctor(OutType)
        , visitor(visitorIn)
        , filter(filterIn)
    {
    }

private:
    RyNativeVisitorSig visitor;
    RyNativeFileFilterSig filter;
};

//---------------------------------------------------------------------------------------------------------------------
/**
*/
class RyBPPlatformFileFunctor : public RyBasePlatformFileFunctor
{
public:

    virtual bool DoVisit(const FString& FileOrDirectoryPath, const bool IsDirectory, const bool WasFiltered) override
    {
        if(visitor.IsBound())
        {
            return visitor.Execute(FileOrDirectoryPath, IsDirectory, WasFiltered, dirLevel);
        }
        return RyBasePlatformFileFunctor::DoVisit(FileOrDirectoryPath, IsDirectory, WasFiltered);
    }
    virtual bool DoFilter(const FString& PathString) override
    {
        if(filter.IsBound())
        {
            return filter.Execute(PathString);
        }
        return RyBasePlatformFileFunctor::DoFilter(PathString);
    }

    RyBPPlatformFileFunctor(FRyDirectoryVisitor visitorIn, FRyFileFilter filterIn, ERyIterateDirectoryOut OutType)
        : RyBasePlatformFileFunctor(OutType)
        , visitor(visitorIn)
        , filter(filterIn)
    {
    }

private:
    FRyDirectoryVisitor visitor;
    FRyFileFilter filter;
};

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool DoIterateDirectory(const FString& DirectoryName, const bool IncludeSubFolders, ERyIterateDirectoryOut OutType, TArray<FString>& PathsOut, RyBasePlatformFileFunctor& fileVisitor)
{
    int32 dirLevel = 0;
    TArray<FString> newDirs;
    TArray<FString> dirs;
    dirs.Add(DirectoryName);
    bool keepGoing = true;
    do 
    {
        fileVisitor.dirLevel = dirLevel;
        for(const FString& dirName : dirs)
        {
            keepGoing = FPlatformFileManager::Get().GetPlatformFile().IterateDirectory(*dirName, fileVisitor);
            PathsOut.Append(fileVisitor.paths);
            fileVisitor.paths.Reset();
            if(!keepGoing)
                break;

            newDirs.Append(fileVisitor.dirs);
            fileVisitor.dirs.Reset();
        }

        dirs = MoveTemp(newDirs);
        ++dirLevel;
    } while (keepGoing && IncludeSubFolders && dirs.Num());
    return keepGoing;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimePlatformHelpers::RyIterateDirectory(const FString& DirectoryName, const bool IterateSubFolders, ERyIterateDirectoryOut OutType, TArray<FString>& PathsOut, 
                                                   FRyDirectoryVisitor Visitor, FRyFileFilter Filter)
{
    RyBPPlatformFileFunctor bpVisitor(Visitor, Filter, OutType);
    return DoIterateDirectory(DirectoryName, IterateSubFolders, OutType, PathsOut, bpVisitor);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimePlatformHelpers::IterateDirectory(const FString& DirectoryName, const bool IterateSubFolders, ERyIterateDirectoryOut OutType, TArray<FString>& PathsOut, 
                                                 RyNativeVisitorSig Visitor, RyNativeFileFilterSig Filter)
{
    RyNativePlatformFileFunctor nativeVisitor(Visitor, Filter, OutType);
    return DoIterateDirectory(DirectoryName, IterateSubFolders, OutType, PathsOut, nativeVisitor);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FDateTime URyRuntimePlatformHelpers::GetFileTimeStamp(const FString& filePath, bool& isValid)
{
    IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
    isValid = FileExists(filePath);
    return isValid ? platformFile.GetTimeStamp(*filePath) : FDateTime(0);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
FText URyRuntimePlatformHelpers::GetFileTimeStampText(const FString& filePath, bool& isValid, const bool longName)
{
    IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
    isValid = FileExists(filePath);
    if(!isValid)
    {
        return FText::FromString(TEXT("INVALID"));
    }
    FDateTime FileTimeStamp = platformFile.GetTimeStamp(*filePath);
    const FTimespan UTCOffset = FDateTime::Now() - FDateTime::UtcNow();
    FileTimeStamp += UTCOffset;
    if(longName)
    {
        return FText::AsDateTime(FileTimeStamp, EDateTimeStyle::Long, EDateTimeStyle::Medium, FText::GetInvariantTimeZone());
    } 
    return FText::AsDateTime(FileTimeStamp, EDateTimeStyle::Short, EDateTimeStyle::Short,FText::GetInvariantTimeZone());
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimePlatformHelpers::DeleteFile(const FString& filePath)
{
    IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
    return platformFile.DeleteFile(*filePath);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimePlatformHelpers::DeleteDirectory(const FString& directoryPath, const bool recursive)
{
    IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
    if(recursive)
    {
        return platformFile.DeleteDirectoryRecursively(*directoryPath);
    }
    return platformFile.DeleteDirectory(*directoryPath);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimePlatformHelpers::CreateDirectoryTree(const FString& directoryTreePath)
{
    IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
    return platformFile.CreateDirectoryTree(*directoryTreePath);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimePlatformHelpers::CopyFile(const FString& sourcePath, const FString& destinationPath, const bool updateTimeStamp)
{
    IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
    const bool fileCopied = platformFile.CopyFile(*destinationPath,*sourcePath);
    if(fileCopied && updateTimeStamp)
    {
        platformFile.SetTimeStamp(*destinationPath, FDateTime::Now());
    }
    return fileCopied;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimePlatformHelpers::MoveFile(const FString& sourcePath, const FString& destinationPath, const bool updateTimeStamp)
{
    IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
    const bool fileMoved = platformFile.MoveFile(*destinationPath, *sourcePath);
    if(fileMoved && updateTimeStamp)
    {
        platformFile.SetTimeStamp(*destinationPath, FDateTime::Now());
    }
    return fileMoved;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimePlatformHelpers::FileExists(const FString& filePath)
{
    IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
    return platformFile.FileExists(*filePath);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimePlatformHelpers::FolderExists(const FString& directoryPath)
{
    IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
    return platformFile.DirectoryExists(*directoryPath);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimePlatformHelpers::PathInfo(const FString& fileSystemPath,
                         bool& exists,
                         bool& isFile,
                         bool& isDirectory,
                         bool& isReadOnly,
                         FDateTime& creationTime,
                         FDateTime& accessTime,
                         FDateTime& modificationTime,
                         int64& fileSize)
{
    IPlatformFile& platformFile = FPlatformFileManager::Get().GetPlatformFile();
    const FFileStatData statData = platformFile.GetStatData(*fileSystemPath);
    exists = statData.bIsValid;
    isFile = !statData.bIsDirectory;
    isDirectory = statData.bIsDirectory;
    isReadOnly = statData.bIsReadOnly;
    creationTime = statData.CreationTime;
    accessTime = statData.AccessTime;
    modificationTime = statData.ModificationTime;
    fileSize = statData.FileSize;
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimePlatformHelpers::ClipboardCopy(const FString& Str)
{
    FGenericPlatformApplicationMisc::ClipboardCopy(*Str);
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimePlatformHelpers::ClipboardPaste(FString& Dest)
{
    FGenericPlatformApplicationMisc::ClipboardPaste(Dest);
}

#if ENGINE_MAJOR_VERSION > 4 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 27)
static_assert(ERyDeviceScreenOrientation::LandscapeSensor == static_cast<ERyDeviceScreenOrientation>(EDeviceScreenOrientation::LandscapeSensor), "EDeviceScreenOrientation misalignment!");
#else
static_assert(ERyDeviceScreenOrientation::FaceDown == static_cast<ERyDeviceScreenOrientation>(EDeviceScreenOrientation::FaceDown), "EDeviceScreenOrientation misalignment!");
#endif

//---------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimePlatformHelpers::SetDeviceOrientation(ERyDeviceScreenOrientation NewDeviceOrientation)
{
#if ENGINE_MAJOR_VERSION > 4 || (ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION >= 27)
#if PLATFORM_ANDROID == 0
    UE_LOG(LogRyRuntime, Warning, TEXT("SetDeviceOrientation does nothing on this platform!"));
#endif
#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1) || ENGINE_MAJOR_VERSION > 5
    FPlatformMisc::SetAllowedDeviceOrientation(static_cast<EDeviceScreenOrientation>(NewDeviceOrientation));
#else
    FPlatformMisc::SetDeviceOrientation(static_cast<EDeviceScreenOrientation>(NewDeviceOrientation));
#endif
#else
    UE_LOG(LogRyRuntime, Warning, TEXT("SetDeviceOrientation does nothing on this engine version! Supports 4.27 or greater!"));
#endif
}

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimePlatformHelpers::PlatformRequiresCookedData()
{
    return FPlatformProperties::RequiresCookedData();
}
