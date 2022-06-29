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
	
public:
	AGameMap_Alpha();
protected:
	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
public:
	UFUNCTION(BlueprintImplementableEvent)
	void MyEvent();
};
