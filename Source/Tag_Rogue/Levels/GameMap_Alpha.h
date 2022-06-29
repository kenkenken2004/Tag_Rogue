// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
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
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	FVector Cie_Convert(int32,int32, float) const;
	static TArray<int32> Cie_Invert(FVector);
	
public:
	AGameMap_Alpha();

public:
};
