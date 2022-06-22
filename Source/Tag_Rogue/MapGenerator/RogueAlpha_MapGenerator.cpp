// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAlpha_MapGenerator.h"

URogueAlpha_MapGenerator::URogueAlpha_MapGenerator(const int32 MapHeight, const int32 MapWidth): UMapGeneratorBase(MapHeight, MapWidth)
{
}

URogueAlpha_MapGenerator::URogueAlpha_MapGenerator(): URogueAlpha_MapGenerator(1,1){}

bool URogueAlpha_MapGenerator::RandomPlaceSpace(const EType SpaceType)
{
	for(int32 i=0;i<20;i++)
	{
		FCell Rand = GetCell(FMath::RandRange(1, MapHeight-1-StructureSize[SpaceType].Height), FMath::RandRange(1,MapWidth-1)-StructureSize[SpaceType].Width);
		if (FSpace Space = FSpace(FRect(Rand, StructureSize[SpaceType].Height, StructureSize[SpaceType].Width),SpaceType); Space.CanPlace())
		{
			Space.Place();
			return true;
		}
	}
	return false;
}

bool URogueAlpha_MapGenerator::SetDirectPath(FArea* Area1, FArea* Area2)
{
	FSpace* Space1 = Area1->Owner;
	FSpace* Space2 = Area2->Owner;
	return true;
}



TArray<int32> URogueAlpha_MapGenerator::SpaceBuild()
{
	TArray<int32> ActualNums = TArray<int32>();
	int32 Count = 0;
	for (const auto& Structure : StructureNumber)
	{
		ActualNums.Add(0);
		for (int32 i=0;i<StructureNumber[Structure.Key];i++)
		{
			if (RandomPlaceSpace(Structure.Key))ActualNums[Count]++;
		}
		Count++;
	}
	return ActualNums;
}



void URogueAlpha_MapGenerator::AreaBuild()
{
	for (int32 i=0;i<SpaceList.Num();i++)
	{
		FCell& Ins1 = *(SpaceList[i]->LeftTopCell);
		FCell& Ins2 = *(SpaceList[i]->RightBottomCell);
		FArea Ins3 = FArea(Ins1, Ins2);
		AreaList.Add(&Ins3);
	}
	bool BigFlag = true;
	while (BigFlag)
	{
		bool SmallFlag = false;
		for (int32 i=0;i<AreaList.Num();i++)
		{
			SmallFlag |= AreaList[i]->Expand();
		}
		BigFlag &= SmallFlag;
	}
}

void URogueAlpha_MapGenerator::PathBuild()
{
}


void URogueAlpha_MapGenerator::SetStructureParam(const EType Type, const int32 Height, const int32 Width, int32 Num)
{
	const FRect Ins = FRect(GetCell(0,0), GetCell(Height-1, Width-1));
	StructureSize.Add(Type, Ins);
	StructureNumber.Add(Type, Num);
}

