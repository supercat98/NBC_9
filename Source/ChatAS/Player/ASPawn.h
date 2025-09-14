#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "ASPawn.generated.h"

UCLASS()
class CHATAS_API AASPawn : public APawn
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void PossessedBy(AController* NewController) override;

};
