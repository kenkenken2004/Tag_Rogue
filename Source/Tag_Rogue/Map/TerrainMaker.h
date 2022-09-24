// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueAlpha_MapGenerator.h"
#include "Tag_Rogue/MapObject/MapUnitBase.h"
#include "UObject/Object.h"
#include "TerrainMaker.generated.h"

/**
 * 
 */
UCLASS()
class TAG_ROGUE_API UTerrainMaker final : public UObject
{
	GENERATED_BODY()
	UPROPERTY()
	URogueAlpha_MapGenerator* Generator;

	
	void PlaceMapUnitBase(int32,int32) const;
	void AddGate(const URogueAlpha_MapGenerator::FCell*,const URogueAlpha_MapGenerator::EDirection) const;
	void AddRoomObjects() const;
	void SpawnGlobe() const;

public:
	float CellSize;
	UTerrainMaker();
	void Construct(URogueAlpha_MapGenerator*, float);
	FVector Cie_Convert(int32,int32, float) const;
	static TArray<int32> Cie_Invert(FVector);
	void Build();
};
