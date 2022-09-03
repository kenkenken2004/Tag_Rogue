// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Tag_Rogue/Character/CharacterBase.h"
#include "RadarComponent.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TAG_ROGUE_API URadarComponent : public UStaticMeshComponent
{
	GENERATED_BODY()
	URadarComponent();
	UPROPERTY()
	UTag_RogueGameInstance* GameInstance;
	UPROPERTY(Replicated)
	ACharacterBase* OwnerPlayer;
	UPROPERTY()
	UMaterialInstanceDynamic* RadarMaterial;
	UPROPERTY(Replicated)
	float EnemyDistance = 0;
	UPROPERTY(Replicated)
	float EnemyDirection = 0;

protected:
	virtual void BeginPlay() override;
public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(NetMulticast, Reliable)
	void Initialize();
	UFUNCTION(NetMulticast, Reliable)
	void UpdateRadar();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps)const override;

};
