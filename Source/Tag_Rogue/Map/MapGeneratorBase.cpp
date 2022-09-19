// Fill out your copyright notice in the Description page of Project Settings.

#include "MapGeneratorBase.h"


UMapGeneratorBase::FCell::FCell(const int32 Y, const int32 X,const EType Attr, UMapGeneratorBase* Generator)
{
	Gen = Generator;
	Py = Y;Px = X;
	Attribution = Attr;
	IsJunction = false;
	IsCorner = false;
	IsGate = false;
	HasObjects = false;
	Direction = EDirection::Null;
}

UMapGeneratorBase::FCell::FCell(const int32 Y, const int32 X, UMapGeneratorBase* Generator) : FCell(Y,X,EType::Wall, Generator)
{
}

UMapGeneratorBase::FRect::FRect(FCell* LeftTop,FCell* RightBottom)
{
	Gen = LeftTop->Gen;
	Height = RightBottom->Py - LeftTop->Py + 1;
	Width = RightBottom->Px - LeftTop->Px + 1;
	LeftTopCell = LeftTop;
	RightBottomCell = RightBottom;
}

UMapGeneratorBase::FRect::FRect(FCell* LeftTop, const int32 Height, const int32 Width): FRect(LeftTop, LeftTop->Gen->GetCell(LeftTop->Py-1+Height, LeftTop->Px-1+Width)) // <- this is the bug point!!! 
{
}


TArray<UMapGeneratorBase::FCell*> UMapGeneratorBase::FRect::GetAllCells() const
{
	TArray<FCell*> Ret = TArray<FCell*>();
	for (int i=this->LeftTopCell->Py;i<=this->RightBottomCell->Py;i++)
	{
		for (int j=this->LeftTopCell->Px;j<=this->RightBottomCell->Px;j++)
		{
			Ret.Add(Gen->GetCell(i,j));
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
		for (int i=this->LeftTopCell->Py;i<=this->RightBottomCell->Py;i++)Ret.Add(Gen->GetCell(i, LeftTopCell->Px));
	case EDirection::East:
		for (int i=this->LeftTopCell->Py;i<=this->RightBottomCell->Py;i++)Ret.Add(Gen->GetCell(i, RightBottomCell->Px));
	case EDirection::West:
		for (int j=this->LeftTopCell->Px+1;j<this->RightBottomCell->Px;j++)Ret.Add(Gen->GetCell(LeftTopCell->Py, j));
	case EDirection::South:
		for (int j=this->LeftTopCell->Px+1;j<this->RightBottomCell->Px;j++)Ret.Add(Gen->GetCell(RightBottomCell->Py, j));
	default:
		break;
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
		if(Dir == EDirection::North && 0<=i && i<=Gen->MapHeight && 0<=t && t<=Gen->MapWidth)Ret.Add(Gen->GetCell(i, t));
		if(Dir == EDirection::South && 0<=i && i<=Gen->MapHeight && 0<=s && s<=Gen->MapWidth)Ret.Add(Gen->GetCell(i, s));
	}
	for (int j=this->LeftTopCell->Px;j<=this->RightBottomCell->Px;j++)
	{
		const int32 t = LeftTopCell->Py - 1;
		const int32 s = RightBottomCell->Py + 1;
		if(Dir == EDirection::East && 0<=j && j<=Gen->MapWidth && 0<=t && t<=Gen->MapHeight)Ret.Add(Gen->GetCell(s, j));
		if(Dir == EDirection::West && 0<=j && j<=Gen->MapWidth && 0<=s && s<=Gen->MapHeight)Ret.Add(Gen->GetCell(t, j));
	}
	return Ret;
}

UMapGeneratorBase::FCell* UMapGeneratorBase::FRect::GetCenterCell() const
{
	const int32 My = (Height%2==1)?(LeftTopCell->Py+RightBottomCell->Py)/2:(LeftTopCell->Py+RightBottomCell->Py-1)/2;
	const int32 Mx = (Height%2==1)?(LeftTopCell->Px+RightBottomCell->Px)/2:(LeftTopCell->Px+RightBottomCell->Px-1)/2;
	return Gen->GetCell(My, Mx);
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
		Index = Gen->SpaceList.Num();
		Gen->SpaceList.Add(this);
		TArray<FCell*> Elements = this->GetAllCells();
		for (int i=0;i<Elements.Num();i++)
		{
			Elements[i]->Attribution = this->Attribution;
		}
	}
}

