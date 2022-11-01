// Copyright 2020-2022 Solar Storm Interactive

#include "RyRuntimeDialogHelpers.h"

#if WITH_EDITOR
#include "AssetToolsModule.h"
#include "PropertyEditorModule.h"
#include "Editor/LevelEditor/Public/LevelEditor.h"
#include "Interfaces/IMainFrameModule.h"
#include "EditorDirectories.h"
#endif

#if PLATFORM_DESKTOP
#include "DesktopPlatformModule.h"
#endif

#include "Framework/Application/SlateApplication.h"

#include "Misc/MessageDialog.h"
#include "Runtime/Launch/Resources/Version.h"

//--------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeDialogHelpers::OpenFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& DefaultFile, 
                                            TArray<FString>& OutFilenames, int32& OutFilterIndex, ERyRunOpenFileDialogFlags Flag,
                                            const FString& FileTypes)
{
#if PLATFORM_DESKTOP
    IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
    if(DesktopPlatform)
    {
        const void* ParentWindowWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
        if(!ParentWindowWindowHandle)
        {
            return false;
        }
        return DesktopPlatform->OpenFileDialog(ParentWindowWindowHandle,
                                               DialogTitle,
                                               DefaultPath,
                                               DefaultFile,
                                               FileTypes,
                                               static_cast<uint32>(Flag),
                                               OutFilenames,
                                               OutFilterIndex);
    }
#endif
    return false;
}

//--------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeDialogHelpers::OpenDirectoryDialog(const FString& DialogTitle, const FString& DefaultPath, FString& OutFolderName)
{
#if PLATFORM_DESKTOP
    IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
    if(DesktopPlatform)
    {
        const void* ParentWindowWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
        if(!ParentWindowWindowHandle)
        {
            return false;
        }
        return DesktopPlatform->OpenDirectoryDialog(ParentWindowWindowHandle,
                                                    DialogTitle,
                                                    DefaultPath,
                                                    OutFolderName);
    }
#endif
    return false;
}

//--------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyRuntimeDialogHelpers::SaveFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& DefaultFile, 
                                            TArray<FString>& OutFilenames, ERyRunOpenFileDialogFlags Flag,
                                            const FString& FileTypes)
{
#if PLATFORM_DESKTOP
    IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
    if(DesktopPlatform)
    {
        const void* ParentWindowWindowHandle = FSlateApplication::Get().FindBestParentWindowHandleForDialogs(nullptr);
        if(!ParentWindowWindowHandle)
        {
            return false;
        }
        return DesktopPlatform->SaveFileDialog(ParentWindowWindowHandle,
                                               DialogTitle,
                                               DefaultPath,
                                               DefaultFile,
                                               FileTypes,
                                               static_cast<uint32>(Flag),
                                               OutFilenames);
    }
#endif
    return false;
}
#if WITH_EDITOR
static_assert(ERyRunLastDirectory::MAX == static_cast<ERyRunLastDirectory>(ELastDirectory::MAX), "Last Directory Enum changed. Update ERyRunLastDirectory to match ELastDirectory!");
#endif
//--------------------------------------------------------------------------------------------------------------------
/**
*/
FString URyRuntimeDialogHelpers::GetLastDirectory(ERyRunLastDirectory lastDirectory)
{
#if WITH_EDITOR
    const ELastDirectory::Type convertType = static_cast<ELastDirectory::Type>(lastDirectory);
    return FEditorDirectories::Get().GetLastDirectory(convertType);
#else
    return TEXT("");
#endif
}

static_assert(ERyRunAppReturnType::Continue == static_cast<ERyRunAppReturnType>(EAppReturnType::Continue), "EAppReturnType changed. Update ERyRunAppReturnType to match EAppReturnType!");
static_assert(ERyRunAppMsgType::YesNoYesAll == static_cast<ERyRunAppMsgType>(EAppMsgType::YesNoYesAll), "EAppMsgType changed. Update ERyRunAppMsgType to match EAppMsgType!");

