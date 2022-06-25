// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapGeneratorBase.h"
#include "RogueAlpha_MapGenerator.generated.h"

/**
 * 
 */
UCLASS()
class TAG_ROGUE_API URogueAlpha_MapGenerator : public UMapGeneratorBase
{
public:
	GENERATED_BODY()
	TMap<EType, FRect> StructureSize;
	TMap<EType, int32> StructureNumber;
	TArray<TArray<FSpace>> SpaceAdjacencyMatrix;
	bool RandomPlaceSpace(EType);
	bool SetDirectPath(FArea*, FArea*);
	bool SetTurnedPath(FArea*, FArea*);
	TArray<int32> BuildSpace();
	void BuildArea();
	void BuildPath();
public:
	URogueAlpha_MapGenerator(int32, int32);
	URogueAlpha_MapGenerator();
	void SetStructureParam(EType, int32, int32, int32);
	TArray<FString> GetStructureString();
	TArray<FString> GetAreaString();
};
