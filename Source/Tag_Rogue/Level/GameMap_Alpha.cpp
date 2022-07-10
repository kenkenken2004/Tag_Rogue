// Fill out your copyright notice in the Description page of Project Settings.
#include "GameMap_Alpha.h"
#include "Kismet/GameplayStatics.h"
#include "Tag_Rogue/Character/CharacterBase.h"
#include "Tag_Rogue/Interface/LimitCountComponent.h"
#include "Tag_Rogue/Interface/MiniMap.h"
#include "Tag_Rogue/Interface/MiniMapComponent.h"
#include "Tag_Rogue/MapObject/HoloGlobe.h"

AGameMap_Alpha::AGameMap_Alpha()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AGameMap_Alpha::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(IsValidChecked(GameInstance))
	{
		if(GameInstance->FloatRemainingTime>0)GameInstance->FloatRemainingTime -= DeltaSeconds;
        	if(FMath::CeilToInt32(GameInstance->FloatRemainingTime) < GameInstance->IntRemainingTime)
        	{
        		GameInstance->IntRemainingTime = FMath::CeilToInt32(GameInstance->FloatRemainingTime);
        		Player0->LimitCount->UpdateNumbers();
        	}
	}
}


void AGameMap_Alpha::BeginPlay()
{
	Super::BeginPlay();
	Player0 = static_cast<ACharacterBase*>(UGameplayStatics::GetPlayerPawn(this, 0));
	Initialize(300,50,50);
	Generator->BuildMap();
	TerrainMaker->Build();
	SpawnPlayer();
	Generator->GetStructureString();
	GameInstance->FloatRemainingTime = GameTimeLimit;
	GameInstance->IntRemainingTime = GameTimeLimit;
}

void AGameMap_Alpha::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AGameMap_Alpha::Initialize(const int32 MapCellSize, const int32 MapHeight, const int32 MapWidth)
{
	GameInstance = static_cast<UTag_RogueGameInstance*>(GetGameInstance());
	GameInstance->LoadAssets();
	CellSize = MapCellSize;
	Generator = NewObject<URogueAlpha_MapGenerator>(GetWorld());
	Generator->Construct(MapHeight,MapWidth);
	Generator->SetStructureParam(UMapGeneratorBase::EType::Plaza, 9, 9, 4);
	Generator->SetStructureParam(UMapGeneratorBase::EType::Room, 5, 5, 9);
	TerrainMaker = NewObject<UTerrainMaker>(GetWorld());
	TerrainMaker->Construct(Generator,GameInstance,MapCellSize);
	
}

APawn* AGameMap_Alpha::SpawnPlayer() const
{
	
	int32 X=0;int32 Y=0;
	while (Generator->GetCell(Y,X)->Attribution==UMapGeneratorBase::EType::Wall)
	{
		X = FMath::RandRange(0,Generator->MapWidth-1);
		Y = FMath::RandRange(0,Generator->MapHeight-1);
	}
	
	Player0->SetActorLocation(TerrainMaker->Cie_Convert(Y,X,CellSize));
	Player0->MiniMap->Initialize(Generator, TerrainMaker);
	Player0->LimitCount->Initialize();
	Player0->LimitCount->UpdateNumbers();
	return Player0;
}