//--------------------------------------------------------------------------------------------------------------------
/**
*/
ERyRunAppReturnType URyRuntimeDialogHelpers::OpenMessageDialog(ERyRunAppMsgType MessageType, const FText Message, const FText WindowTitle)
{
    const FText* optTitle = nullptr;
    if(!WindowTitle.IsEmpty())
    {
        optTitle = &WindowTitle;
    }

    return static_cast<ERyRunAppReturnType>(FMessageDialog::Open(static_cast<EAppMsgType::Type>(MessageType), EAppReturnType::Cancel,
                                                              Message, optTitle));
}

//--------------------------------------------------------------------------------------------------------------------
/**
*/
void URyRuntimeDialogHelpers::OpenDebugMessageDialog(const FText Message, const FText WindowTitle)
{
    const FText* optTitle = nullptr;
    if(!WindowTitle.IsEmpty())
    {
        optTitle = &WindowTitle;
    }

    FMessageDialog::Debugf(Message, optTitle);
}

#if PLATFORM_DESKTOP && WITH_EDITOR
//--------------------------------------------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_OneParam(FOnRyDialogButtonClicked, TWeakObjectPtr<UObject>);
DECLARE_DELEGATE(FOnRyDialogWindowClosed);

class RyObjectDetailsDialog
{ 
public:
    static TSharedPtr<RyObjectDetailsDialog> CreateDetailsDialogWindow(UClass* importFieldsClass, const FText windowTitle, const FText buttonText, const FVector2D& clientSize)
    {
        TSharedPtr<RyObjectDetailsDialog> detailsDialog = MakeShared<RyObjectDetailsDialog>();

        detailsDialog->ParamsObject = NewObject<UObject>(GetTransientPackage(), importFieldsClass);
        check(detailsDialog->ParamsObject.IsValid());
        detailsDialog->ParamsObject->AddToRoot();

        //FPropertyEditorModule& PropertyModule = FModuleManager::LoadModuleChecked<FPropertyEditorModule>("PropertyEditor");
        TArray< UObject* > ObjectsToView;
        ObjectsToView.Add(detailsDialog->ParamsObject.Get());

        SAssignNew(detailsDialog->DialogWindow, SWindow)
            .Title(windowTitle)
            .ClientSize(clientSize);

        FDetailsViewArgs Args;
        Args.bHideSelectionTip = true;
        Args.bLockable = false;

        FPropertyEditorModule& PropertyEditorModule = FModuleManager::GetModuleChecked<FPropertyEditorModule>("PropertyEditor");
        TSharedRef<IDetailsView> DetailView = PropertyEditorModule.CreateDetailView(Args);

        bool bHaveTemplate = false;
        for(int32 i = 0; i < ObjectsToView.Num(); i++)
        {
            if(ObjectsToView[i] != nullptr && ObjectsToView[i]->IsTemplate())
            {
                bHaveTemplate = true;
                break;
            }
        }

        DetailView->SetIsPropertyVisibleDelegate(FIsPropertyVisible::CreateLambda([](const FPropertyAndParent& PropertyAndParent, bool bHaveTemplate) -> bool
        {
#if ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION < 25
            const UProperty& Property = PropertyAndParent.Property;
#else
            const FProperty& Property = PropertyAndParent.Property;
#endif

            if(bHaveTemplate)
            {
#if ENGINE_MAJOR_VERSION == 4 && ENGINE_MINOR_VERSION < 25
                const UClass* PropertyOwnerClass = Cast<const UClass>(Property.GetOuter());
#else
                const UClass* PropertyOwnerClass = Property.GetOwnerClass();
#endif
                const bool bDisableEditOnTemplate = PropertyOwnerClass
                    && PropertyOwnerClass->IsNative()
                    && Property.HasAnyPropertyFlags(CPF_DisableEditOnTemplate);
                if(bDisableEditOnTemplate)
                {
                    return false;
                }
            }
            return true;
        }, bHaveTemplate));

        DetailView->SetObjects(ObjectsToView);

        detailsDialog->DialogWindow->SetContent(
            SNew(SVerticalBox)
            + SVerticalBox::Slot()
            .FillHeight(1.0f)
            [
                SNew(SBorder)
                .BorderImage(FEditorStyle::GetBrush(TEXT("PropertyWindow.WindowBorder")))
                [
                    DetailView
                ]
            ]
            + SVerticalBox::Slot()
            .AutoHeight()
            [
                SNew(SBorder)
                .BorderImage(FEditorStyle::GetBrush(TEXT("PropertyWindow.WindowBorder")))
                [
                    SNew(SButton)
                    .Text(buttonText)
                    .ForegroundColor(FLinearColor::Black)
                    .OnClicked(detailsDialog.ToSharedRef(), &RyObjectDetailsDialog::OnImportClicked)
                ]
            ]
        );

        detailsDialog->DialogWindow->SetOnWindowClosed(FOnWindowClosed::CreateSP(detailsDialog.ToSharedRef(), &RyObjectDetailsDialog::OnImportWindowClosed));
        return detailsDialog;
    }

