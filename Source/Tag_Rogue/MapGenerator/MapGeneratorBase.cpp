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
	Room,
	Lane
};

UMapGeneratorBase::FCell::FCell(const int32 Y, const int32 X, const int32 TheID, const ECellType Attr)
{
	Py = Y;Px = X;
	Attribution = Attr;
	ID = TheID;
}

UMapGeneratorBase::FCell::FCell(const int32 Y, const int32 X, const int32 TheID) : FCell(Y,X,TheID, ECellType::Wall)
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
