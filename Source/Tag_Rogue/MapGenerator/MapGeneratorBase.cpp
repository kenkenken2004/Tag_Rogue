// Fill out your copyright notice in the Description page of Project Settings.

#include "MapGeneratorBase.h"


enum struct UMapGeneratorBase::EType
{
	Wall,
	Plaza,
	Room,
	Path
};


enum struct UMapGeneratorBase::EDirection
{
	North,
	East,
	West,
	South
};

UMapGeneratorBase::FCell::FCell(const int32 Y, const int32 X, const int32 TheID, const EType Attr, UMapGeneratorBase& Generator): Gen(Generator)
{
	Py = Y;Px = X;
	Attribution = Attr;
	Index = TheID;
}

UMapGeneratorBase::FCell::FCell(const int32 Y, const int32 X, const int32 TheID, UMapGeneratorBase& Generator) : FCell(Y,X,TheID, EType::Wall, Generator)
{
}

void UMapGeneratorBase::FCell::ChangeAttr(const EType Type)
{
	Attribution = Type;
}

UMapGeneratorBase::FRect::FRect(FCell& LeftTop,FCell& RightBottom): Gen(LeftTop.Gen)
{
	Height = RightBottom.Py - LeftTop.Py + 1;
	Width = RightBottom.Px - LeftTop.Px + 1;
	LeftTopCell = &LeftTop;
	RightBottomCell = &RightBottom;
}


TArray<UMapGeneratorBase::FCell*> UMapGeneratorBase::FRect::GetAllCells() const
{
	TArray<FCell*> Ret = TArray<FCell*>();
	for (int i=this->LeftTopCell->Py;i<=this->RightBottomCell->Py;i++)
	{
		for (int j=this->LeftTopCell->Px;j<=this->RightBottomCell->Px;j++)
		{
			Ret.Add(&Gen.CellList[Gen.MapMatrix[i][j]]);
		}
	}
	return Ret;
}

TArray<UMapGeneratorBase::FCell*> UMapGeneratorBase::FRect::GetInnerBorderCells() const
{
	TArray<FCell*> Ret = TArray<FCell*>();
	for (int i=this->LeftTopCell->Py;i<=this->RightBottomCell->Py;i++)
	{
		Ret.Add(&Gen.CellList[Gen.MapMatrix[i][LeftTopCell->Px]]);
		Ret.Add(&Gen.CellList[Gen.MapMatrix[i][RightBottomCell->Px]]);
	}
	for (int j=this->LeftTopCell->Px+1;j<this->RightBottomCell->Px;j++)
	{
		Ret.Add(&Gen.CellList[Gen.MapMatrix[LeftTopCell->Py][j]]);
		Ret.Add(&Gen.CellList[Gen.MapMatrix[RightBottomCell->Py][j]]);
	}
	return Ret;
}

TArray<UMapGeneratorBase::FCell*> UMapGeneratorBase::FRect::GetOuterBorderCells() const
{
	TArray<FCell*> Ret = TArray<FCell*>();
	for (int i=this->LeftTopCell->Py-1;i<=this->RightBottomCell->Py+1;i++)
	{
		const int32 t = LeftTopCell->Px - 1;
		const int32 s = RightBottomCell->Px + 1;
		if(0<=i & i<=Gen.MapHeight & 0<=t & t<=Gen.MapWidth)Ret.Add(&Gen.CellList[Gen.MapMatrix[i][t]]);
		if(0<=i & i<=Gen.MapHeight & 0<=s & s<=Gen.MapWidth)Ret.Add(&Gen.CellList[Gen.MapMatrix[i][s]]);
	}
	for (int j=this->LeftTopCell->Px;j<=this->RightBottomCell->Px;j++)
	{
		const int32 t = LeftTopCell->Py - 1;
		const int32 s = RightBottomCell->Py + 1;
		if(0<=j & j<=Gen.MapWidth & 0<=t & t<=Gen.MapHeight)Ret.Add(&Gen.CellList[Gen.MapMatrix[LeftTopCell->Py][j]]);
		if(0<=j & j<=Gen.MapWidth & 0<=s & s<=Gen.MapHeight)Ret.Add(&Gen.CellList[Gen.MapMatrix[RightBottomCell->Py][j]]);
	}
	return Ret;
}