    TSharedPtr<SWindow> DialogWindow;
    TWeakObjectPtr<UObject> ParamsObject;

    FOnRyDialogButtonClicked OnButtonClickedEvent;
    FOnRyDialogWindowClosed OnDialogClosedEvent;

    ~RyObjectDetailsDialog()
    {
        if(ParamsObject.IsValid())
        {
            ParamsObject->RemoveFromRoot();
        }
    }

    FReply OnImportClicked()
    {
        OnButtonClickedEvent.ExecuteIfBound(ParamsObject);
        DialogWindow->RequestDestroyWindow();
        return FReply::Handled();
    }
    void OnImportWindowClosed(const TSharedRef<SWindow>& window)
    {
        OnDialogClosedEvent.ExecuteIfBound();
    }

    void Show()
    {
        // If the main frame exists parent the window to it
        TSharedPtr< SWindow > ParentWindow;
#if WITH_EDITOR
        if(FModuleManager::Get().IsModuleLoaded("MainFrame"))
        {
            IMainFrameModule& MainFrame = FModuleManager::GetModuleChecked<IMainFrameModule>("MainFrame");
            ParentWindow = MainFrame.GetParentWindow();
        }
#endif

        FSlateApplication::Get().AddModalWindow(DialogWindow.ToSharedRef(), ParentWindow);

//         if(ParentWindow.IsValid())
//         {
//             // Parent the window to the main frame 
//             FSlateApplication::Get().AddWindowAsNativeChild(DialogWindow.ToSharedRef(), ParentWindow.ToSharedRef());
//         }
//         else
//         {
//             FSlateApplication::Get().AddWindow(DialogWindow.ToSharedRef());
//         }
    }
};
#endif

//--------------------------------------------------------------------------------------------------------------------
/**
*/
UObject* URyRuntimeDialogHelpers::OpenObjectDetailsDialog(TSubclassOf<UObject> DetailsClass, const FText WindowTitle, const FText ButtonText, bool& ButtonPressedOut, const FVector2D WindowSize)
{
#if PLATFORM_DESKTOP && WITH_EDITOR
    ButtonPressedOut = false;
    TWeakObjectPtr<UObject> ParamsObjectOut;
    TSharedPtr<RyObjectDetailsDialog> CurrentImportWindow = RyObjectDetailsDialog::CreateDetailsDialogWindow(DetailsClass, WindowTitle, ButtonText, WindowSize);
    CurrentImportWindow->OnButtonClickedEvent.BindLambda([&ParamsObjectOut, &ButtonPressedOut](TWeakObjectPtr<UObject> ParamsObject) {
        ParamsObjectOut = ParamsObject;
        ButtonPressedOut = true;
    });
//     CurrentImportWindow->OnDialogClosedEvent.BindLambda([]() {
//         
//     });
    CurrentImportWindow->Show();

    if(ParamsObjectOut.IsValid())
    {
        return ParamsObjectOut.Get();
    }
#endif
    return nullptr;
}
