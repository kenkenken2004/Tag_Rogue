// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMap_Alpha.h"
#include "Tag_Rogue/MapGenerator/RogueAlpha_MapGenerator.h"

AGameMap_Alpha::AGameMap_Alpha()
{
}


void AGameMap_Alpha::BeginPlay()
{
	CellSize = 1.0;
	URogueAlpha_MapGenerator* Generator = NewObject<URogueAlpha_MapGenerator>(GetWorld());
	Generator->Construct(50,50);
	Generator->BuildMap();
	const TSubclassOf<class AActor> SubClass = TSoftClassPtr<AActor>(FSoftObjectPath(TEXT("/Game/MapObject/MapUnit/MapBasicWall.MapBasicWall"))).LoadSynchronous();
	const FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
	const AActor* Instance = GetWorld()->SpawnActor<AActor>(FVector(0,0,100),FRotator(0,0,0));
	const FString Name = Instance->GetName();
	UE_LOG(LogTemp, Log,TEXT("%s"), *Name)
	Super::BeginPlay();
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


