// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAlpha_MapGenerator.h"

URogueAlpha_MapGenerator::URogueAlpha_MapGenerator(const int32 Map_Height, const int32 Map_Width): UMapGeneratorBase(Map_Height, Map_Width)
{
}

URogueAlpha_MapGenerator::URogueAlpha_MapGenerator(): URogueAlpha_MapGenerator(50,50){}

void URogueAlpha_MapGenerator::Construct(const int32 Map_Height, const int32 Map_Width)
{
	Space_Margin = 3;
	Super::Construct(Map_Height, Map_Width);
}

void URogueAlpha_MapGenerator::BuildMap()
{
	BuildSpace();
	BuildArea();
	BuildPath();
}


bool URogueAlpha_MapGenerator::RandomPlaceSpace(const EType SpaceType, const int32 Margin)
{
	for(int32 i=0;i<20;i++)
	{
		
		FCell *Rand = GetCell(FMath::RandRange(1, MapHeight-1-StructureSize[SpaceType].Height), FMath::RandRange(1,MapWidth-1-StructureSize[SpaceType].Width));
		FSpace* Space = new FSpace(Rand, GetCell(Rand->Py+StructureSize[SpaceType].Height-1, Rand->Px+StructureSize[SpaceType].Width-1),SpaceType); //Problem here. Instance are not independent
		FSpace Surround = FSpace(GetCell(FMath::Max(0, Space->LeftTopCell->Py-Margin), FMath::Max(0, Space->LeftTopCell->Px-Margin)), GetCell(FMath::Min(MapHeight-1, Space->RightBottomCell->Py+Margin), FMath::Min(MapWidth-1, Space->RightBottomCell->Px+Margin)), Space->Attribution);
		if (Surround.CanPlace())
		{
			Space->Place();
			return true;
		}
	}
	return false;
}

bool URogueAlpha_MapGenerator::SetPath(const FArea* Area1, const FArea* Area2)
{
	FSpace* Spc1 = Area1->Owner;
	FSpace* Spc2 = Area2->Owner;
	FCell* C1 = Spc1->GetCenterCell();
	FCell* C2 = Spc2->GetCenterCell();
	const FCell* In1 = GetCell(C1->Py, C2->Px);
	const FCell* In2 = GetCell(C2->Py, C1->Px);
	bool Flag = false;
	TArray<FCell*> Cells1 = TArray<FCell*>();
	TArray<FCell*> Cells2 = TArray<FCell*>();
	
	Cells1.Append(FRect(GetCell(C1->Py, C1->Px<In1->Px?C1->Px:In1->Px), GetCell(C1->Py,C1->Px<In1->Px?In1->Px:C1->Px)).GetAllCells());
	const int32 Corner1 = (C1->Px<In1->Px?In1->Px:C1->Px) - (C1->Px<In1->Px?C1->Px:In1->Px);
	if(C1->Px>=In1->Px)Algo::Reverse(Cells1);
	TArray<FCell*> Ins = FRect(GetCell(C2->Py<In1->Py?C2->Py:In1->Py, C2->Px), GetCell(C2->Py<In1->Py?In1->Py:C2->Py,C2->Px)).GetAllCells();
	if(C2->Py<=In1->Py)Algo::Reverse(Ins);
	Cells1.Pop();
	Cells1.Append(Ins);

	Cells2.Append(FRect(GetCell(C1->Py<In2->Py?C1->Py:In2->Py, C1->Px), GetCell(C1->Py<In2->Py?In2->Py:C1->Py,C1->Px)).GetAllCells());
	const int32 Corner2 = (C1->Py<In2->Py?In2->Py:C1->Py) - (C1->Py<In2->Py?C1->Py:In2->Py);
	if(C1->Py>=In2->Py)Algo::Reverse(Cells2);
	Ins = FRect(GetCell(C2->Py, C2->Px<In2->Px?C2->Px:In2->Px), GetCell(C2->Py,C2->Px<In2->Px?In2->Px:C2->Px)).GetAllCells();
	if(C2->Px<=In2->Px)Algo::Reverse(Ins);
	Cells2.Pop();
	Cells2.Append(Ins);
	
	FPath Path1 = FPath(Spc1, Spc2, Cells1, C1, C2, Corner1);
	FPath Path2 = FPath(Spc1, Spc2, Cells2, C1, C2, Corner2);
	if (Path1.CanPlace())
	{
		Path1.Place();
		Flag = true;
	}
	else if (Path2.CanPlace())
	{
		Path2.Place();
		Flag = true;
	}
	return Flag;
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
			if (RandomPlaceSpace(Structure.Key, Space_Margin))ActualNums[Count]++;
		}
		Count++;
	}
	return ActualNums;
}

