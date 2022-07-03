// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MapGeneratorBase.h"
#include "RogueAlpha_MapGenerator.generated.h"

/**
MapGeneratorBaseの派生。
最初に部屋を配置し、その後に領域を設定、隣接する部屋を通路で接続する。
 */
UCLASS()
class TAG_ROGUE_API URogueAlpha_MapGenerator final : public UMapGeneratorBase
{
public:
	GENERATED_BODY()
	int32 Space_Margin = 0;
	TMap<EType, FRect> StructureSize;
	TMap<EType, int32> StructureNumber;
	TArray<TArray<FSpace>> SpaceAdjacencyMatrix;
	bool RandomPlaceSpace(EType, int32);
	bool SetPath(const FArea*, const FArea*);
	TArray<int32> BuildSpace();
	void BuildArea();
	void BuildPath();
public:
	URogueAlpha_MapGenerator(int32, int32);
	URogueAlpha_MapGenerator();
	void SetStructureParam(EType, int32, int32, int32);
	TArray<FString> GetStructureString();
	TArray<FString> GetAreaString();
	void Construct(int32, int32) override;
	void BuildMap();
};
