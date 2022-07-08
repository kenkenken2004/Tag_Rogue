// Fill out your copyright notice in the Description page of Project Settings.
#include "GameMap_Alpha.h"
#include "Kismet/GameplayStatics.h"
#include "Tag_Rogue/Interface/MiniMap.h"
#include "Tag_Rogue/MapObject/HoloGlobe.h"

AGameMap_Alpha::AGameMap_Alpha()
{
}


void AGameMap_Alpha::BeginPlay()
{
	Initialize(300,50,50);
	
	Generator->BuildMap();
	TerrainMaker->Build();
	SpawnPlayer();
	Generator->GetStructureString();
	AMiniMap* MiniMap = GetWorld()->SpawnActor<AMiniMap>(FVector(0,0,400), FRotator(0,180,0));
	MiniMap->Initialize(Generator);
	Super::BeginPlay();
}

void AGameMap_Alpha::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void AGameMap_Alpha::Initialize(const int32 MapCellSize, const int32 MapHeight, const int32 MapWidth)
{
	GameInstance = static_cast<UTag_RogueGameInstance*>(GetGameInstance());
	for(int32 i=0;i<AssetsPathArray.Num();i++)GameInstance->LoadAssets(AssetsPathArray[i]);
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
	APawn* Player0Pawn = UGameplayStatics::GetPlayerPawn(this, 0);
	int32 X=0;int32 Y=0;
	while (Generator->GetCell(Y,X)->Attribution==UMapGeneratorBase::EType::Wall)
	{
		X = FMath::RandRange(0,Generator->MapWidth-1);
		Y = FMath::RandRange(0,Generator->MapHeight-1);
	}
	
	Player0Pawn->SetActorLocation(TerrainMaker->Cie_Convert(Y,X,CellSize));
	return Player0Pawn;
}