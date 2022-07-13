// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGate.h"

#include "Net/UnrealNetwork.h"


// Sets default values
AMapGate::AMapGate()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
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
	Collision->InitBoxExtent(FVector(75,50,50));
	Collision->OnComponentBeginOverlap.AddDynamic(this, &AMapGate::OnOverlapBegin);
	Collision->OnComponentEndOverlap.AddDynamic(this, &AMapGate::OnComponentOverlapEnd);
}

// Called when the game starts or when spawned
void AMapGate::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = Cast<UTag_RogueGameInstance, UGameInstance>(GetGameInstance());
	GameInstance->LoadAssets();
	InnerBottomGateMesh = GameInstance->GetAssetObject<UStaticMesh>(TEXT("SF_Gate_InnerBottomGate"));
	InnerTopGateMesh    = GameInstance->GetAssetObject<UStaticMesh>(TEXT("SF_Gate_InnerTopGate"));
	OuterBottomGateMesh = GameInstance->GetAssetObject<UStaticMesh>(TEXT("SF_Gate_OuterBottomGate"));
	OuterTopGateMesh    = GameInstance->GetAssetObject<UStaticMesh>(TEXT("SF_Gate_OuterTopGate"));
	InnerBottomGate->SetStaticMesh(InnerBottomGateMesh);
	InnerTopGate->SetStaticMesh(InnerTopGateMesh);
	OuterBottomGate->SetStaticMesh(OuterBottomGateMesh);
	OuterTopGate->SetStaticMesh(OuterTopGateMesh);
}

void AMapGate::DoorManipulation(const float DeltaTime)
{
	switch (GateState)
	{
	case EGateState::Closing:
		if(OuterTopGate->GetRelativeLocation().Z==0 && OuterBottomGate->GetRelativeLocation().Z==0)
		{
			GateState = EGateState::Closed;
		}
		else if(InnerTopGate->GetRelativeLocation().Z==0 && InnerBottomGate->GetRelativeLocation().Z==0)
		{
			OuterTopGate->AddRelativeLocation(FVector(0,0,-1*(DeltaTime/(GateOpenCloseTime/2))*GateOpenedHeight));
			OuterBottomGate->AddRelativeLocation(FVector(0,0,1*(DeltaTime/(GateOpenCloseTime/2))*GateOpenedHeight));
			if(OuterTopGate->GetRelativeLocation().Z<0)OuterTopGate->SetRelativeLocation(FVector(0,0,0));
			if(OuterBottomGate->GetRelativeLocation().Z>0)OuterBottomGate->SetRelativeLocation(FVector(0,0,0));
		}
		else
		{
			InnerTopGate->AddRelativeLocation(FVector(0,0,-1*(DeltaTime/(GateOpenCloseTime/2))*GateOpenedHeight));
			InnerBottomGate->AddRelativeLocation(FVector(0,0,1*(DeltaTime/(GateOpenCloseTime/2))*GateOpenedHeight));
			if(InnerTopGate->GetRelativeLocation().Z<0)InnerTopGate->SetRelativeLocation(FVector(0,0,0));
			if(InnerBottomGate->GetRelativeLocation().Z>0)InnerBottomGate->SetRelativeLocation(FVector(0,0,0));
		}
		break;
	case EGateState::Opening:
		if(InnerTopGate->GetRelativeLocation().Z==GateOpenedHeight && InnerBottomGate->GetRelativeLocation().Z==-GateOpenedHeight)
		{
			GateState = EGateState::Opened;
		}
		else if(OuterTopGate->GetRelativeLocation().Z==GateOpenedHeight && OuterBottomGate->GetRelativeLocation().Z==-GateOpenedHeight)
		{
			InnerTopGate->AddRelativeLocation(FVector(0,0,1*(DeltaTime/(GateOpenCloseTime/2))*GateOpenedHeight));
			InnerBottomGate->AddRelativeLocation(FVector(0,0,-1*(DeltaTime/(GateOpenCloseTime/2))*GateOpenedHeight));
			if(InnerTopGate->GetRelativeLocation().Z>GateOpenedHeight)InnerTopGate->SetRelativeLocation(FVector(0,0,GateOpenedHeight));
			if(InnerBottomGate->GetRelativeLocation().Z<-GateOpenedHeight)InnerBottomGate->SetRelativeLocation(FVector(0,0,-GateOpenedHeight));
		}
		else
		{
			OuterTopGate->AddRelativeLocation(FVector(0,0,1*(DeltaTime/(GateOpenCloseTime/2))*GateOpenedHeight));
			OuterBottomGate->AddRelativeLocation(FVector(0,0,-1*(DeltaTime/(GateOpenCloseTime/2))*GateOpenedHeight));
			if(OuterTopGate->GetRelativeLocation().Z>GateOpenedHeight)OuterTopGate->SetRelativeLocation(FVector(0,0,GateOpenedHeight));
			if(OuterBottomGate->GetRelativeLocation().Z<-GateOpenedHeight)OuterBottomGate->SetRelativeLocation(FVector(0,0,-GateOpenedHeight));
		}
		break;
	default:
		break;
	}
}

void AMapGate::Open()
{
	if(GateState!=EGateState::Opened)GateState=EGateState::Opening;
}

void AMapGate::Close()
{
	if(GateState!=EGateState::Closed)GateState=EGateState::Closing;
}

void AMapGate::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                              UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OverlappingNumber++;
	if(GateState!=EGateState::Opened)
	{
		GateState=EGateState::Opening;
	}
}

void AMapGate::OnComponentOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	OverlappingNumber--;
	if(OverlappingNumber<=0&&GateState!=EGateState::Closed)
	{
		GateState=EGateState::Closing;
	}
}

// Called every frame
void AMapGate::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
	DoorManipulation(DeltaTime);
}

void AMapGate::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMapGate, GateState);
	
}