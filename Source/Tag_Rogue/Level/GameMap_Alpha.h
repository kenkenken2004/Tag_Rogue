// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "Tag_Rogue/MapGenerator/RogueAlpha_MapGenerator.h"
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
	TMap<FString, TSubclassOf<AActor>> BP_Reference;
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	FVector Cie_Convert(int32,int32, float) const;
	static TArray<int32> Cie_Invert(FVector);
	TSubclassOf<AActor> GetClass(FString);
	void AddClass(FString,FString);
	void PlaceCubes();
public:
	AGameMap_Alpha();
};