// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/ObjectLibrary.h"
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
	static UTag_RogueGameInstance* GetInstance();
	static void DisplayDebugMessage(FString Message);
	float FloatRemainingTime = 0;
	int32 IntRemainingTime = 0;
	bool bIsAssetDataLoaded = false;
	
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
	};
	
};