void UMapGeneratorBase::FSpace::Remove()
{
	if (Gen->SpaceList.Contains(this))
	{
		for (int i=Index+1;i<Gen->SpaceList.Num();i++)Gen->SpaceList[i]->Index -= 1;
		Gen->SpaceList.RemoveAt(Index);
		TArray<FCell*> Elements = GetAllCells();
		for (int j=0;j<Elements.Num();j++)
		{
			Elements[j]->Attribution = EType::Wall;
		}
		Index = -1;
	}
	
}


UMapGeneratorBase::FPath::FPath(FSpace* N1, FSpace* N2, const TArray<FCell*> Elem, FCell* E1, FCell* E2, int32 Corner)
{
	Gen = N1->Gen;
	Index = -1;
	this->Node1 = N1;
	this->Node2 = N2;
	this->Length = Elem.Num();
	this->End1 = E1;
	this->End2 = E2;
	Cells = TArray<FCell*>();
	Cells.Append(Elem);
	CornerIndex = Corner;
}


bool UMapGeneratorBase::FPath::CanPlace() const
{
	bool Ret = true;
	TArray<FCell*> Elements = Cells;
	for (int i=0;i<Elements.Num();i++)
	{
		Ret &= !(Elements[i]->Attribution!=EType::Path&&Elements[i]->Attribution!=EType::Wall&&Elements[i]->AreaIndex!=End1->AreaIndex&&Elements[i]->AreaIndex!=End2->AreaIndex);
	}
	return Ret;
}

void UMapGeneratorBase::FPath::Place()
{
	if (Index == -1)
	{
		EDirection CurD = EDirection::Null; //現在の道の方向を宣言
		if(Cells[0]->Px==Cells[1]->Px)CurD=EDirection::North; //道の方向を初期化
		else if(Cells[0]->Py==Cells[1]->Py)CurD=EDirection::East;
		if(CornerIndex==0)
		{
			if(CurD==EDirection::North)CurD=EDirection::East;
			else if(CurD==EDirection::East)CurD=EDirection::North;
		}
		Index = Gen->PathList.Num();
		Gen->PathList.Add(this);
		for (int i=0;i<Cells.Num();i++)
		{
			bool bDoesPlace = true;
			if(i==CornerIndex) //現在のセルが角に該当するとき（既に他の道がおかれているかを問わない）
			{
				if(CurD==EDirection::North)CurD=EDirection::East; //道の方向を転換
				else if(CurD==EDirection::East)CurD=EDirection::North;
			}
			if (Cells[i]->Attribution == EType::Path) //現在のセルが既にほかの道を構成するとき
			{
				if(Cells[i]->IsCorner) //現在のセルが他の道の角であるとき⇒そこが交差点であることの十分条件
				{
					Cells[i]->IsJunction = true; //交差点に設定（T字か十字かは問わず）
				}
				else //現在のセルが他の道の直進部分であるとき
				{
					if(i==CornerIndex) //現在のセルがこの道の角であるとき⇒交差点であることの十分条件
					{
						Cells[i]->IsJunction = true; //交差点に設定（T字か十字かは問わず）
					}
					else //そうでないとき
					{
						if(CurD==Cells[i]->Direction) //この道での方向と他の道での方向が一致するとき⇒この道から削除可能
						{
							Length--;
							Cells.RemoveAt(i);
							i--;
							if(i<CornerIndex)CornerIndex--;
							End1 = Cells[0];
							End2 = Cells[Cells.Num()-1];
							bDoesPlace = false;
						}
						else //この道での直進方向と他の道での直進方向が違うとき⇒交差点であることの十分条件
						{
							Cells[i]->IsJunction = true;
						}
					}
					
				}
			}
			else if (Cells[i]->Attribution == EType::Wall) //現在のセルに道が置かれておらず部屋でもないとき
			{
				Cells[i]->Direction = CurD;
				if(i==CornerIndex)Cells[i]->IsCorner = true;

			}
			else //部屋のとき
			{
				int32 Count = 0;
				if(Cells[i]->North()->Attribution==EType::Path)Count++;
				if(Cells[i]->East() ->Attribution==EType::Path)Count++;
				if(Cells[i]->West() ->Attribution==EType::Path)Count++;
				if(Cells[i]->South()->Attribution==EType::Path)Count++;
				if(i==0&&Cells[i+1]->Attribution==EType::Wall)Count++;
				if(Count!=0) //部屋と通路の接続部分(部屋側)であるとき
				{
					Cells[i]->IsGate = true;
					Cells[i]->Direction = CurD;
				}
				Length--;
				Cells.RemoveAt(i);
				i--;
				if(i<CornerIndex)CornerIndex--;
				End1 = Cells[0];
				End2 = Cells[Cells.Num()-1];
				bDoesPlace = false;
			}
			if(bDoesPlace)Cells[i]->Attribution = EType::Path;
		}
	}
}

