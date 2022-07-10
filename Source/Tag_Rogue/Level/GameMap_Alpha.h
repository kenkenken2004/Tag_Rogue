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
UCLASS()
class TAG_ROGUE_API AGameMap_Alpha : public ALevelScriptActor
{
	GENERATED_BODY()
protected:
	float CellSize;
	UPROPERTY()
	URogueAlpha_MapGenerator* Generator;
	UPROPERTY()
	UTerrainMaker* TerrainMaker;
	UPROPERTY()
	UTag_RogueGameInstance* GameInstance;
	UPROPERTY()
	ACharacterBase* Player0;
	UPROPERTY()
	int32 GameTimeLimit = 60;
	
	//Functions
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	UFUNCTION(BlueprintCallable)
	void Initialize(const int32 MapCellSize, const int32 MapHeight, const int32 MapWidth, UGameInstance* GameIns);
	APawn* SpawnPlayer() const;
public:
	AGameMap_Alpha();
	virtual void Tick(float DeltaSeconds) override;
};
