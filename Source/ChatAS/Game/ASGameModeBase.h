#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ASGameModeBase.generated.h"

class AASPlayerController;

UCLASS()
class CHATAS_API AASGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	virtual void OnPostLogin(AController* NewPlayer) override;

	FString GenerateSecretNumber();

	bool IsGuessNumberString(const FString& InNumberString);

	FString JudgeResult(const FString& InSecretNumberString, const FString& InGuessNumberString);

	virtual void BeginPlay() override;

	void PrintChatMessageString(AASPlayerController* InChattingPlayerController, const FString& InChatMessageString);

	void IncreaseGuessCount(AASPlayerController* InChattingPlayerController);

	void ResetGame();

	void JudgeGame(AASPlayerController* InChattingPlayerController, int InStrikeCount);

protected:
	FString SecretNumberString;

	TArray<TObjectPtr<AASPlayerController>> AllPlayerControllers;
};
