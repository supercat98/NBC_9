#include "ASPawn.h"
#include "ChatAS.h"


void AASPawn::BeginPlay()
{
	Super::BeginPlay();

	FString NetRoleString = ChatASFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("CXPawn::BeginPlay() %s [%s]"), *ChatASFunctionLibrary::GetNetModeString(this), *NetRoleString);
	ChatASFunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}

void AASPawn::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	FString NetRoleString = ChatASFunctionLibrary::GetRoleString(this);
	FString CombinedString = FString::Printf(TEXT("CXPawn::PossessedBy() %s [%s]"), *ChatASFunctionLibrary::GetNetModeString(this), *NetRoleString);
	ChatASFunctionLibrary::MyPrintString(this, CombinedString, 10.f);
}

