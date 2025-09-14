#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ASGameStateBase.generated.h"

UCLASS()
class CHATAS_API AASGameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	
public:
	UFUNCTION(NetMulticast, Reliable)
	void MulticastRPCBroadcastLoginMessage(const FString& InNameString = FString(TEXT("XXXXXXX")));

};
