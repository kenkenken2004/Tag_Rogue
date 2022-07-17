// Fill out your copyright notice in the Description page of Project Settings.


#include "TerrainMaker.h"
#include "Tag_Rogue/MapObject/HoloGlobe.h"
#include "Tag_Rogue/MapObject/MapGate.h"
#include "Tag_Rogue/MapObject/MapUnitBase.h"

UTerrainMaker::UTerrainMaker()
{
}

void UTerrainMaker::Construct(URogueAlpha_MapGenerator* Gen, const float MapCellSize)
{
	Generator = Gen;
	CellSize = MapCellSize;
}

void UTerrainMaker::Build()
{
	for(int32 y=0;y<Generator->MapHeight;y++)
	{
		for(int32 x=0;x<Generator->MapWidth;x++)
		{
			PlaceMapUnitBase(y,x);
		}
	}
}

void UTerrainMaker::AddGate(const URogueAlpha_MapGenerator::FCell* Cell,
							const URogueAlpha_MapGenerator::EDirection Dir) const
{
	FVector Location = Cie_Convert(Cell->Py, Cell->Px, CellSize);
	FVector OffSet = FVector(0, 0, 0);
	FRotator Rotator = FRotator(0, 0, 0);
	switch (Dir)
	{
	case UMapGeneratorBase::EDirection::North:
		OffSet = FVector(0, -CellSize / 2, 0);
		Rotator = FRotator(0, 90, 0);
		break;
	case UMapGeneratorBase::EDirection::East:
		OffSet = FVector(CellSize / 2, 0, 0);
		break;
	case UMapGeneratorBase::EDirection::West:
		OffSet = FVector(-CellSize / 2, 0, 0);
		break;
	case UMapGeneratorBase::EDirection::South:
		OffSet = FVector(0, CellSize / 2, 0);
		Rotator = FRotator(0, 90, 0);
		break;
	default:
		break;
	}
	Location += OffSet;
	AMapGate* MapGate = GetWorld()->SpawnActor<AMapGate>(Location, Rotator);
	MapGate->SetActorScale3D(FVector(CellSize / 100, CellSize / 100, CellSize / 100));
}

void UTerrainMaker::SpawnGlobe()
{
	for (int32 i = 0; i < Generator->SpaceList.Num(); i++)
	{
		const UMapGeneratorBase::FCell* Cell = Generator->SpaceList[i]->GetCenterCell();
		GetWorld()->SpawnActor<AHoloGlobe>(Cie_Convert(Cell->Py, Cell->Px, 0), FRotator(0, 0, 0));
	}
}

