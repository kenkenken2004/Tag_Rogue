// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGate.h"


// Sets default values
AMapGate::AMapGate()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Collision = CreateDefaultSubobject<UBoxComponent>(FName("CollisionMesh"));
	OuterBottomGate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OuterBottom"));
	OuterTopGate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OuterTop"));
	InnerBottomGate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InnerBottom"));
	InnerTopGate = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InnerTop"));
	SetRootComponent(Collision);
	OuterBottomGate->SetupAttachment(RootComponent);
	OuterTopGate->SetupAttachment(RootComponent);
	InnerBottomGate->SetupAttachment(RootComponent);
	InnerTopGate->SetupAttachment(RootComponent);

	//SetUp Detection Box
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetCollisionResponseToAllChannels(ECR_Ignore);
	Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Collision->InitBoxExtent(FVector(150,100,100));
	TFunction<void(void)> Func = [this]()
	{
		OnOverlapBegin();
	};
	Collision->OnComponentBeginOverlap.Add()
}

// Called when the game starts or when spawned
void AMapGate::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = Cast<UTag_RogueGameInstance, UGameInstance>(GetGameInstance());
	InnerBottomGateMesh = GameInstance->GetAssetObject<UStaticMesh>(TEXT("SF_Gate_InnerBottomGate"));
	InnerTopGateMesh    = GameInstance->GetAssetObject<UStaticMesh>(TEXT("SF_Gate_InnerTopGate"));
	OuterBottomGateMesh = GameInstance->GetAssetObject<UStaticMesh>(TEXT("SF_Gate_OuterBottomGate"));
	OuterTopGateMesh    = GameInstance->GetAssetObject<UStaticMesh>(TEXT("SF_Gate_OuterTopGate"));
	InnerBottomGate->SetStaticMesh(InnerBottomGateMesh);
	InnerTopGate->SetStaticMesh(InnerTopGateMesh);
	OuterBottomGate->SetStaticMesh(OuterBottomGateMesh);
	OuterTopGate->SetStaticMesh(OuterTopGateMesh);
}

void AMapGate::Open()
{
}

void AMapGate::Close()
{
}

void AMapGate::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

// Called every frame
void AMapGate::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	LastApproach+=DeltaTime;
	
	switch (GateState)
	{
	case EGateState::Closing:
	case EGateState::Opening:
	case EGateState::Closed:
	case EGateState::Opened:;
	}
}

