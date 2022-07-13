// Fill out your copyright notice in the Description page of Project Settings.


#include "HoloGlobe.h"


// Sets default values
AHoloGlobe::AHoloGlobe()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	CollisionMesh = CreateDefaultSubobject<UBoxComponent>(FName("Collision Mesh"));
	Globe = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Globe"));
	Grid = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Grid"));
	Orbit1 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Orbit1"));
	Orbit2 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Orbit2"));
	Orbit3 = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Orbit3"));
	SetRootComponent(CollisionMesh);
	CollisionMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Globe->SetupAttachment(CollisionMesh);
	Grid->SetupAttachment(CollisionMesh);
	Orbit1->SetupAttachment(CollisionMesh);
	Orbit2->SetupAttachment(CollisionMesh);
	Orbit3->SetupAttachment(CollisionMesh);
	GlobeInitialPos = FVector(0,0,600);
	SetActorScale3D(FVector(0.5,0.5,0.5));
	SetReplicates(true);
}

// Called when the game starts or when spawned
void AHoloGlobe::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = Cast<UTag_RogueGameInstance, UGameInstance>(GetGameInstance());
	GameInstance->LoadAssets();
	GlobeMesh = GameInstance->GetAssetObject<UStaticMesh>(TEXT("SF_Globe_GlobeTexture"));
	GridMesh = GameInstance->GetAssetObject<UStaticMesh>(TEXT("SF_Glove_GlobeGrid"));
	Orbit1Mesh = GameInstance->GetAssetObject<UStaticMesh>(TEXT("SF_Globe_Orbit1"));
	Orbit2Mesh = GameInstance->GetAssetObject<UStaticMesh>(TEXT("SF_Globe_Orbit2"));
	Orbit3Mesh = GameInstance->GetAssetObject<UStaticMesh>(TEXT("SF_Globe_Orbit3"));
	Globe->SetStaticMesh(GlobeMesh);
	Grid->SetStaticMesh(GridMesh);
	Orbit1->SetStaticMesh(Orbit1Mesh);
	Orbit2->SetStaticMesh(Orbit2Mesh);
	Orbit3->SetStaticMesh(Orbit3Mesh);
	Globe->SetRelativeLocation(GlobeInitialPos);
	Grid->SetRelativeLocation(GlobeInitialPos);
	Orbit1->SetRelativeLocation(GlobeInitialPos);
	Orbit2->SetRelativeLocation(GlobeInitialPos);
	Orbit3->SetRelativeLocation(GlobeInitialPos);
}

// Called every frame
void AHoloGlobe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	UStaticMeshComponent* Ins[] = {Globe, Grid, Orbit1, Orbit2, Orbit3};
	for(int32 i=0;i<5;i++)
	{
		constexpr float Cycle = 4;
		constexpr float DegreePerSecond = 30;
		constexpr float VerticalAmplitude = 3;
		Ins[i]->AddRelativeRotation(FRotator(0, DegreePerSecond*DeltaTime, 0));
		Ins[i]->SetRelativeLocation(FVector(0,0,VerticalAmplitude*FMath::Sin(2*PI/Cycle*TimeSinceCreated))+GlobeInitialPos);
	}
	TimeSinceCreated += DeltaTime;
}

