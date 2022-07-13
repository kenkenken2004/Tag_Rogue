// Fill out your copyright notice in the Description page of Project Settings.
#include "GameMap_Alpha.h"
#include "Kismet/GameplayStatics.h"
#include "Tag_Rogue/Interface/LimitCountComponent.h"

void AGameMap_Alpha::TickOnServer_Implementation(const float DeltaSecond)
{
	if(GameInstance->FloatRemainingTime>0)GameInstance->FloatRemainingTime -= DeltaSecond;
}

AGameMap_Alpha::AGameMap_Alpha()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AGameMap_Alpha::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	TickOnServer(DeltaSeconds);
}


void AGameMap_Alpha::BeginPlay()
{
	Super::BeginPlay();
	Initialize(GetGameInstance());
}

void AGameMap_Alpha::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AGameMap_Alpha::Initialize_Implementation(UGameInstance* GameIns)
{
	GameInstance = static_cast<UTag_RogueGameInstance*>(GameIns);
	GameInstance->LoadAssets();
	GameInstance->InitializeMapBuilders();
	GameInstance->TerrainMaker->Build();
}