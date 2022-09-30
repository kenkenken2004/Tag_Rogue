// Copyright Epic Games, Inc. All Rights Reserved.


#include "Tag_RogueGameModeBase.h"

#include "Tag_RogueGameInstance.h"


ATag_RogueGameModeBase::ATag_RogueGameModeBase()
{
}

void ATag_RogueGameModeBase::TravelToGamePlayWorld()
{
	
	UTag_RogueGameInstance::GetInstance()->Settlement = ESettlement::Yet;
	GetWorld()->ServerTravel(TEXT("GamePlayWorld"),false, false);
}

void ATag_RogueGameModeBase::TravelToMenuWorld()
{
	GetWorld()->ServerTravel(TEXT("MenuWorld"),false, false);
	//UTag_RogueGameInstance::GetInstance()->Settlement = ESettlement::Yet;
}

void ATag_RogueGameModeBase::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	SaveConfig();
}
