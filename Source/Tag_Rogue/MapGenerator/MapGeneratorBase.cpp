// Fill out your copyright notice in the Description page of Project Settings.

#include "MapGeneratorBase.h"


UMapGeneratorBase::FCell::FCell(const int32 Y, const int32 X,const EType Attr, UMapGeneratorBase& Generator): Gen(Generator)
{
	Py = Y;Px = X;
	Attribution = Attr;
}

UMapGeneratorBase::FCell::FCell(const int32 Y, const int32 X, UMapGeneratorBase& Generator) : FCell(Y,X,EType::Wall, Generator)
{
}

UMapGeneratorBase::FRect::FRect(FCell& LeftTop,FCell& RightBottom): Gen(LeftTop.Gen)
{
	Height = RightBottom.Py - LeftTop.Py + 1;
	Width = RightBottom.Px - LeftTop.Px + 1;
	LeftTopCell = &LeftTop;
	RightBottomCell = &RightBottom;
}

UMapGeneratorBase::FRect::FRect(FCell& LeftTop, const int32 Height, const int32 Width): FRect(LeftTop, *LeftTop.Gen.GetCell(LeftTop.Py-1+Height, LeftTop.Px-1+Width)) // <- this is the bug point!!! 
{
}


TArray<UMapGeneratorBase::FCell*> UMapGeneratorBase::FRect::GetAllCells() const
{
	TArray<FCell*> Ret = TArray<FCell*>();
	for (int i=this->LeftTopCell->Py;i<=this->RightBottomCell->Py;i++)
	{
		for (int j=this->LeftTopCell->Px;j<=this->RightBottomCell->Px;j++)
		{
			Ret.Add(Gen.GetCell(i,j));
		}
	}
	return Ret;
}

TArray<UMapGeneratorBase::FCell*> UMapGeneratorBase::FRect::GetInnerBorderCells(const EDirection Dir) const
{
	TArray<FCell*> Ret = TArray<FCell*>();
	switch (Dir)
	{
	case EDirection::North:
		for (int i=this->LeftTopCell->Py;i<=this->RightBottomCell->Py;i++)Ret.Add(Gen.GetCell(i, LeftTopCell->Px));
	case EDirection::East:
		for (int i=this->LeftTopCell->Py;i<=this->RightBottomCell->Py;i++)Ret.Add(Gen.GetCell(i, RightBottomCell->Px));
	case EDirection::West:
		for (int j=this->LeftTopCell->Px+1;j<this->RightBottomCell->Px;j++)Ret.Add(Gen.GetCell(LeftTopCell->Py, j));
	case EDirection::South:
		for (int j=this->LeftTopCell->Px+1;j<this->RightBottomCell->Px;j++)Ret.Add(Gen.GetCell(RightBottomCell->Py, j));

	}
	return Ret;
}