void UMapGeneratorBase::FPath::Remove()
{
	if (Gen->PathList.Contains(this))
	{
		for (int i=Index+1;i<Gen->PathList.Num();i++)Gen->PathList[i]->Index--;
		Gen->PathList.RemoveAt(Index);
		TArray<FCell*> Elements = Cells;
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
	if (Index == -1)
	{
		Index = Gen->AreaList.Num();
		Gen->AreaList.Add(this);
		TArray<FCell*> Elements = this->GetAllCells();
		for (int i=0;i<Elements.Num();i++)
		{
			Elements[i]->AreaIndex = this->Index;
		}
	}
}

void UMapGeneratorBase::FArea::Remove()
{
	if (Gen->AreaList.Contains(this))
	{
		for (int i=Index+1;i<Gen->AreaList.Num();i++)
		{
			Gen->AreaList[i]->Index--;
			for(FCell *Cell : Gen->AreaList[i]->GetAllCells())Cell->AreaIndex--;
		}
		Gen->AreaList.RemoveAt(Index);
		TArray<FCell*> Elements = GetAllCells();
		for (int j=0;j<Elements.Num();j++)
		{
			Elements[j]->AreaIndex = -1;
		}
		Index = -1;
	}
}

UMapGeneratorBase::FSpace::FSpace(FCell* LeftTop, FCell* RightBottom, const EType Attr): FRect(LeftTop, RightBottom)
{
	Attribution = Attr;
}


void UMapGeneratorBase::FSpace::ChangeAttr(const EType Type)
{
	Attribution = Type;
}

void UMapGeneratorBase::Construct(const int32 Map_Height, const int32 Map_Width)
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
			MapMatrix[i].Add(FCell(i, j,EType::Wall, this));
		}
	}
}

UMapGeneratorBase::UMapGeneratorBase(const int32 Map_Height, const int32 Map_Width)
{
	UMapGeneratorBase::Construct(Map_Height, Map_Width);
}

UMapGeneratorBase::UMapGeneratorBase(): UMapGeneratorBase(1, 1){};

UMapGeneratorBase::FArea::FArea(FCell* LeftTop, FCell* RightBottom, FSpace* OwnerSpace): FRect(LeftTop, RightBottom)
{
	Owner = OwnerSpace;
}

UMapGeneratorBase::FArea::FArea(FCell* LeftTop, FCell* RightBottom): FRect(LeftTop, RightBottom)
{
	Owner = nullptr;
}


