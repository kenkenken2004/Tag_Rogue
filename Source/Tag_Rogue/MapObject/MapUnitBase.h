// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "MapUnitBase.generated.h"

UCLASS()
class TAG_ROGUE_API AMapUnitBase final : public AStaticMeshActor
{
	GENERATED_BODY()
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
	// Sets default values for this actor's properties
	AMapUnitBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	void SetMesh(FName);

public:
	TMap<EMeshType, FName> EnumName;
	void InitEnumName();
	EMeshType MeshType;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void SetMeshType(EMeshType);
};
