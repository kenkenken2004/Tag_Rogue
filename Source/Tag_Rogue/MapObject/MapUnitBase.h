// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MapUnitBase.generated.h"

UCLASS()
class TAG_ROGUE_API AMapUnitBase : public AActor
{
	GENERATED_BODY()
	UPROPERTY()
	UStaticMeshComponent* BodyComponent;
	UPROPERTY()
	UStaticMesh* BodyMesh;
	UPROPERTY()
	UMaterial* BodyMaterial;
public:	
	// Sets default values for this actor's properties
	AMapUnitBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
