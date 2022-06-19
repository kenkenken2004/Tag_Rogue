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


TArray<UMapGeneratorBase::FCell*> UMapGeneratorBase::FRect::GetAllCells(UMapGeneratorBase &Base) const
{
	TArray<FCell*> Ret = TArray<FCell*>();
	UMapGeneratorBase &Gen = Base;
	for (int i=this->LeftTopCell.Py;i<=this->RightBottomCell.Py;i++)
	{
		for (int j=this->LeftTopCell.Px;j<=this->RightBottomCell.Px;j++)
		{
			Ret.Add(&Gen.CellList[Gen.MapMatrix[i][j]]);
		}
	}
	return Ret;
}

TArray<UMapGeneratorBase::FCell*> UMapGeneratorBase::FRect::GetInnerBorderCells(UMapGeneratorBase &Base) const
{
	TArray<FCell*> Ret = TArray<FCell*>();
	UMapGeneratorBase &Gen = Base;
	for (int i=this->LeftTopCell.Py;i<=this->RightBottomCell.Py;i++)
	{
		Ret.Add(&Gen.CellList[Gen.MapMatrix[i][LeftTopCell.Px]]);
		Ret.Add(&Gen.CellList[Gen.MapMatrix[i][RightBottomCell.Px]]);
	}
	for (int j=this->LeftTopCell.Px+1;j<this->RightBottomCell.Px;j++)
	{
		Ret.Add(&Gen.CellList[Gen.MapMatrix[LeftTopCell.Py][j]]);
		Ret.Add(&Gen.CellList[Gen.MapMatrix[RightBottomCell.Py][j]]);
	}
	return Ret;
}

TArray<UMapGeneratorBase::FCell*> UMapGeneratorBase::FRect::GetOuterBorderCells(UMapGeneratorBase &Base) const
{
	TArray<FCell*> Ret = TArray<FCell*>();
	UMapGeneratorBase &Gen = Base;
	for (int i=this->LeftTopCell.Py-1;i<=this->RightBottomCell.Py+1;i++)
	{
		const int32 t = LeftTopCell.Px - 1;
		const int32 s = RightBottomCell.Px + 1;
		if(0<=i & i<=Gen.MapHeight & 0<=t & t<=Gen.MapWidth)Ret.Add(&Gen.CellList[Gen.MapMatrix[i][t]]);
		if(0<=i & i<=Gen.MapHeight & 0<=s & s<=Gen.MapWidth)Ret.Add(&Gen.CellList[Gen.MapMatrix[i][s]]);
	}
	for (int j=this->LeftTopCell.Px;j<=this->RightBottomCell.Px;j++)
	{
		const int32 t = LeftTopCell.Py - 1;
		const int32 s = RightBottomCell.Py + 1;
		if(0<=j & j<=Gen.MapWidth & 0<=t & t<=Gen.MapHeight)Ret.Add(&Gen.CellList[Gen.MapMatrix[LeftTopCell.Py][j]]);
		if(0<=j & j<=Gen.MapWidth & 0<=s & s<=Gen.MapHeight)Ret.Add(&Gen.CellList[Gen.MapMatrix[RightBottomCell.Py][j]]);
	}
	return Ret;
}
 
UMapGeneratorBase::FSpace::FSpace(const FRect Rect, const ESpaceType Type): FRect(Rect.LeftTopCell, Rect.RightBottomCell)
{
	this->Attribution = Type;
}

void UMapGeneratorBase::FSpace::ChangeAttr(const ESpaceType Type)
{
	this->Attribution = Type;
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
			CellList.Add(FCell(i, j, Num));
			MapMatrix[i][j] = Num;
		}
	}
}



UMapGeneratorBase::FCell UMapGeneratorBase::GetCell(const int32 PosY, const int32 PosX)
{
	return this->CellList[this->MapMatrix[PosY][PosX]];
	
}
