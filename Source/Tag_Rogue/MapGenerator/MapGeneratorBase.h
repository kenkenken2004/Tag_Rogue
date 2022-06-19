// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MapGeneratorBase.generated.h"
#define WALL false
#define PATH true

/**
 * 
 */
UCLASS()
class TAG_ROGUE_API UMapGeneratorBase : public UObject
{
	GENERATED_BODY()
private:
	enum struct ECellType;
	struct FCell;
public:
	int32 Map_Height, Map_Width;
	UMapGeneratorBase();
};
