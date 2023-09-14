// Copyright 2020-2023 Solar Storm Interactive

#include "RyEditorDialogHelpers.h"
#include "Runtime/Launch/Resources/Version.h"

#include "AssetToolsModule.h"
#include "PropertyEditorModule.h"

#include "Editor/LevelEditor/Public/LevelEditor.h"
#include "Interfaces/IMainFrameModule.h"

#include "DesktopPlatformModule.h"
#include "EditorDirectories.h"
#include "Framework/Application/SlateApplication.h"

#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1) || ENGINE_MAJOR_VERSION > 5
#include "Editor/EditorStyle/Public/EditorStyleSet.h"
#include "Runtime/SlateCore/Public/Styling/AppStyle.h"
#endif

#include "Misc/MessageDialog.h"

//--------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyEditorDialogHelpers::OpenFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& DefaultFile, 
                                            TArray<FString>& OutFilenames, int32& OutFilterIndex, ERyOpenFileDialogFlags Flag,
                                            const FString& FileTypes)
{
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

    return false;
}

//--------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyEditorDialogHelpers::OpenDirectoryDialog(const FString& DialogTitle, const FString& DefaultPath, FString& OutFolderName)
{
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

    return false;
}

//--------------------------------------------------------------------------------------------------------------------
/**
*/
bool URyEditorDialogHelpers::SaveFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& DefaultFile, 
                                            TArray<FString>& OutFilenames, ERyOpenFileDialogFlags Flag,
                                            const FString& FileTypes)
{
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

    return false;
}

static_assert(ERyLastDirectory::MAX == static_cast<ERyLastDirectory>(ELastDirectory::MAX), "Last Directory Enum changed. Update ERyLastDirectory to match ELastDirectory!");

//--------------------------------------------------------------------------------------------------------------------
/**
*/
FString URyEditorDialogHelpers::GetLastDirectory(ERyLastDirectory lastDirectory)
{
    const ELastDirectory::Type convertType = static_cast<ELastDirectory::Type>(lastDirectory);
    return FEditorDirectories::Get().GetLastDirectory(convertType);
}

static_assert(ERyAppReturnType::Continue == static_cast<ERyAppReturnType>(EAppReturnType::Continue), "EAppReturnType changed. Update ERyAppReturnType to match EAppReturnType!");
static_assert(ERyAppMsgType::YesNoYesAll == static_cast<ERyAppMsgType>(EAppMsgType::YesNoYesAll), "EAppMsgType changed. Update ERyAppMsgType to match EAppMsgType!");

//--------------------------------------------------------------------------------------------------------------------
/**
*/
ERyAppReturnType URyEditorDialogHelpers::OpenMessageDialog(ERyAppMsgType MessageType, const FText Message, const FText WindowTitle)
{
#if ENGINE_MAJOR_VERSION > 5 || (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3)
    return static_cast<ERyAppReturnType>(FMessageDialog::Open(static_cast<EAppMsgType::Type>(MessageType), EAppReturnType::Cancel,
                                                              Message, WindowTitle));
#else
    const FText* optTitle = nullptr;
    if(!WindowTitle.IsEmpty())
    {
        optTitle = &WindowTitle;
    }

    return static_cast<ERyAppReturnType>(FMessageDialog::Open(static_cast<EAppMsgType::Type>(MessageType), EAppReturnType::Cancel,
                                                              Message, optTitle));
#endif
}

//--------------------------------------------------------------------------------------------------------------------
/**
*/
void URyEditorDialogHelpers::OpenDebugMessageDialog(const FText Message, const FText WindowTitle)
{
#if ENGINE_MAJOR_VERSION > 5 || (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 3)
    FMessageDialog::Debugf(Message, WindowTitle);
#else
    const FText* optTitle = nullptr;
    if(!WindowTitle.IsEmpty())
    {
        optTitle = &WindowTitle;
    }

    FMessageDialog::Debugf(Message, optTitle);
#endif
}

//--------------------------------------------------------------------------------------------------------------------
/**
*/
DECLARE_DELEGATE_OneParam(FOnRyDialogButtonClicked, TWeakObjectPtr<UObject>);
DECLARE_DELEGATE(FOnRyDialogWindowClosed);

class RyObjectDetailsDialog
{ 
public:
    static TSharedPtr<RyObjectDetailsDialog> CreateWindow(UClass* importFieldsClass, const FText windowTitle, const FText buttonText, const FVector2D& clientSize)
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
#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1) || ENGINE_MAJOR_VERSION > 5
                .BorderImage(FAppStyle::GetBrush(TEXT("PropertyWindow.WindowBorder")))
#else
                .BorderImage(FEditorStyle::GetBrush(TEXT("PropertyWindow.WindowBorder")))
#endif
                [
                    DetailView
                ]
            ]
            + SVerticalBox::Slot()
            .AutoHeight()
            [
                SNew(SBorder)
#if (ENGINE_MAJOR_VERSION == 5 && ENGINE_MINOR_VERSION >= 1) || ENGINE_MAJOR_VERSION > 5
                .BorderImage(FAppStyle::GetBrush(TEXT("PropertyWindow.WindowBorder")))
#else
                .BorderImage(FEditorStyle::GetBrush(TEXT("PropertyWindow.WindowBorder")))
#endif
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
        if(FModuleManager::Get().IsModuleLoaded("MainFrame"))
        {
            IMainFrameModule& MainFrame = FModuleManager::GetModuleChecked<IMainFrameModule>("MainFrame");
            ParentWindow = MainFrame.GetParentWindow();
        }

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

//--------------------------------------------------------------------------------------------------------------------
/**
*/
UObject* URyEditorDialogHelpers::OpenObjectDetailsDialog(TSubclassOf<UObject> DetailsClass, const FText WindowTitle, const FText ButtonText, bool& ButtonPressedOut, const FVector2D WindowSize)
{
    ButtonPressedOut = false;
    TWeakObjectPtr<UObject> ParamsObjectOut;
    TSharedPtr<RyObjectDetailsDialog> CurrentImportWindow = RyObjectDetailsDialog::CreateWindow(DetailsClass, WindowTitle, ButtonText, WindowSize);
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
    return nullptr;
}