bool UMapGeneratorBase::FSpace::CanPlace() const
{
	bool Ret = true;
	TArray<FCell*> Elements = this->GetAllCells();
	for (int i=0;i<Elements.Num();i++)
	{
		Ret &= Elements[i]->Attribution == EType::Wall;
	}
	return Ret;
}

void UMapGeneratorBase::FSpace::Place()
{
	if (!Gen.SpaceList.Contains(*this))
	{
		Index = Gen.SpaceList.Num();
		Gen.SpaceList.Add(this);
		TArray<FCell*> Elements = this->GetAllCells();
		for (int i=0;i<Elements.Num();i++)
		{
			Elements[i]->Attribution = this->Attribution;
		}
	}
}

UMapGeneratorBase::FPath::FPath(FRect* N1, FRect* N2, const FRect* Body): FRect(*Body)
{
	this->Node1 = N1;
	this->Node2 = N2;
	this->Length = FMath::Max(Body->Height, Body->Width);
	this->Direction = Body->Height>Body->Width ? EDirection::North : EDirection::West;
}


bool UMapGeneratorBase::FPath::CanPlace() const
{
	bool Ret = true;
	TArray<FCell*> Elements = this->GetAllCells();
	for (int i=0;i<Elements.Num();i++)
	{
		Ret &= Elements[i]->Attribution == EType::Wall;
	}
	return Ret;
}

void UMapGeneratorBase::FPath::Place()
{
	if (!Gen.PathList.Contains(*this))
	{
		Index = Gen.PathList.Num();
		Gen.PathList.Add(this);
		TArray<FCell*> Elements = this->GetAllCells();
		for (int i=0;i<Elements.Num();i++)
		{
			Elements[i]->Attribution = EType::Path;
		}
	}
}

bool UMapGeneratorBase::FArea::CanPlace() const
{
	bool Ret = true;
	TArray<FCell*> Elements = this->GetAllCells();
	for (int i=0;i<Elements.Num();i++)
	{
		Ret &= Elements[i]->Attribution == EType::Wall;
	}
	return Ret;
}

void UMapGeneratorBase::FArea::Place()
{
	if (!Gen.AreaList.Contains(*this))
	{
		Index = Gen.AreaList.Num();
		Gen.AreaList.Add(this);
		TArray<FCell*> Elements = this->GetAllCells();
		for (int i=0;i<Elements.Num();i++)
		{
			Elements[i]->AreaIndex = this->Index;
		}
	}
}


 
UMapGeneratorBase::FSpace::FSpace(const FRect Rect, const EType Type): FRect(*Rect.LeftTopCell, *Rect.RightBottomCell)
{
	Attribution = Type;
}

void UMapGeneratorBase::FSpace::ChangeAttr(const EType Type)
{
	Attribution = Type;
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
			CellList.Add(FCell(i, j, Num, *this));
			MapMatrix[i][j] = Num;
		}
	}
}

UMapGeneratorBase::FArea::FArea(const FRect Rect): FRect(*Rect.LeftTopCell, *Rect.RightBottomCell)
{
}


void UMapGeneratorBase::FArea::Expand(const EDirection Dir, const int32 Num)
{
	switch (Dir)
	{
	case EDirection::North:
		LeftTopCell = &Gen.GetCell(LeftTopCell->Py-Num, LeftTopCell->Px);
		Height += Num;
	case EDirection::East:
		RightBottomCell = &Gen.GetCell(RightBottomCell->Py, RightBottomCell->Px+Num);
		Width += Num;
	case EDirection::West:
		LeftTopCell = &Gen.GetCell(LeftTopCell->Py, LeftTopCell->Px-Num);
		Width += Num;
	case EDirection::South:
		RightBottomCell = &Gen.GetCell(RightBottomCell->Py+Num, RightBottomCell->Px);
		Height += Num;
	}
}

void UMapGeneratorBase::FArea::Expand(const EDirection Dir)
{
	Expand(Dir, 1);
}

TArray<UMapGeneratorBase::FArea> UMapGeneratorBase::FArea::Split(EDirection, int32)
{
	
}


UMapGeneratorBase::FCell& UMapGeneratorBase::GetCell(const int32 PosY, const int32 PosX)
{
	return this->CellList[this->MapMatrix[PosY][PosX]];
	
}