void UTerrainMaker::PlaceMapUnitBase(int32 Py, int32 Px) const
{
	AMapUnitBase::EMeshType MeshType = AMapUnitBase::EMeshType::Null;
	FRotator Rotator = FRotator(0, 0, 0);
	bool bIsXReversed = false;
	const UMapGeneratorBase::FCell* Cell = Generator->GetCell(Py, Px);
	if (Cell->Attribution == UMapGeneratorBase::EType::Plaza || Cell->Attribution == UMapGeneratorBase::EType::Room)
	//部屋のとき
	{
		if (Cell->IsGate) //接続口のとき
		{
			int32 Count1 = 0;
			if (Cell->North()->Attribution != Cell->Attribution)
			{
				Count1++;
			}
			if (Cell->East()->Attribution != Cell->Attribution)
			{
				Count1++;
			}
			if (Cell->West()->Attribution != Cell->Attribution)
			{
				Count1++;
			}
			if (Cell->South()->Attribution != Cell->Attribution)
			{
				Count1++;
			}
			int32 Count2 = 0;
			if (Cell->North()->Attribution == UMapGeneratorBase::EType::Path && (Cell->North()->Direction ==
				UMapGeneratorBase::EDirection::North && !Cell->North()->IsCorner) || Cell->North()->IsJunction)
			{
				Count2++;
			}
			if (Cell->East()->Attribution == UMapGeneratorBase::EType::Path && (Cell->East()->Direction ==
				UMapGeneratorBase::EDirection::East && !Cell->East()->IsCorner) || Cell->East()->IsJunction)
			{
				Count2++;
			}
			if (Cell->West()->Attribution == UMapGeneratorBase::EType::Path && (Cell->West()->Direction ==
				UMapGeneratorBase::EDirection::East && !Cell->West()->IsCorner) || Cell->West()->IsJunction)
			{
				Count2++;
			}
			if (Cell->South()->Attribution == UMapGeneratorBase::EType::Path && (Cell->South()->Direction ==
				UMapGeneratorBase::EDirection::North && !Cell->South()->IsCorner) || Cell->South()->IsJunction)
			{
				Count2++;
			}
			if (Count1 == 1)
			{
				MeshType = AMapUnitBase::EMeshType::GateNormal;
				if (Cell->North()->Attribution != Cell->Attribution)
				{
					Rotator = FRotator(0, 180, 0);
					AddGate(Cell, UMapGeneratorBase::EDirection::North);
				}
				if (Cell->East()->Attribution != Cell->Attribution)
				{
					Rotator = FRotator(0, 270, 0);
					AddGate(Cell, UMapGeneratorBase::EDirection::East);
				}
				if (Cell->West()->Attribution != Cell->Attribution)
				{
					Rotator = FRotator(0, 90, 0);
					AddGate(Cell, UMapGeneratorBase::EDirection::West);
				}
				if (Cell->South()->Attribution != Cell->Attribution)
				{
					Rotator = FRotator(0, 0, 0);
					AddGate(Cell, UMapGeneratorBase::EDirection::South);
				}
			}
			else if (Count1 == 2 && Count2 == 1)
			{
				MeshType = AMapUnitBase::EMeshType::GateCornerOne;
				if (Cell->North()->Attribution != Cell->Attribution && Cell->East()->Attribution != Cell->Attribution)
				{
					Rotator = FRotator(0, 180, 0);
					if (Cell->East()->Attribution == UMapGeneratorBase::EType::Path)
					{
						bIsXReversed = true;
						Rotator.Yaw += 90;
						AddGate(Cell, UMapGeneratorBase::EDirection::East);
					}
				}
				else if (Cell->East()->Attribution != Cell->Attribution && Cell->South()->Attribution != Cell->
					Attribution)
				{
					Rotator = FRotator(0, 270, 0);
					if (Cell->South()->Attribution == UMapGeneratorBase::EType::Path)
					{
						bIsXReversed = true;
						Rotator.Yaw += 90;
						AddGate(Cell, UMapGeneratorBase::EDirection::South);
					}
				}
				else if (Cell->South()->Attribution != Cell->Attribution && Cell->West()->Attribution != Cell->
					Attribution)
				{
					Rotator = FRotator(0, 0, 0);
					if (Cell->West()->Attribution == UMapGeneratorBase::EType::Path)
					{
						bIsXReversed = true;
						Rotator.Yaw += 90;
						AddGate(Cell, UMapGeneratorBase::EDirection::West);
					}
				}
				else if (Cell->West()->Attribution != Cell->Attribution && Cell->North()->Attribution != Cell->
					Attribution)
				{
					Rotator = FRotator(0, 90, 0);
					if (Cell->North()->Attribution == UMapGeneratorBase::EType::Path)
					{
						bIsXReversed = true;
						Rotator.Yaw += 90;
						AddGate(Cell, UMapGeneratorBase::EDirection::North);
					}
				}
			}
			else if (Count1 == 2 && Count2 == 2)
			{
				MeshType = AMapUnitBase::EMeshType::GateCornerTwo;
				if (Cell->North()->Attribution != Cell->Attribution && Cell->East()->Attribution != Cell->Attribution)
				{
					Rotator = FRotator(0, 270, 0);
					AddGate(Cell, UMapGeneratorBase::EDirection::North);
					AddGate(Cell, UMapGeneratorBase::EDirection::East);
				}
				else if (Cell->East()->Attribution != Cell->Attribution && Cell->South()->Attribution != Cell->
					Attribution)
				{
					Rotator = FRotator(0, 0, 0);
					AddGate(Cell, UMapGeneratorBase::EDirection::East);
					AddGate(Cell, UMapGeneratorBase::EDirection::South);
				}
				else if (Cell->South()->Attribution != Cell->Attribution && Cell->West()->Attribution != Cell->
					Attribution)
				{
					Rotator = FRotator(0, 90, 0);
					AddGate(Cell, UMapGeneratorBase::EDirection::South);
					AddGate(Cell, UMapGeneratorBase::EDirection::West);
				}
				else if (Cell->West()->Attribution != Cell->Attribution && Cell->North()->Attribution != Cell->
					Attribution)
				{
					Rotator = FRotator(0, 180, 0);
					AddGate(Cell, UMapGeneratorBase::EDirection::West);
					AddGate(Cell, UMapGeneratorBase::EDirection::North);
				}
			}
		}
		else //そうでないとき
		{
			int32 Count = 0;
			if (Cell->North()->Attribution != Cell->Attribution)
			{
				Count++;
			}
			if (Cell->East()->Attribution != Cell->Attribution)
			{
				Count++;
			}
			if (Cell->West()->Attribution != Cell->Attribution)
			{
				Count++;
			}
			if (Cell->South()->Attribution != Cell->Attribution)
			{
				Count++;
			}
			switch (Count)
			{
			case 0:
				MeshType = AMapUnitBase::EMeshType::RoomNormal;
				break;
			case 1:
				MeshType = AMapUnitBase::EMeshType::RoomSide;
				if (Cell->North()->Attribution != Cell->Attribution)
				{
					Rotator = FRotator(0, 90, 0);
				}
				if (Cell->East()->Attribution != Cell->Attribution)
				{
					Rotator = FRotator(0, 180, 0);
				}
				if (Cell->West()->Attribution != Cell->Attribution)
				{
					Rotator = FRotator(0, 0, 0);
				}
				if (Cell->South()->Attribution != Cell->Attribution)
				{
					Rotator = FRotator(0, 270, 0);
				}
				break;
			case 2:
				MeshType = AMapUnitBase::EMeshType::RoomCorner;
				if (Cell->North()->Attribution != Cell->Attribution && Cell->East()->Attribution != Cell->Attribution)
				{
					Rotator = FRotator(0, 90, 0);
				}
				else if (Cell->East()->Attribution != Cell->Attribution && Cell->South()->Attribution != Cell->
					Attribution)
				{
					Rotator = FRotator(0, 180, 0);
				}
				else if (Cell->South()->Attribution != Cell->Attribution && Cell->West()->Attribution != Cell->
					Attribution)
				{
					Rotator = FRotator(0, 270, 0);
				}
				else if (Cell->West()->Attribution != Cell->Attribution && Cell->North()->Attribution != Cell->
					Attribution)
				{
					Rotator = FRotator(0, 0, 0);
				}
				break;
			default:
				MeshType = AMapUnitBase::EMeshType::Null;
				break;
			}
		}
	}
	else if (Cell->Attribution == UMapGeneratorBase::EType::Path) //通路のとき
	{
		if (Cell->IsJunction) //交差点のとき
		{
			MeshType = AMapUnitBase::EMeshType::PathTJunction;
			if (!Cell->North()->IsGate && Cell->North()->Direction != UMapGeneratorBase::EDirection::North && !Cell->
				North()->IsJunction && !(Cell->North()->IsCorner && (Cell->North()->North()->Direction != Cell->
					Direction || !Cell->North()->North()->IsJunction)))
			{
				Rotator = FRotator(0, 90, 0);
			}
			else if (!Cell->East()->IsGate && Cell->East()->Direction != UMapGeneratorBase::EDirection::East && !Cell->
				East()->IsJunction && !(Cell->East()->IsCorner && (Cell->East()->East()->Direction != Cell->Direction ||
					!Cell->East()->East()->IsJunction)))
			{
				Rotator = FRotator(0, 180, 0);
			}
			else if (!Cell->West()->IsGate && Cell->West()->Direction != UMapGeneratorBase::EDirection::East && !Cell->
				West()->IsJunction && !(Cell->West()->IsCorner && (Cell->West()->West()->Direction != Cell->Direction ||
					!Cell->West()->West()->IsJunction)))
			{
				Rotator = FRotator(0, 0, 0);
			}
			else if (!Cell->South()->IsGate && Cell->South()->Direction != UMapGeneratorBase::EDirection::North && !Cell
				->South()->IsJunction && !(Cell->South()->IsCorner && (Cell->South()->South()->Direction != Cell->
					Direction || !Cell->South()->South()->IsJunction)))
			{
				Rotator = FRotator(0, 270, 0);
			}
			else
			{
				MeshType = AMapUnitBase::EMeshType::PathCrossRoad;
			}
		}
		else //交差点でないとき
		{
			if (Cell->IsCorner) //角のとき
			{
				MeshType = AMapUnitBase::EMeshType::PathCorner;
				bool bNorth = Cell->North()->Direction == UMapGeneratorBase::EDirection::North;
				bool bEast = Cell->East()->Direction == UMapGeneratorBase::EDirection::East;
				bool bWest = Cell->West()->Direction == UMapGeneratorBase::EDirection::East;
				bool bSouth = Cell->South()->Direction == UMapGeneratorBase::EDirection::North;
				bool bNorthI = Cell->North()->IsJunction && !bSouth;
				bool bEastI = Cell->East()->IsJunction && !bWest;
				bool bWestI = Cell->West()->IsJunction && !bEast;
				bool bSouthI = Cell->South()->IsJunction && !bNorth;
				bNorth |= bNorthI;
				bEast |= bEastI;
				bWest |= bWestI;
				bSouth |= bSouthI;
				if (bNorth && bEast)
				{
					Rotator = FRotator(0, 90, 0);
				}
				else if (bEast && bSouth)
				{
					Rotator = FRotator(0, 180, 0);
				}
				else if (bSouth && bWest)
				{
					Rotator = FRotator(0, 270, 0);
				}
				else if (bWest && bNorth)
				{
					Rotator = FRotator(0, 0, 0);
				}
			}
			else //直進路のとき
			{
				MeshType = AMapUnitBase::EMeshType::PathStraight;
				if (Cell->Direction == UMapGeneratorBase::EDirection::North)
				{
					Rotator = FRotator(0, 0, 0);
				}
				else if (Cell->Direction == UMapGeneratorBase::EDirection::East)
				{
					Rotator = FRotator(0, 90, 0);
				}
			}
		}
	}
	if (Cell->Attribution != UMapGeneratorBase::EType::Wall && MeshType != AMapUnitBase::EMeshType::Null)
	{
		AMapUnitBase* Base = GetWorld()->SpawnActor<AMapUnitBase>(Cie_Convert(Py, Px, CellSize), Rotator);
		Base->SetActorScale3D(FVector(CellSize / 100 * (bIsXReversed ? -1 : 1), CellSize / 100, CellSize / 100));
		Base->SetMeshType(MeshType);
	}
}

FVector UTerrainMaker::Cie_Convert(const int32 PosY, const int32 PosX, const float Height) const
{
	return FVector((PosX + 0.5) * CellSize, (PosY + 0.5) * CellSize, Height);
}

TArray<int32> UTerrainMaker::Cie_Invert(const FVector PosVector)
{
	return {FMath::RoundToInt32(PosVector.Y), FMath::RoundToInt32(PosVector.X)};
}