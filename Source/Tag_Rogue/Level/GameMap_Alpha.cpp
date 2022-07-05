// Fill out your copyright notice in the Description page of Project Settings.
#include "GameMap_Alpha.h"
#include "Tag_Rogue/MapObject/MapUnitBase.h"


AGameMap_Alpha::AGameMap_Alpha()
{
}


void AGameMap_Alpha::BeginPlay()
{
	GameInstance = Cast<UTag_RogueGameInstance, UGameInstance>(GetGameInstance());
	GameInstance->LoadAssets(FName(TEXT("/Game/MapObject/Mesh/")));
	GameInstance->LoadAssets(FName(TEXT("/Game/MapObject/Material/")));
	CellSize = 100.0;
	Generator = NewObject<URogueAlpha_MapGenerator>(GetWorld());
	Generator->Construct(50,50);
	Generator->SetStructureParam(UMapGeneratorBase::EType::Plaza, 9, 9, 4);
    Generator->SetStructureParam(UMapGeneratorBase::EType::Room, 5, 5, 9);
	Generator->BuildMap();
	PlaceCubes();
	Super::BeginPlay();
}

void AGameMap_Alpha::PlaceCubes() const
{
	for(int32 y=0;y<Generator->MapHeight;y++)
	{
		for (int32 x=0;x<Generator->MapWidth;x++)
		{
			if (Generator->GetCell(y, x)->Attribution==UMapGeneratorBase::EType::Wall)
			{
				
				GetWorld()->SpawnActor<AMapUnitBase>(Cie_Convert(y,x,0), FRotator(0,0,0));
			}
			
		}
	}
}


void AGameMap_Alpha::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

FVector AGameMap_Alpha::Cie_Convert(const int32 PosY, const int32 PosX, const float Height) const
{
	return FVector((PosX+0.5)*CellSize,(PosY+0.5)*CellSize,Height);
}

TArray<int32> AGameMap_Alpha::Cie_Invert(const FVector PosVector)
{
	return {FMath::RoundToInt32(PosVector.Y),FMath::RoundToInt32(PosVector.X)};
}

