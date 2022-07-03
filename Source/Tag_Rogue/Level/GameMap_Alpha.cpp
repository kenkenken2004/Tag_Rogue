// Fill out your copyright notice in the Description page of Project Settings.
#include "GameMap_Alpha.h"

#include "Tag_Rogue/MapObject/InteriorOne.h"
#include "Tag_Rogue/MapObject/MapUnitBase.h"


AGameMap_Alpha::AGameMap_Alpha()
{
	BP_Reference = TMap<FString, TSubclassOf<AActor>>();
}


void AGameMap_Alpha::BeginPlay()
{
	CellSize = 100.0;
	Generator = NewObject<URogueAlpha_MapGenerator>(GetWorld());
	Generator->Construct(50,50);
	Generator->SetStructureParam(UMapGeneratorBase::EType::Plaza, 9, 9, 4);
    Generator->SetStructureParam(UMapGeneratorBase::EType::Room, 5, 5, 9);
	Generator->BuildMap();
	AddClass(TEXT("MapUnitBase"), TEXT("/Game/MapObject/MapUnit/MapBasicWall.MapBasicWall_C"));
	AddClass(TEXT("InteriorOne"), TEXT("/Game/MapObject/InteriorOne/InteriorOne.InteriorOne_C"));
	PlaceCubes();
	Super::BeginPlay();
}

void AGameMap_Alpha::PlaceCubes()
{
	for(int32 y=0;y<Generator->MapHeight;y++)
	{
		for (int32 x=0;x<Generator->MapWidth;x++)
		{
			if (Generator->GetCell(y, x)->Attribution==UMapGeneratorBase::EType::Wall)
			{
				//GetWorld()->SpawnActor<AActor>(GetClass(TEXT("MapUnitBase")), Cie_Convert(y,x,0), FRotator(0,0,0));
				GetWorld()->SpawnActor<AMapUnitBase>(Cie_Convert(y,x,0), FRotator(0,0,0));
			}
			
		}
	}
	for(int32 i=0;i<Generator->SpaceList.Num();i++)
	{
		if(Generator->SpaceList[i]->Attribution == UMapGeneratorBase::EType::Plaza)
		{
			FVector Pos = Cie_Convert(Generator->SpaceList[i]->GetCenterCell()->Py, Generator->SpaceList[i]->GetCenterCell()->Px, 0);
			//GetWorld()->SpawnActor<AActor>(GetClass(TEXT("InteriorOne")), Pos, FRotator(0,0,0));
			GetWorld()->SpawnActor<AInteriorOne>(Pos, FRotator(0,0,0));
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

void AGameMap_Alpha::AddClass(const FString Name, const FString ReferencePath)
{
	BP_Reference.Add(FString(Name), TSoftClassPtr<AActor>(FSoftObjectPath(ReferencePath)).LoadSynchronous());
}

TSubclassOf<AActor> AGameMap_Alpha::GetClass(const FString Name)
{
	return BP_Reference[Name];
}