void UMapGeneratorBase::FArea::Expand(const EDirection Dir, const int32 Num)
{
	switch (Dir)
	{
	case EDirection::North:
		LeftTopCell = Gen->GetCell(LeftTopCell->Py-Num, LeftTopCell->Px);
		Height += Num;
		break;
	case EDirection::East:
		RightBottomCell = Gen->GetCell(RightBottomCell->Py, RightBottomCell->Px+Num);
		Width += Num;
		break;
	case EDirection::West:
		LeftTopCell = Gen->GetCell(LeftTopCell->Py, LeftTopCell->Px-Num);
		Width += Num;
		break;
	case EDirection::South:
		RightBottomCell = Gen->GetCell(RightBottomCell->Py+Num, RightBottomCell->Px);
		Height += Num;
		break;
	default:
		break;
	}
	TArray<FCell*> Elements = GetAllCells();
	for (int32 i=0;i<Elements.Num();i++)
	{
		Elements[i]->AreaIndex = Index;
	}
}


bool UMapGeneratorBase::FArea::Expand()
{
	bool Flag = false;
	if ((LeftTopCell->Py>0) && FArea(Gen->GetCell(LeftTopCell->Py-1, LeftTopCell->Px),Gen->GetCell(LeftTopCell->Py-1,RightBottomCell->Px)).CanPlace())
	{
		Expand(EDirection::North, 1);
		Flag = true;
	}
	if ((RightBottomCell->Px<Gen->MapWidth-1) && FArea(Gen->GetCell(LeftTopCell->Py, RightBottomCell->Px+1),Gen->GetCell(RightBottomCell->Py,RightBottomCell->Px+1)).CanPlace())
	{Expand(EDirection::East, 1);
		Flag = true;
	}
	if ((LeftTopCell->Px>0) && FArea(Gen->GetCell(LeftTopCell->Py, LeftTopCell->Px-1),Gen->GetCell(RightBottomCell->Py,LeftTopCell->Px-1)).CanPlace())
	{
		Expand(EDirection::West, 1);
		Flag = true;
	}
	if ((RightBottomCell->Py<Gen->MapHeight-1) && FArea(Gen->GetCell(RightBottomCell->Py+1, LeftTopCell->Px),Gen->GetCell(RightBottomCell->Py+1,RightBottomCell->Px)).CanPlace())
	{
		Expand(EDirection::South, 1);
		Flag = true;
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
			FArea* Area1 = new FArea(LeftTopCell, Gen->GetCell(LeftTopCell->Py+Num-1, RightBottomCell->Px));
			FArea* Area2 = new FArea(Gen->GetCell(LeftTopCell->Py+Num, LeftTopCell->Px), RightBottomCell);
			Remove();
			Area1->Place();
			Area2->Place();
			Ret.Add(Area1);
			Ret.Add(Area2);
		}
		break;
	case EDirection::East:
	case EDirection::West:
		{
			if(Dir == EDirection::West) Num = Width - Num;
			FArea* Area1 = new FArea(Gen->GetCell(LeftTopCell->Py, RightBottomCell->Px-Num+1), RightBottomCell);
			FArea* Area2 = new FArea(LeftTopCell, Gen->GetCell(RightBottomCell->Py, RightBottomCell->Px-Num));
			Remove();
			Area1->Place();
			Area2->Place();
			Ret.Add(Area1);
			Ret.Add(Area2);
		}
		break;
	default:
		break;
	}
	return Ret;
}


UMapGeneratorBase::FCell* UMapGeneratorBase::GetCell(const int32 PosY, const int32 PosX)
{
	//UE_LOG(LogTemp, Log, TEXT("DATA=%d, %d"), PosY, PosX)
	if (0<=PosY && PosY<MapHeight && 0<=PosX && PosX<MapWidth)
	{
		return &MapMatrix[PosY][PosX];
	}
		return nullptr;
	
}
