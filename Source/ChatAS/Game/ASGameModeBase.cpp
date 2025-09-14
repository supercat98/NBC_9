#include "ASGameModeBase.h"
#include "ASGameStateBase.h"
#include "Player/ASPlayerController.h"
#include "Player/ASPlayerState.h"
#include "EngineUtils.h"

void AASGameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);

	AASPlayerController* CASPlayerController = Cast<AASPlayerController>(NewPlayer);
	if (IsValid(CASPlayerController) == true)
	{
		CASPlayerController->NotificationText = FText::FromString(TEXT("Connected to the game server."));

		AllPlayerControllers.Add(CASPlayerController);

		AASPlayerState* CASPS = CASPlayerController->GetPlayerState<AASPlayerState>();
		if (IsValid(CASPS) == true)
		{
			CASPS->PlayerNameString = TEXT("Player") + FString::FromInt(AllPlayerControllers.Num());
		}

		AASGameStateBase* CASGameStateBase = GetGameState<AASGameStateBase>();
		if (IsValid(CASGameStateBase) == true)
		{
			CASGameStateBase->MulticastRPCBroadcastLoginMessage(CASPS->PlayerNameString);
		}
	}
}

FString AASGameModeBase::GenerateSecretNumber()
{
	TArray<int32> Numbers;
	for (int32 i = 1; i <= 9; ++i)
	{
		Numbers.Add(i);
	}

	FMath::RandInit(FDateTime::Now().GetTicks());
	Numbers = Numbers.FilterByPredicate([](int32 Num) { return Num > 0; });

	FString Result;
	for (int32 i = 0; i < 3; ++i)
	{
		int32 Index = FMath::RandRange(0, Numbers.Num() - 1);
		Result.Append(FString::FromInt(Numbers[Index]));
		Numbers.RemoveAt(Index);
	}

	return Result;
}

bool AASGameModeBase::IsGuessNumberString(const FString& InNumberString)
{
	bool bCanPlay = false;

	do {

		if (InNumberString.Len() != 3)
		{
			break;
		}

		bool bIsUnique = true;
		TSet<TCHAR> UniqueDigits;
		for (TCHAR C : InNumberString)
		{
			if (FChar::IsDigit(C) == false || C == '0')
			{
				bIsUnique = false;
				break;
			}

			UniqueDigits.Add(C);
		}

		if (bIsUnique == false)
		{
			break;
		}

		bCanPlay = true;

	} while (false);

	return bCanPlay;
}

FString AASGameModeBase::JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString)
{
	int32 StrikeCount = 0, BallCount = 0;

	for (int32 i = 0; i < 3; ++i)
	{
		if (InSecretNumberString[i] == InGuessNumberString[i])
		{
			StrikeCount++;
		}
		else
		{
			FString PlayerGuessChar = FString::Printf(TEXT("%c"), InGuessNumberString[i]);
			if (InSecretNumberString.Contains(PlayerGuessChar))
			{
				BallCount++;
			}
		}
	}

	if (StrikeCount == 0 && BallCount == 0)
	{
		return TEXT("OUT");
	}

	return FString::Printf(TEXT("%dS%dB"), StrikeCount, BallCount);
}

void AASGameModeBase::BeginPlay()
{
	Super::BeginPlay();

	SecretNumberString = GenerateSecretNumber();
}

void AASGameModeBase::PrintChatMessageString(AASPlayerController* InChattingPlayerController, const FString& InChatMessageString)
{
	FString ChatMessageString = InChatMessageString;
	int Index = InChatMessageString.Len() - 3;
	FString GuessNumberString = InChatMessageString.RightChop(Index);
	if (IsGuessNumberString(GuessNumberString) == true)
	{
		FString JudgeResultString = JudgeResult(SecretNumberString, GuessNumberString);

		IncreaseGuessCount(InChattingPlayerController);

		for (TActorIterator<AASPlayerController> It(GetWorld()); It; ++It)
		{
			AASPlayerController* CASPlayerController = *It;
			if (IsValid(CASPlayerController) == true)
			{
				FString CombinedMessageString = InChatMessageString + TEXT(" -> ") + JudgeResultString;
				CASPlayerController->ClientRPCPrintChatMessageString(CombinedMessageString);

				int32 StrikeCount = FCString::Atoi(*JudgeResultString.Left(1));
				JudgeGame(InChattingPlayerController, StrikeCount);
			}
		}
	}
	else
	{
		for (TActorIterator<AASPlayerController> It(GetWorld()); It; ++It)
		{
			AASPlayerController* CASPlayerController = *It;
			if (IsValid(CASPlayerController) == true)
			{
				CASPlayerController->ClientRPCPrintChatMessageString(InChatMessageString);
			}
		}
	}
}

void AASGameModeBase::IncreaseGuessCount(AASPlayerController* InChattingPlayerController)
{
	AASPlayerState* CASPS = InChattingPlayerController->GetPlayerState<AASPlayerState>();
	if (IsValid(CASPS) == true)
	{
		CASPS->CurrentGuessCount++;
	}
}

void AASGameModeBase::ResetGame()
{
	SecretNumberString = GenerateSecretNumber();

	for (const auto& CASPlayerController : AllPlayerControllers)
	{
		AASPlayerState* CASPS = CASPlayerController->GetPlayerState<AASPlayerState>();
		if (IsValid(CASPS) == true)
		{
			CASPS->CurrentGuessCount = 0;
		}
	}
}

void AASGameModeBase::JudgeGame(AASPlayerController* InChattingPlayerController, int InStrikeCount)
{
	if (3 == InStrikeCount)
	{
		AASPlayerState* CASPS = InChattingPlayerController->GetPlayerState<AASPlayerState>();
		for (const auto& CASPlayerController : AllPlayerControllers)
		{
			if (IsValid(CASPS) == true)
			{
				FString CombinedMessageString = CASPS->PlayerNameString + TEXT(" has won the game.");
				CASPlayerController->NotificationText = FText::FromString(CombinedMessageString);

				ResetGame();
			}
		}
	}
	else
	{
		bool bIsDraw = true;
		for (const auto& CASPlayerController : AllPlayerControllers)
		{
			AASPlayerState* CASPS = CASPlayerController->GetPlayerState<AASPlayerState>();
			if (IsValid(CASPS) == true)
			{
				if (CASPS->CurrentGuessCount < CASPS->MaxGuessCount)
				{
					bIsDraw = false;
					break;
				}
			}
		}

		if (true == bIsDraw)
		{
			for (const auto& CASPlayerController : AllPlayerControllers)
			{
				CASPlayerController->NotificationText = FText::FromString(TEXT("Draw..."));

				ResetGame();
			}
		}
	}
}