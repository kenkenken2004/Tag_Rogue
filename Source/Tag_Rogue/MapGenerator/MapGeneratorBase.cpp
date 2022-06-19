// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGeneratorBase.h"

enum struct UMapGeneratorBase::ECellType
{
	Wall,
	Path
};

enum struct UMapGeneratorBase::ESpaceType
{
	Plaza,
	Room
};

UMapGeneratorBase::FCell::FCell(const int32 Y, const int32 X, const int32 TheID, const ECellType Attr)
{
	Py = Y;Px = X;
	Attribution = Attr;
	Index = TheID;
}

UMapGeneratorBase::FCell::FCell(const int32 Y, const int32 X, const int32 TheID) : FCell(Y,X,TheID, ECellType::Wall)
{
}

void UMapGeneratorBase::FCell::ChangeAttr(const ECellType Type)
{
	this->Attribution = Type;
}

UMapGeneratorBase::FRect::FRect(const FCell& LeftTop, const FCell& RightBottom)
{
	this->LeftTopCell = LeftTop;
	this->RightBottomCell = RightBottom;
	this->Height = RightBottom.Py - LeftTop.Py + 1;
	this->Width = RightBottom.Px - LeftTop.Px + 1;
}


TArray<UMapGeneratorBase::FCell&> UMapGeneratorBase::FRect::GetAllCells()
{
	
}

UMapGeneratorBase::UMapGeneratorBase(const int32 Map_Height, const int32 Map_Width)
{
	MapHeight = Map_Height;
	MapWidth = Map_Width;
	TArray<int32> Instant = TArray<int32>();
	Instant.Init(0, MapWidth);
	MapMatrix = TArray<TArray<int32>>();
	MapMatrix.Init(Instant, MapWidth);
	CellList = TArray<FCell>();
	for (int i =0;i<Map_Height;i++)
	{
		for (int j=0;j<Map_Width;j++)
		{
			const int32 Num = i*Map_Height+j;
			FCell Cell = FCell(i, j, Num);
			CellList.Add(Cell);
			MapMatrix[i][j] = Num;
		}
	}
}

UMapGeneratorBase::FCell UMapGeneratorBase::GetCell(const int32 PosY, const int32 PosX)
{
	return this->CellList[this->MapMatrix[PosY][PosX]];
	
}
