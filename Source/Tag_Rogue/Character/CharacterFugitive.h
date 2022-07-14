// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterBase.h"
#include "CharacterFugitive.generated.h"

/**
 * 
 */
UCLASS()
class TAG_ROGUE_API ACharacterFugitive : public ACharacterBase
{
	GENERATED_BODY()
	
public:
	ACharacterFugitive();
	UFUNCTION()
	void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
};
