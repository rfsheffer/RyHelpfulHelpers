// Copyright 2020 Sheffer Online Services.
// MIT License. See LICENSE for details.

#include "RyRuntimePlatformHelpers.h"
#include "GenericPlatform/GenericPlatformFile.h"
#include "HAL/PlatformFilemanager.h"

//---------------------------------------------------------------------------------------------------------------------
/**
*/
class RyBasePlatforFileFunctor : public IPlatformFile::FDirectoryVisitor
{
public:

    RyBasePlatforFileFunctor(ERyIterateDirectoryOut OutType) 
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

        bool shouldVisit = DoVisit(FilenameOrDirectory, bIsDirectory, wasFiltered);
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
class RyNativePlatformFileFunctor : public RyBasePlatforFileFunctor
{
public:

    virtual bool DoVisit(const FString& FileOrDirectoryPath, const bool IsDirectory, const bool WasFiltered) override
    {
        if(visitor)
        {
            return visitor(FileOrDirectoryPath, IsDirectory, WasFiltered, dirLevel);
        }
        return RyBasePlatforFileFunctor::DoVisit(FileOrDirectoryPath, IsDirectory, WasFiltered);
    }
    virtual bool DoFilter(const FString& PathString) override
    {
        if(filter)
        {
            return filter(PathString);
        }
        return RyBasePlatforFileFunctor::DoFilter(PathString);
    }

    RyNativePlatformFileFunctor(RyNativeVisitorSig visitorIn, RyNativeFileFilterSig filterIn, ERyIterateDirectoryOut OutType)
        : RyBasePlatforFileFunctor(OutType)
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
class RyBPPlatformFileFunctor : public RyBasePlatforFileFunctor
{
public:

    virtual bool DoVisit(const FString& FileOrDirectoryPath, const bool IsDirectory, const bool WasFiltered) override
    {
        if(visitor.IsBound())
        {
            return visitor.Execute(FileOrDirectoryPath, IsDirectory, WasFiltered, dirLevel);
        }
        return RyBasePlatforFileFunctor::DoVisit(FileOrDirectoryPath, IsDirectory, WasFiltered);
    }
    virtual bool DoFilter(const FString& PathString) override
    {
        if(filter.IsBound())
        {
            return filter.Execute(PathString);
        }
        return RyBasePlatforFileFunctor::DoFilter(PathString);
    }

    RyBPPlatformFileFunctor(FRyDirectoryVisitor visitorIn, FRyFileFilter filterIn, ERyIterateDirectoryOut OutType)
        : RyBasePlatforFileFunctor(OutType)
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
bool DoIterateDirectory(const FString& DirectoryName, const bool IncludeSubFolders, ERyIterateDirectoryOut OutType, TArray<FString>& PathsOut, RyBasePlatforFileFunctor& fileVisitor)
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
