// Fill out your copyright notice in the Description page of Project Settings.

#include "MapUnitBase.h"

// Sets default values
AMapUnitBase::AMapUnitBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BodyComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Wall Body"));
	
	BodyMesh = LoadObject<UStaticMesh>(NULL, TEXT("/Engine/BasicShapes/Cube.Cube"), NULL, LOAD_None, NULL);
	BodyMaterial = LoadObject<UMaterial>(NULL, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"),NULL, LOAD_None, NULL);
	BodyComponent->SetStaticMesh(BodyMesh);
	BodyComponent->SetMaterial(0, BodyMaterial);
	BodyComponent->SetRelativeLocation(FVector(0,0,100));
	BodyComponent->SetRelativeScale3D(FVector(1,1,2));
	BodyComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AMapUnitBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMapUnitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

