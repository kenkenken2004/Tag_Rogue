// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Tag_Rogue/Tag_RogueGameInstance.h"
#include "Tag_Rogue/Character/CharacterBase.h"
#include "Tag_Rogue/Map/RogueAlpha_MapGenerator.h"
#include "Tag_Rogue/Map/TerrainMaker.h"
#include "MiniMapComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TAG_ROGUE_API UMiniMapComponent final : public UStaticMeshComponent
{
	GENERATED_BODY()
public:
	UPROPERTY(Replicated)
	ACharacterBase* OwnerPlayer;
	UPROPERTY()
	UTag_RogueGameInstance* GameInstance;
	UPROPERTY(Replicated)
	UStaticMesh* DisplayMesh;
	UPROPERTY()
	URogueAlpha_MapGenerator* Generater;
	UPROPERTY()
	UTerrainMaker* Maker;
	UPROPERTY()
	UMaterialInstanceDynamic* MapMaterial;
	UPROPERTY(Replicated)
	float CellSize;
	UPROPERTY(Replicated)
	int32 MapHeight;
	UPROPERTY(Replicated)
	int32 MapWidth;
	UPROPERTY(Replicated)
	float Scale;
	UPROPERTY(Replicated)
	TArray<bool> TextureBitArray;
	UPROPERTY(Replicated)
	float EnemyDirection;
	UPROPERTY(Replicated)
	float EnemyDistance;
	UPROPERTY(Replicated)
	float RadarSensitivity;
	UPROPERTY(Replicated)
	float RadarDistExp;
	UPROPERTY(Replicated)
	float PointerScale;
	// Sets default values for this component's properties
	UMiniMapComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(NetMulticast, Reliable)
	void Initialize();
	void InitializeByServer();
	UFUNCTION(NetMulticast, Reliable)
	void UpdateMapDirection();
	UTexture* CreateMiniMapTexture() const;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps)const override;
};