TArray<UMapGeneratorBase::FCell*> UMapGeneratorBase::FRect::GetOuterBorderCells(const EDirection Dir) const
{
	TArray<FCell*> Ret = TArray<FCell*>();
	for (int i=this->LeftTopCell->Py-1;i<=this->RightBottomCell->Py+1;i++)
	{
		const int32 t = LeftTopCell->Px - 1;
		const int32 s = RightBottomCell->Px + 1;
		if(Dir == EDirection::North && 0<=i && i<=Gen.MapHeight && 0<=t && t<=Gen.MapWidth)Ret.Add(Gen.GetCell(i, t));
		if(Dir == EDirection::South && 0<=i && i<=Gen.MapHeight && 0<=s && s<=Gen.MapWidth)Ret.Add(Gen.GetCell(i, s));
	}
	for (int j=this->LeftTopCell->Px;j<=this->RightBottomCell->Px;j++)
	{
		const int32 t = LeftTopCell->Py - 1;
		const int32 s = RightBottomCell->Py + 1;
		if(Dir == EDirection::East && 0<=j && j<=Gen.MapWidth && 0<=t && t<=Gen.MapHeight)Ret.Add(Gen.GetCell(s, j));
		if(Dir == EDirection::West && 0<=j && j<=Gen.MapWidth && 0<=s && s<=Gen.MapHeight)Ret.Add(Gen.GetCell(t, j));
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
	if (Index == -1)
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

void UMapGeneratorBase::FSpace::Remove()
{
	if (Gen.SpaceList.Contains(this))
	{
		for (int i=Index+1;i<Gen.SpaceList.Num();i++)Gen.SpaceList[i]->Index -= 1;
		Gen.SpaceList.RemoveAt(Index);
		TArray<FCell*> Elements = GetAllCells();
		for (int j=0;j<Elements.Num();j++)
		{
			Elements[j]->Attribution = EType::Wall;
		}
		Index = -1;
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
	if (Index != -1)
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

void UMapGeneratorBase::FPath::Remove()
{
	if (Gen.PathList.Contains(this))
	{
		for (int i=Index+1;i<Gen.PathList.Num();i++)Gen.PathList[i]->Index -= 1;
		Gen.PathList.RemoveAt(Index);
		TArray<FCell*> Elements = GetAllCells();
		for (int j=0;j<Elements.Num();j++)
		{
			Elements[j]->Attribution = EType::Wall;
		}
		Index = -1;
	}
}

bool UMapGeneratorBase::FArea::CanPlace() const
{
	bool Ret = true;
	TArray<FCell*> Elements = this->GetAllCells();
	for (int i=0;i<Elements.Num();i++)
	{
		Ret &= Elements[i]->AreaIndex == -1;
	}
	return Ret;
}

void UMapGeneratorBase::FArea::Place()
{
	if (Index != -1)
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

void UMapGeneratorBase::FArea::Remove()
{
	if (Gen.AreaList.Contains(this))
	{
		for (int i=Index+1;i<Gen.AreaList.Num();i++)Gen.AreaList[i]->Index -= 1;
		Gen.AreaList.RemoveAt(Index);
		TArray<FCell*> Elements = GetAllCells();
		for (int j=0;j<Elements.Num();j++)
		{
			Elements[j]->AreaIndex = -1;
		}
		Index = -1;
	}
}


 
UMapGeneratorBase::FSpace::FSpace(const FRect Rect, const EType Type): FRect(*Rect.LeftTopCell, *Rect.RightBottomCell)
{
	Attribution = Type;
}

UMapGeneratorBase::FSpace::FSpace(FCell& LeftTop, FCell& RightBottom, const EType Attr): FRect(LeftTop, RightBottom)
{
	Attribution = Attr;
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
	MapMatrix = TArray<TArray<FCell>>();
	for (int i =0;i<MapHeight;i++)
	{
		MapMatrix.Add(TArray<FCell>());
		for (int j=0;j<MapWidth;j++)
		{
			FCell Cell = FCell(i, j,EType::Wall, *this);
			MapMatrix[i].Add(Cell);
		}
	}
}

UMapGeneratorBase::UMapGeneratorBase(): UMapGeneratorBase(1, 1){};

UMapGeneratorBase::FArea::FArea(FCell& LeftTop, FCell& RightBottom, FSpace* OwnerSpace): FRect(LeftTop, RightBottom)
{
	Owner = OwnerSpace;
}

UMapGeneratorBase::FArea::FArea(FCell& LeftTop, FCell& RightBottom): FRect(LeftTop, RightBottom)
{
	Owner = nullptr;
}


void UMapGeneratorBase::FArea::Expand(const EDirection Dir, const int32 Num)
{
	switch (Dir)
	{
	case EDirection::North:
		LeftTopCell = Gen.GetCell(LeftTopCell->Py-Num, LeftTopCell->Px);
		Height += Num;
	case EDirection::East:
		RightBottomCell = Gen.GetCell(RightBottomCell->Py, RightBottomCell->Px+Num);
		Width += Num;
	case EDirection::West:
		LeftTopCell = Gen.GetCell(LeftTopCell->Py, LeftTopCell->Px-Num);
		Width += Num;
	case EDirection::South:
		RightBottomCell = Gen.GetCell(RightBottomCell->Py+Num, RightBottomCell->Px);
		Height += Num;
	}
}


bool UMapGeneratorBase::FArea::Expand()
{
	bool Flag = false;
	if (LeftTopCell->Py != 0)
	{
		if (FArea(*Gen.GetCell(LeftTopCell->Py-1, LeftTopCell->Px),*Gen.GetCell(LeftTopCell->Py-1,RightBottomCell->Px)).CanPlace())
		{
			Expand(EDirection::North, 1);
			Flag = true;
		}
		if (FArea(*Gen.GetCell(LeftTopCell->Py, RightBottomCell->Px+1),*Gen.GetCell(RightBottomCell->Py,RightBottomCell->Px+1)).CanPlace())
		{
			Expand(EDirection::East, 1);
			Flag = true;
		}
		if (FArea(*Gen.GetCell(LeftTopCell->Py, RightBottomCell->Px-1),*Gen.GetCell(RightBottomCell->Py,RightBottomCell->Px-1)).CanPlace())
		{
			Expand(EDirection::West, 1);
			Flag = true;
		}
		if (FArea(*Gen.GetCell(LeftTopCell->Py+1, LeftTopCell->Px),*Gen.GetCell(LeftTopCell->Py+1,RightBottomCell->Px)).CanPlace())
		{
			Expand(EDirection::South, 1);
			Flag = true;
		}
	}
	return Flag;
}


TArray<UMapGeneratorBase::FArea*> UMapGeneratorBase::FArea::Split(const EDirection Dir, int32 Num)
{
	TArray<FArea*> Ret = TArray<FArea*>();
	switch (Dir)
	{
	case EDirection::North:
	case EDirection::South:
		{
			if(Dir == EDirection::South) Num = Height - Num;
			FArea Area1 = FArea(*LeftTopCell, *Gen.GetCell(LeftTopCell->Py+Num-1, RightBottomCell->Px));
			FArea Area2 = FArea(*Gen.GetCell(LeftTopCell->Py+Num, LeftTopCell->Px), *RightBottomCell);
			Remove();
			Area1.Place();
			Area2.Place();
			Ret.Add(&Area1);
			Ret.Add(&Area2);
		}
	case EDirection::East:
	case EDirection::West:
		{
			if(Dir == EDirection::West) Num = Width - Num;
			FArea Area1 = FArea(*Gen.GetCell(LeftTopCell->Py, RightBottomCell->Px-Num+1), *RightBottomCell);
			FArea Area2 = FArea(*LeftTopCell, *Gen.GetCell(RightBottomCell->Py, RightBottomCell->Px-Num));
			Remove();
			Area1.Place();
			Area2.Place();
			Ret.Add(&Area1);
			Ret.Add(&Area2);
		}
	}
	return Ret;
}


UMapGeneratorBase::FCell* UMapGeneratorBase::GetCell(const int32 PosY, const int32 PosX)
{
	int32 Py = 0;
	int32 Px = 0;
	//UE_LOG(LogTemp, Log, TEXT("DATA=%d, %d"), PosY, PosX)
	if (0<=PosY && PosY<MapHeight)Py = PosY;
	if (0<=PosX && PosX<MapWidth)Px = PosX;
	return &MapMatrix[Py][Px];
	
}
