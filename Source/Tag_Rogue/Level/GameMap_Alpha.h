// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Tag_Rogue/Tag_RogueGameInstance.h"
#include "Tag_Rogue/Character/CharacterBase.h"
#include "Tag_Rogue/Map/RogueAlpha_MapGenerator.h"
#include "Tag_Rogue/Map/TerrainMaker.h"
#include "GameMap_Alpha.generated.h"

/**
 * 
 */
UCLASS(Config=Game)
class TAG_ROGUE_API AGameMap_Alpha final : public ALevelScriptActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(Config)
	float CellSize = 300;
	UPROPERTY()
	URogueAlpha_MapGenerator* Generator;
	UPROPERTY()
	UTerrainMaker* TerrainMaker;
	UPROPERTY()
	UTag_RogueGameInstance* GameInstance;
	UPROPERTY()
	ACharacterBase* Player0;
	UPROPERTY(Config)
	float GameTimeLimit = 60;
	UPROPERTY(Config)
	int32 GameMapHeight = 50;
	UPROPERTY(Config)
	int32 GameMapWidth = 50;
	UPROPERTY(Config)
	int32 PlazaSize = 9;
	UPROPERTY(Config)
	int32 RoomSize = 5;
	UPROPERTY(Config)
	int32 PlazaNum = 4;
	UPROPERTY(Config)
	int32 RoomNum = 9;
	//Functions
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UFUNCTION(BlueprintCallable)
	void Initialize(UGameInstance* GameIns);
	APawn* SpawnPlayer() const;
public:
	AGameMap_Alpha();
	virtual void Tick(float DeltaSeconds) override;
};
