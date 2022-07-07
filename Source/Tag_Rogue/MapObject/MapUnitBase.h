// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tag_Rogue/Tag_RogueGameInstance.h"
#include "MapUnitBase.generated.h"

UCLASS()
class TAG_ROGUE_API AMapUnitBase final : public AActor
{
	GENERATED_BODY()
	UPROPERTY()
	UTag_RogueGameInstance* GameInstance;
	UPROPERTY()
	UStaticMeshComponent* BodyComponent;
	UPROPERTY()
	UStaticMesh* BodyMesh;
public:
	
	enum struct EMeshType
	{
		GateCornerOne,
		GateCornerTwo,
		GateNormal,
		PathCorner,
		PathCrossRoad,
		PathStraight,
		PathTJunction,
		RoomCorner,
		RoomNormal,
		RoomSide,
		Null
	};
	
protected:
	TMap<EMeshType, FName> EnumName;
	void InitEnumName();
	EMeshType MeshType;
	
public:	
	// Sets default values for this actor's properties
	AMapUnitBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void SetMesh(FName);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetMeshType(EMeshType);
};
