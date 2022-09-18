// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Desk.generated.h"

UCLASS()
class TAG_ROGUE_API ADesk : public AActor
{
	GENERATED_BODY()
	UPROPERTY()
	UStaticMeshComponent* DeskComponent;
	UPROPERTY()
	UStaticMesh* DeskMesh;
public:	
	// Sets default values for this actor's properties
	ADesk();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetDeskMesh();
};
