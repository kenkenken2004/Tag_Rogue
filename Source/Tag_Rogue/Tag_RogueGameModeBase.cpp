// Copyright Epic Games, Inc. All Rights Reserved.


#include "Tag_RogueGameModeBase.h"


ATag_RogueGameModeBase::ATag_RogueGameModeBase()
{
}

void ATag_RogueGameModeBase::TravelToGamePlayWorld()
{
	GetWorld()->ServerTravel(TEXT("GamePlayWorld"),false, false);
}

void ATag_RogueGameModeBase::TravelToMenuWorld()
{
	GetWorld()->ServerTravel(TEXT("MenuWorld"),false, false);
}
