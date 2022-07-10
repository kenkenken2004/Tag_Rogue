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
	const float GateOpenedHeight = 60.0;
	float GateHeight = 0;
	int32 OverlappingNumber = 0;
	UPROPERTY(EditAnywhere)
	float GateOpenCloseTime = 0.5;
public:
	// Sets default values for this actor's properties
	AMapGate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void DoorManipulation(float);
	UFUNCTION(BlueprintCallable)
	void Open();
	UFUNCTION(BlueprintCallable)
	void Close();
	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent,AActor* OtherActor,UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult);
	UFUNCTION()
	void OnComponentOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
