// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "InteriorOne.generated.h"

UCLASS()
class TAG_ROGUE_API AInteriorOne : public AActor
{
	GENERATED_BODY()
	UPROPERTY()
	UStaticMeshComponent* Basement = nullptr;
	UPROPERTY()
	UStaticMeshComponent* Globe = nullptr;
	UPROPERTY()
	UBoxComponent* CollisionMesh = nullptr;
	UPROPERTY()
	UStaticMesh* BasementMesh = nullptr;
	UPROPERTY()
	UStaticMesh* GlobeMesh = nullptr;
	UPROPERTY(EditAnywhere)
	UMaterial* BasementMaterial = nullptr;
	UPROPERTY(EditAnywhere)
	UMaterial* GlobeMaterial = nullptr;

	FVector GlobeInitialPos;
	float TimeSinceCreated = 0;
public:	
	// Sets default values for this actor's properties
	AInteriorOne();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
