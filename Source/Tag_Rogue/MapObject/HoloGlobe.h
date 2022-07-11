// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMeshActor.h"
#include "GameFramework/Actor.h"
#include "Tag_Rogue/Tag_RogueGameInstance.h"
#include "HoloGlobe.generated.h"

UCLASS()
class TAG_ROGUE_API AHoloGlobe : public AStaticMeshActor
{
	GENERATED_BODY()
public:
	UPROPERTY()
	UTag_RogueGameInstance* GameInstance;
	UPROPERTY()
	UStaticMeshComponent* Globe;
	UPROPERTY()
	UStaticMeshComponent* Grid;
	UPROPERTY()
	UStaticMeshComponent* Orbit1;
	UPROPERTY()
	UStaticMeshComponent* Orbit2;
	UPROPERTY()
	UStaticMeshComponent* Orbit3;
	UPROPERTY()
	UBoxComponent* CollisionMesh;
	UPROPERTY()
	UStaticMesh* GlobeMesh;
	UPROPERTY()
	UStaticMesh* GridMesh;
	UPROPERTY()
	UStaticMesh* Orbit1Mesh;
	UPROPERTY()
	UStaticMesh* Orbit2Mesh;
	UPROPERTY()
	UStaticMesh* Orbit3Mesh;
	UPROPERTY()
	FVector GlobeInitialPos = FVector(0,0,0);
	UPROPERTY(EditAnywhere)
	float TimeSinceCreated = 0;
public:	
	// Sets default values for this actor's properties
	AHoloGlobe();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
