// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Tag_Rogue/Tag_RogueGameInstance.h"
#include "Tag_Rogue/Character/CharacterBase.h"
#include "GameMap_Alpha.generated.h"

/**
 * 
 */
UCLASS(Config=Game)
class TAG_ROGUE_API AGameMap_Alpha final : public ALevelScriptActor
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	UTag_RogueGameInstance* GameInstance;
	
	//Functions
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void Initialize(UGameInstance* GameIns);

public:
	AGameMap_Alpha();
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnGameEnd();
};
