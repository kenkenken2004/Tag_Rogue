// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Tag_RogueGameModeBase.generated.h"

/**
 * 
 */
UCLASS(Config=Game)
class TAG_ROGUE_API ATag_RogueGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool bIsHostChaser = false;
	ATag_RogueGameModeBase();
	
	UFUNCTION(BlueprintCallable)
	void TravelToGamePlayWorld();
	UFUNCTION(BlueprintCallable)
	void TravelToMenuWorld();
};
