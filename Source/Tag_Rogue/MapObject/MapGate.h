// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "Tag_Rogue/Tag_RogueGameInstance.h"
#include "MapGate.generated.h"

UCLASS()
class TAG_ROGUE_API AMapGate final : public AActor
{
	GENERATED_BODY()
	enum struct EGateState
	{
		Opening,
		Closing,
		Opened,
		Closed
	};
	
	UPROPERTY()
	UTag_RogueGameInstance* GameInstance;
	UPROPERTY()
	UBoxComponent* Collision;
	UPROPERTY()
	UStaticMeshComponent* OuterBottomGate;
	UPROPERTY()
	UStaticMeshComponent* OuterTopGate;
	UPROPERTY()
	UStaticMeshComponent* InnerBottomGate;
	UPROPERTY()
	UStaticMeshComponent* InnerTopGate;
	UPROPERTY()
	UStaticMesh* OuterBottomGateMesh;
	UPROPERTY()
	UStaticMesh* OuterTopGateMesh;
	UPROPERTY()
	UStaticMesh* InnerBottomGateMesh;
	UPROPERTY()
	UStaticMesh* InnerTopGateMesh;
	
	EGateState GateState = EGateState::Closed;
	float LastApproach = 0.0;
	const float GateOpenedHeight = 50.0;

public:
	// Sets default values for this actor's properties
	AMapGate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void Open();
	void Close();
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
