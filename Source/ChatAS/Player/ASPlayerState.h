#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "ASPlayerState.generated.h"


UCLASS()
class CHATAS_API AASPlayerState : public APlayerState
{
	GENERATED_BODY()

public:
	AASPlayerState();

	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

	FString GetPlayerInfoString();

public:
	UPROPERTY(Replicated)
	FString PlayerNameString;

	UPROPERTY(Replicated)
	int32 CurrentGuessCount;

	UPROPERTY(Replicated)
	int32 MaxGuessCount;

};
