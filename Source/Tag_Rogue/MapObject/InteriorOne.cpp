// Fill out your copyright notice in the Description page of Project Settings.


#include "InteriorOne.h"

// Sets default values
AInteriorOne::AInteriorOne()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionMesh = CreateDefaultSubobject<UBoxComponent>(FName("Collision Mesh"));
	SetRootComponent(CollisionMesh);
	Basement = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Cylinder"));
	Globe = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Sphere"));
	BasementMesh = LoadObject<UStaticMesh>(NULL, TEXT("/Engine/BasicShapes/Cylinder.Cylinder"),NULL, LOAD_None, NULL);
	GlobeMesh = LoadObject<UStaticMesh>(NULL, TEXT("/Engine/BasicShapes/Sphere.Sphere"),NULL, LOAD_None, NULL);
	if(BasementMaterial==nullptr)BasementMaterial = LoadObject<UMaterial>(NULL, TEXT("/Engine/BasicShapes/BasicShapeMaterial.BasicShapeMaterial"),NULL, LOAD_None, NULL);
	if(GlobeMaterial==nullptr)GlobeMaterial = LoadObject<UMaterial>(NULL, TEXT("/Engine/EngineMaterials/WorldGridMaterial.WorldGridMaterial"),NULL, LOAD_None, NULL);
	Basement->SetStaticMesh(BasementMesh);
	Globe->SetStaticMesh(GlobeMesh);
	Basement->SetMaterial(0, BasementMaterial);
	Globe->SetMaterial(0, GlobeMaterial);
	Basement->SetupAttachment(RootComponent);
	Globe->SetupAttachment(RootComponent);
	Basement->SetRelativeLocation(FVector(0,0,10));
	Basement->SetRelativeScale3D(FVector(1,1,0.2));
	Globe->SetRelativeLocation(FVector(0,0,200));
	GlobeInitialPos = Globe->GetRelativeLocation();
}

// Called when the game starts or when spawned
void AInteriorOne::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInteriorOne::Tick(float DeltaTime)
{
	constexpr float DegreePerSecond = 30;
	constexpr float VerticalAmplitude = 20;
	constexpr float Cycle = 4;
	Super::Tick(DeltaTime);
	Globe->AddRelativeRotation(FRotator(0, DegreePerSecond*DeltaTime, 0));
	Globe->SetRelativeLocation(FVector(0,0,VerticalAmplitude*FMath::Sin(2*PI/Cycle*TimeSinceCreated))+GlobeInitialPos);
	TimeSinceCreated += DeltaTime;
}

