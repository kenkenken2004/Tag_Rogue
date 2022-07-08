// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Tag_Rogue/Tag_RogueGameInstance.h"
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


	TArray<FName> AssetsPathArray = TArray<FName>
	(
		{
			TEXT("/Game/MapObject/Mesh/"),
			TEXT("/Game/MapObject/Material/"),
			TEXT("/Game/Interface/Display/")
		}
	);
	
	//Functions
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void Initialize(int32,int32,int32);
	APawn* SpawnPlayer() const;
public:
	AGameMap_Alpha();
};
