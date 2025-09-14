#include "ASPlayerController.h"
#include "UI/ASChatInput.h"
#include "Kismet/KismetSystemLibrary.h"
#include "ChatAS/ChatAS.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Game/ASGameModeBase.h"
#include "ASPlayerState.h"
#include "Net/UnrealNetwork.h"

AASPlayerController::AASPlayerController()
{
	bReplicates = true;
}

void AASPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocalController() == false)
	{
		return;
	}

	FInputModeUIOnly InputModeUIOnly;
	SetInputMode(InputModeUIOnly);

	if (IsValid(ChatInputWidgetClass) == true)
	{
		ChatInputWidgetInstance = CreateWidget<UASChatInput>(this, ChatInputWidgetClass);
		if (IsValid(ChatInputWidgetInstance) == true)
		{
			ChatInputWidgetInstance->AddToViewport();
		}
	}

	if (IsValid(NotificationTextWidgetClass) == true)
	{
		NotificationTextWidgetInstance = CreateWidget<UUserWidget>(this, NotificationTextWidgetClass);
		if (IsValid(NotificationTextWidgetInstance) == true)
		{
			NotificationTextWidgetInstance->AddToViewport();
		}
	}
}

void AASPlayerController::SetChatMessageString(const FString& InChatMessageString)
{
	ChatMessageString = InChatMessageString;

	if (IsLocalController() == true)
	{
		AASPlayerState* CASPS = GetPlayerState<AASPlayerState>();
		if (IsValid(CASPS) == true)
		{
			FString CombinedMessageString = CASPS->GetPlayerInfoString() + TEXT(": ") + InChatMessageString;

			ServerRPCPrintChatMessageString(CombinedMessageString);
		}
	}
}

void AASPlayerController::PrintChatMessageString(const FString& InChatMessageString)
{
	ChatASFunctionLibrary::MyPrintString(this, InChatMessageString, 10.f);
}

void AASPlayerController::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, NotificationText);
}

void AASPlayerController::ClientRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	PrintChatMessageString(InChatMessageString);
}

void AASPlayerController::ServerRPCPrintChatMessageString_Implementation(const FString& InChatMessageString)
{
	AGameModeBase* GM = UGameplayStatics::GetGameMode(this);
	if (IsValid(GM) == true)
	{
		AASGameModeBase* CASGM = Cast<AASGameModeBase>(GM);
		if (IsValid(CASGM) == true)
		{
			CASGM->PrintChatMessageString(this, InChatMessageString);
		}
	}
}