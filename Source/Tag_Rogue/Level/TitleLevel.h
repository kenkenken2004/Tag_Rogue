// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Tag_Rogue/Tag_RogueGameInstance.h"
#include "TitleLevel.generated.h"

/**
 * 
 */
UCLASS()
class TAG_ROGUE_API ATitleLevel : public ALevelScriptActor
{
	GENERATED_BODY()

	UPROPERTY()
	UTag_RogueGameInstance* GameInstance;
	
	virtual void BeginPlay() override;
};