void URogueAlpha_MapGenerator::BuildArea()
{
	for (int32 i=0;i<SpaceList.Num();i++)
	{
		FCell* Ins1 = SpaceList[i]->LeftTopCell;
		FCell* Ins2 = SpaceList[i]->RightBottomCell;
		FArea* Ins3 = new FArea(Ins1, Ins2, SpaceList[i]);
		if (Ins3->CanPlace())
		{
			Ins3->Place();
		}
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

void URogueAlpha_MapGenerator::BuildPath()
{
	TArray<TArray<bool>> Edges = TArray<TArray<bool>>();
	TArray<bool> Ins = TArray<bool>();
	Ins.Init(false, SpaceList.Num());
	Edges.Init(Ins, SpaceList.Num());
	for (int32 i=0;i<MapHeight;i++)
	{
		for (int32 j=0;j<MapWidth;j++)
		{
			const FCell* Cell1 = GetCell(i, j);
			if(Cell1->AreaIndex==-1)continue;
			if(const FCell* Cell2 = GetCell(i-1, j); Cell2!=nullptr && Cell2->AreaIndex!=-1)
			{
				Edges[Cell1->AreaIndex][Cell2->AreaIndex] = true;
			}
			if(const FCell* Cell2 = GetCell(i, j-1); Cell2!=nullptr && Cell2->AreaIndex!=-1)
			{
				Edges[Cell1->AreaIndex][Cell2->AreaIndex] = true;
			}
		}
	}
	for (int32 i=0;i<Edges.Num();i++)
	{
		for(int32 j=0;j<Edges.Num();j++)
		{
			if(i!=j&&Edges[i][j])
			{
				SetPath(AreaList[i], AreaList[j]);
			}
		}
	}
}

void URogueAlpha_MapGenerator::SetStructureParam(const EType Type, const int32 Height, const int32 Width, const int32 Num) // Both size must be odd numbers.
{
	if (MapHeight <= Height || MapWidth <= Width)return;
	const FRect Ins = FRect(GetCell(0,0), GetCell(Height-1, Width-1));
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
			//Ret[i]+= GetCell(i, j)->Attribution != EType::Path ? static_cast<char>('A' + (GetCell(i, j))->AreaIndex) : '#';
			if(GetCell(i,j)->Attribution==EType::Path)
			{
				if(GetCell(i,j)->Direction == EDirection::North)Ret[i]+=TEXT("↑");
				else if(GetCell(i,j)->Direction == EDirection::East)Ret[i]+=TEXT("→");
				else if(GetCell(i,j)->Direction == EDirection::West)Ret[i]+=TEXT("←");
				else if(GetCell(i,j)->Direction == EDirection::South)Ret[i]+=TEXT("↓");
				else Ret[i]+='.';
			}
			else if(GetCell(i,j)->Attribution==EType::Plaza||GetCell(i,j)->Attribution==EType::Room)Ret[i]+=' ';
			else Ret[i]+='#';
		}
	}
	for (int32 i=0;i<MapHeight;i++)
	{
		UE_LOG(LogTemp, Log,TEXT("%s"), *Ret[i])
	}
	UE_LOG(LogTemp, Log, TEXT("\n"))
	return Ret;
}