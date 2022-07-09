// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Tag_Rogue/Tag_RogueGameInstance.h"
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
	UPROPERTY()
	UStaticMesh* DisplayMesh;
	UPROPERTY()
	UMaterialInstanceDynamic* DigitLeft;
	UPROPERTY()
	UMaterialInstanceDynamic* DigitRight;
protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	void Initialize();
	void UpdateNumbers() const;
};
