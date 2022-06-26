// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMap_Alpha.h"
#include "Tag_Rogue/MapGenerator/RogueAlpha_MapGenerator.h"

AGameMap_Alpha::AGameMap_Alpha()
{
	
}


void AGameMap_Alpha::BeginPlay()
{
<<<<<<< HEAD
	URogueAlpha_MapGenerator Generator = URogueAlpha_MapGenerator(50, 50);
	Generator.GetMapString();
=======
	
	URogueAlpha_MapGenerator *Generator = new URogueAlpha_MapGenerator;
	*Generator = URogueAlpha_MapGenerator(50,50);
	//URogueAlpha_MapGenerator(50, 50);
	//Generator.GetStructureString();
>>>>>>> b21e3c7 (remove uclass marking)
	Super::BeginPlay();
}
void AGameMap_Alpha::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}


