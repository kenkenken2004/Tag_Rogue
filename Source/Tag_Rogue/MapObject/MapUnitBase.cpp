// Fill out your copyright notice in the Description page of Project Settings.

#include "MapUnitBase.h"

#include "Tag_Rogue/Tag_RogueGameInstance.h"

void AMapUnitBase::InitEnumName()
{
	EnumName = TMap<EMeshType, FName>();
 	EnumName.Add(EMeshType::GateCornerOne, FName(TEXT("GateCornerOne")));
 	EnumName.Add(EMeshType::GateCornerTwo, FName(TEXT("GateCornerTwo")));
 	EnumName.Add(EMeshType::GateNormal, FName(TEXT("GateNormal")));
 	EnumName.Add(EMeshType::PathCorner, FName(TEXT("PathCorner")));
 	EnumName.Add(EMeshType::PathCrossRoad, FName(TEXT("PathCrossRoad")));
 	EnumName.Add(EMeshType::PathStraight, FName(TEXT("PathStraight")));
 	EnumName.Add(EMeshType::PathTJunction, FName(TEXT("PathTJunction")));
 	EnumName.Add(EMeshType::RoomCorner, FName(TEXT("RoomCorner")));
 	EnumName.Add(EMeshType::RoomNormal, FName(TEXT("RoomNormal")));
 	EnumName.Add(EMeshType::RoomSide, FName(TEXT("RoomSide")));
}

// Sets default values
AMapUnitBase::AMapUnitBase()
{
	InitEnumName();
	BodyComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MapUnit"));
	SetRootComponent(BodyComponent);
	BodyComponent->SetRelativeScale3D(FVector(1,1,2));
	bReplicates = true;
	BodyComponent->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void AMapUnitBase::BeginPlay()
{
	Super::BeginPlay();
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AMapUnitBase::SetMesh(const FName Path)
{
	BodyMesh = UTag_RogueGameInstance::GetInstance()->GetAssetObject<UStaticMesh>(Path);
	BodyComponent->SetStaticMesh(BodyMesh);
}

// Called every frame
void AMapUnitBase::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMapUnitBase::SetMeshType(const EMeshType Type)
{
	MeshType = Type;
	SetMesh(EnumName[Type]);
}
