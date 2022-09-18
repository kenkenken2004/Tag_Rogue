// Fill out your copyright notice in the Description page of Project Settings.


#include "Desk.h"

#include "Tag_Rogue/Tag_RogueGameInstance.h"

// Sets default values
ADesk::ADesk()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DeskComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Desk"));
	SetRootComponent(DeskComponent);
	DeskComponent->SetIsReplicated(true);
}

// Called when the game starts or when spawned
void ADesk::BeginPlay()
{
	Super::BeginPlay();
	PrimaryActorTick.bCanEverTick = true;
}

// Called every frame
void ADesk::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ADesk::SetDeskMesh()
{
	DeskMesh = UTag_RogueGameInstance::GetInstance()->GetAssetObject<UStaticMesh>(TEXT("DeskMesh"));
	DeskComponent->SetStaticMesh(DeskMesh);
}

