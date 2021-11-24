// Copyright 2020-2021 Sheffer Online Services.
// MIT License. See LICENSE for details.

#include "RyRuntimePlatformHelpers.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "HAL/PlatformFilemanager.h"
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

//---------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimePlatformHelpers::PlatformRequiresCookedData()
{
    return FPlatformProperties::RequiresCookedData();
}
