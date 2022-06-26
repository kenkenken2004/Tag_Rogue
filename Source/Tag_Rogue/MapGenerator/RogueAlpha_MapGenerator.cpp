// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAlpha_MapGenerator.h"

URogueAlpha_MapGenerator::URogueAlpha_MapGenerator(const int32 MapHeight, const int32 MapWidth): UMapGeneratorBase(MapHeight, MapWidth)
{
	SetStructureParam(EType::Plaza, 7, 7, 2);
	SetStructureParam(EType::Room, 3, 3, 9);
	BuildSpace();
	BuildArea();
	GetAreaString();
}

URogueAlpha_MapGenerator::URogueAlpha_MapGenerator(): URogueAlpha_MapGenerator(50,50){}

bool URogueAlpha_MapGenerator::RandomPlaceSpace(const EType SpaceType)
{
	for(int32 i=0;i<20;i++)
	{
		
		FCell *Rand = GetCell(FMath::RandRange(1, MapHeight-1-StructureSize[SpaceType].Height), FMath::RandRange(1,MapWidth-1-StructureSize[SpaceType].Width));
		FSpace* Space = new FSpace(*Rand, *GetCell(Rand->Py+StructureSize[SpaceType].Height-1, Rand->Px+StructureSize[SpaceType].Width-1),SpaceType); //Problem here. Instancce are not independent
		if (Space->CanPlace())
		{
			Space->Place();
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



TArray<int32> URogueAlpha_MapGenerator::BuildSpace()
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



void URogueAlpha_MapGenerator::BuildArea()
{
	for (int32 i=0;i<SpaceList.Num();i++)
	{
		FCell& Ins1 = *(SpaceList[i]->LeftTopCell);
		FCell& Ins2 = *(SpaceList[i]->RightBottomCell);
		FArea* Ins3 = new FArea(Ins1, Ins2, SpaceList[i]);
		if (Ins3->CanPlace())
		{
			Ins3->Place();
		}
	}
	bool BigFlag = true;
	while (BigFlag) // infinite loop
	{
		bool SmallFlag = false;
		for (int32 i=0;i<AreaList.Num();i++)
		{
			SmallFlag |= AreaList[i]->Expand();
		}
		BigFlag &= SmallFlag;
	}
}

void URogueAlpha_MapGenerator::BuildPath()
{
	
}


void URogueAlpha_MapGenerator::SetStructureParam(const EType Type, const int32 Height, const int32 Width, const int32 Num)
{
	if (MapHeight <= Height || MapWidth <= Width)return;
	const FRect Ins = FRect(*GetCell(0,0), *GetCell(Height-1, Width-1));
	StructureSize.Add(Type, Ins);
	StructureNumber.Add(Type, Num);
}

TArray<FString> URogueAlpha_MapGenerator::GetStructureString()
{
	TArray<FString> Ret = TArray<FString>();
	Ret.Init(FString(), MapHeight);
	for (int i=0;i<MapHeight;i++)
	{
		for (int j=0;j<MapWidth;j++)
		{
			Ret[i]+= (*GetCell(i,j)).Attribution == EType::Wall ? '#' : ' ';
		}
	}
	return Ret;
}

TArray<FString> URogueAlpha_MapGenerator::GetAreaString()
{
	TArray<FString> Ret = TArray<FString>();
	Ret.Init(FString(), MapHeight);
	for (int i=0;i<MapHeight;i++)
	{
		for (int j=0;j<MapWidth;j++)
		{
			Ret[i]+= GetCell(i, j)->Attribution == EType::Wall ? static_cast<char>('A' + (GetCell(i, j))->AreaIndex) : ' ';
		}
	}
	for (int32 i=0;i<MapHeight;i++)
	{
		UE_LOG(LogTemp, Log,TEXT("%s"), *Ret[i])
	}
	return Ret;
}