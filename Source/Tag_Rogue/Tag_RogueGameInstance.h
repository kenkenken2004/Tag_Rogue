// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Map/RogueAlpha_MapGenerator.h"
#include "Map/TerrainMaker.h"
#include "Tag_RogueGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TAG_ROGUE_API UTag_RogueGameInstance final : public UGameInstance
{
	GENERATED_BODY()
public:
	//Member Variable
	TMap<FName,FAssetData> AssetDatas;
	UTag_RogueGameInstance();
	UTag_RogueGameInstance* GetInstance() const;
	static void DisplayDebugMessage(FString Message);
	UPROPERTY(VisibleAnywhere)
	float FloatRemainingTime = 0;
	UPROPERTY(VisibleAnywhere)
	int32 IntRemainingTime = 0;
	bool bIsAssetDataLoaded = false;
	bool bShouldSChangeNumbers = false;

	UPROPERTY()
	URogueAlpha_MapGenerator* MapGenerator;
	UPROPERTY()
	UTerrainMaker* TerrainMaker;
	
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
	UPROPERTY(Config)
	float CellSize = 300;

	
	TArray<FName> AssetsPathArray = TArray<FName>
	(
		{
			TEXT("/Game/MapObject/Earth/"),
			TEXT("/Game/MapObject/Gate/"),
			TEXT("/Game/MapObject/MapUnit/"),
			TEXT("/Game/MapObject/Material/"),
			TEXT("/Game/Interface/Display/"),
			TEXT("/Game/Interface/Font/Tr2n/"),
			TEXT("/Game/Interface/Count/")
		}
	);
	
	void LoadAssets();
	template <typename T> T* GetAssetObject(const FName AssetName)
	{
		UObject* Instant = AssetDatas[AssetName].GetAsset();
		return static_cast<T*>(Instant);
	}

	void InitializeMapBuilders();
	
};
