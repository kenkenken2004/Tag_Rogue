// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tag_Rogue/Tag_RogueGameInstance.h"
#include "Tag_Rogue/Character/CharacterBase.h"
#include "LimitCountComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TAG_ROGUE_API ULimitCountComponent : public UStaticMeshComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	ULimitCountComponent();
	UPROPERTY()
	UTag_RogueGameInstance* GameInstance;
	UPROPERTY(Replicated)
	UStaticMesh* DisplayMesh;
	UPROPERTY()
	UMaterialInstanceDynamic* DigitLeft;
	UPROPERTY(Replicated)
	int32 DigitLeftNumber;
	UPROPERTY()
	UMaterialInstanceDynamic* DigitRight;
	UPROPERTY(Replicated)
	int32 DigitRightNumber;
	UPROPERTY(Replicated)
	ACharacterBase* OwnerPlayer; 
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(NetMulticast, Reliable)
	void Initialize();
	void CheckShouldUpdateNumbers(float DeltaTime);
	UFUNCTION(NetMulticast, Reliable)
	void UpdateNumbers();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps)const override;
};
