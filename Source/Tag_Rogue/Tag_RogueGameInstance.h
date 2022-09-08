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

UENUM(BlueprintType)
enum struct ESettlement: uint8
{
	Yet UMETA(DisplayName="Yet"),
	Chaser UMETA(DisplayName="ChaserWon"),
	Fugitive UMETA(DisplayName="FugitiveWon")
};

UCLASS(Config=Game)
class TAG_ROGUE_API UTag_RogueGameInstance final : public UGameInstance
{
	GENERATED_BODY()
public:
	//Member Variable
	TMap<FName,FAssetData> AssetDatas;
	UTag_RogueGameInstance();
	~UTag_RogueGameInstance();
	static UTag_RogueGameInstance* GetInstance();
	static void DisplayDebugMessage(FString Message);
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float FloatRemainingTime = 0;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 IntRemainingTime = 0;
	bool bIsAssetDataLoaded = false;
	bool bShouldSChangeNumbers = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	ESettlement Settlement = ESettlement::Yet;
	

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
	UPROPERTY(Config)
	float MapScale = 0.25;
	UPROPERTY(Config)
	float RadarSensitivity = 5000;
	UPROPERTY(Config)
	float RadarDistExp = 0.5;
	UPROPERTY(Config)
	float PointerScale = 1;

	
	TArray<FName> AssetsPathArray = TArray<FName>
	(
		{
			TEXT("/Game/MapObject/StarsAndPlanet/"),
			TEXT("/Game/MapObject/StarsAndPlanet/Earth"),
			TEXT("/Game/MapObject/Gate/"),
			TEXT("/Game/MapObject/MapUnit/"),
			TEXT("/Game/MapObject/Material/"),
			TEXT("/Game/Interface/Map/"),
			TEXT("/Game/Interface/Font/Tr2n/"),
			TEXT("/Game/Interface/Count/"),
			TEXT("/Game/Interface/UI/")
		}
	);
	
	void LoadAssets();
	template <typename T> T* GetAssetObject(const FName AssetName)
	{
		UObject* Instant = AssetDatas[AssetName].GetAsset();
		return static_cast<T*>(Instant);
	}

	void InitializeMapBuilders();

	UFUNCTION(BlueprintCallable)
	void ChaserWon();
	UFUNCTION(BlueprintCallable)
	void FugitiveWon();
	UFUNCTION(BlueprintImplementableEvent)
	void OnChaserWon();
	UFUNCTION(BlueprintImplementableEvent)
	void OnFugitiveWon();
};
