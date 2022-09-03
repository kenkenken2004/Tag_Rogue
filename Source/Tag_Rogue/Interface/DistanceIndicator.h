// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Tag_Rogue/Tag_RogueGameInstance.h"
#include "Tag_Rogue/Character/CharacterBase.h"
#include "DistanceIndicator.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TAG_ROGUE_API UDistanceIndicator : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	UDistanceIndicator();
	UPROPERTY()
	UTag_RogueGameInstance* GameInstance;
	UPROPERTY(Replicated)
	ACharacterBase* OwnerPlayer;
	UPROPERTY()
	UMaterialInstanceDynamic* IndicatorMaterial;
	UPROPERTY(Replicated)
	float Distance;
	UPROPERTY(Replicated)
	float Closeness;
	UPROPERTY()
	float StandardMaxDistance = 500;
protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(NetMulticast, Reliable)
	void Initialize();
	UFUNCTION(NetMulticast, Reliable)
	void UpdateIndicator();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps)const override;
};
