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
	//UObjectLibrary* ObjectLibrary;
	//TArray<FAssetData> AssetDatas;
	
	
	
	
	UTag_RogueGameInstance();
	static UTag_RogueGameInstance* GetInstance();
	static void DisplayDebugMessage(FString Message);

	void AddClass(FString);
	void AddObject(FString);
	
};
