// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMap_Alpha.h"
#include "Tag_Rogue/MapGenerator/RogueAlpha_MapGenerator.h"

AGameMap_Alpha::AGameMap_Alpha()
{
	
}


void AGameMap_Alpha::BeginPlay()
{
	URogueAlpha_MapGenerator* Generator = NewObject<URogueAlpha_MapGenerator>(GetWorld());
	Generator->Construct(50,50);
	Generator->BuildMap();
	Super::BeginPlay();
}
void AGameMap_Alpha::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}


