// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Tag_Rogue/Tag_RogueGameInstance.h"
#include "MapGate.generated.h"

UENUM(BlueprintType)
enum struct EGateState: uint8
{
	Opening UMETA(DisplayName="Opening"),
	Closing UMETA(DisplayName="Closing"),
	Opened UMETA(DisplayName="Opened"),
	Closed UMETA(DisplayName="Closed")
};

UCLASS(Config=Game)
class TAG_ROGUE_API AMapGate final : public AStaticMeshActor
{
	GENERATED_BODY()
	UPROPERTY()
	UTag_RogueGameInstance* GameInstance;
	UPROPERTY(Replicated)
	EGateState GateState = EGateState::Closed;
	const float GateOpenedHeight = 60.0;
	float GateHeight = 0;
	int32 OverlappingNumber = 0;
	
public:
	// Sets default values for this actor's properties
	AMapGate();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Config)
	float GateOpenCloseTime = 1;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UBoxComponent* Collision;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* OuterBottomGate;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* OuterTopGate;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* InnerBottomGate;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UStaticMeshComponent* InnerTopGate;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAudioComponent* OuterCloseAudio;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAudioComponent* InnerCloseAudio;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAudioComponent* OuterOpenAudio;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAudioComponent* InnerOpenAudio;

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
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps)const override;

};

