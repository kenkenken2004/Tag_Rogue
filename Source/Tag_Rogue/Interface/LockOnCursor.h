// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "Tag_Rogue/Tag_RogueGameInstance.h"
#include "Tag_Rogue/Character/CharacterBase.h"
#include "LockOnCursor.generated.h"

/**
 * 
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TAG_ROGUE_API ULockOnCursor : public UStaticMeshComponent
{
	GENERATED_BODY()
	ULockOnCursor();
	UPROPERTY()
	UTag_RogueGameInstance* GameInstance;
	UPROPERTY(Replicated)
	ACharacterBase* OwnerPlayer;
	UPROPERTY()
	UMaterialInstanceDynamic* CursorMaterial;
	UPROPERTY(Replicated)
	float UVposX = 0;
	UPROPERTY(Replicated)
	float UVposY = 0;

protected:
	virtual void BeginPlay() override;
public:
	UPROPERTY(Replicated, BlueprintReadWrite)
	bool DoesIndicate = false;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(NetMulticast, Reliable)
	void Initialize();
	UFUNCTION(NetMulticast, Reliable)
	void UpdateCursor();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps)const override;

};
