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
	GENERATED_BODY()
protected:
	TMap<EType, int32> StructureSize;
	
	void RandomPlaceSpace();
	void BuildSpaceLayer();
public:
	URogueAlpha_MapGenerator(int32, int32);
	URogueAlpha_MapGenerator();
};
