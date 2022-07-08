// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tag_Rogue/Tag_RogueGameInstance.h"
#include "Tag_Rogue/Map/RogueAlpha_MapGenerator.h"
#include "Tag_Rogue/Map/TerrainMaker.h"
#include "MiniMap.generated.h"

UCLASS()
class TAG_ROGUE_API AMiniMap : public AActor
{
	GENERATED_BODY()
	UPROPERTY()
	UTag_RogueGameInstance* GameInstance;
	UPROPERTY()
	UStaticMeshComponent* Display;
	UPROPERTY()
	UStaticMesh* DisplayMesh;
	UPROPERTY()
	URogueAlpha_MapGenerator* Generater;
	UPROPERTY()
	UTerrainMaker* Maker;
	UPROPERTY()
	UMaterialInstanceDynamic* MapMaterial;
public:
	// Sets default values for this actor's properties
	AMiniMap();
	void Initialize(URogueAlpha_MapGenerator* Gen, UTerrainMaker*);
	UTexture* CreateMiniMapTexture() const;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
