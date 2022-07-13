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
	UPROPERTY(Replicated)
	UMaterialInstanceDynamic* MapMaterial;
	// Sets default values for this component's properties
	UMiniMapComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	UFUNCTION(Server,Reliable)
	void Initialize();
	UFUNCTION(Server, Reliable)
	void UpdateMapDirection();
	UTexture* CreateMiniMapTexture() const;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps)const override;
};

