// Fill out your copyright notice in the Description page of Project Settings.
#include "GameMap_Alpha.h"
#include "Tag_Rogue/MapObject/MapUnitBase.h"


AGameMap_Alpha::AGameMap_Alpha()
{
}


void AGameMap_Alpha::BeginPlay()
{
	GameInstance = Cast<UTag_RogueGameInstance, UGameInstance>(GetGameInstance());
	GameInstance->LoadAssets(FName(TEXT("/Game/MapObject/Mesh/")));
	GameInstance->LoadAssets(FName(TEXT("/Game/MapObject/Material/")));
	CellSize = 300.0;
	Generator = NewObject<URogueAlpha_MapGenerator>(GetWorld());
	Generator->Construct(50,50);
	Generator->SetStructureParam(UMapGeneratorBase::EType::Plaza, 9, 9, 4);
    Generator->SetStructureParam(UMapGeneratorBase::EType::Room, 5, 5, 9);
	Generator->BuildMap();
	PlaceCubes();
	Generator->GetAreaString();
	Super::BeginPlay();
}

void AGameMap_Alpha::PlaceCubes() const
{
	for(int32 y=0;y<Generator->MapHeight;y++)
	{
		for (int32 x=0;x<Generator->MapWidth;x++)
		{
			AMapUnitBase::EMeshType MeshType = AMapUnitBase::EMeshType::PathStraight;
			FRotator Rotator = FRotator(0,0,0);
			bool bIsXReversed = false;
			const UMapGeneratorBase::FCell* Cell = Generator->GetCell(y, x);
			if(Cell->Attribution==UMapGeneratorBase::EType::Plaza || Cell->Attribution==UMapGeneratorBase::EType::Room) //部屋のとき
			{
				if(Cell->IsGate) //接続口のとき
				{
					int32 Count1 = 0;
					if(Cell->North()->Attribution!=Cell->Attribution)Count1++;
					if(Cell->East() ->Attribution!=Cell->Attribution)Count1++;
					if(Cell->West() ->Attribution!=Cell->Attribution)Count1++;
					if(Cell->South()->Attribution!=Cell->Attribution)Count1++;
					int32 Count2 = 0;
					if(Cell->North()->Attribution==UMapGeneratorBase::EType::Path&&Cell->North()->Direction==UMapGeneratorBase::EDirection::North)Count2++;
					if(Cell->East() ->Attribution==UMapGeneratorBase::EType::Path&&Cell->North()->Direction==UMapGeneratorBase::EDirection::East)Count2++;
					if(Cell->West() ->Attribution==UMapGeneratorBase::EType::Path&&Cell->North()->Direction==UMapGeneratorBase::EDirection::East)Count2++;
					if(Cell->South()->Attribution==UMapGeneratorBase::EType::Path&&Cell->North()->Direction==UMapGeneratorBase::EDirection::North)Count2++;
					if(Count1==1)
					{
						MeshType = AMapUnitBase::EMeshType::GateNormal;
						if(Cell->North()->Attribution!=Cell->Attribution)Rotator = FRotator(0,180,0);
						if(Cell->East() ->Attribution!=Cell->Attribution)Rotator = FRotator(0,270,0);
						if(Cell->West() ->Attribution!=Cell->Attribution)Rotator = FRotator(0,90,0);
						if(Cell->South()->Attribution!=Cell->Attribution)Rotator = FRotator(0,0,0);
					}
					else if(Count1==2&&Count2==1)
					{
						MeshType = AMapUnitBase::EMeshType::GateCornerOne;
						if (Cell->North()->Attribution!=Cell->Attribution&&Cell->East()->Attribution!=Cell->Attribution)
						{
							Rotator = FRotator(0,180,0);
							if(Cell->East()->Attribution==UMapGeneratorBase::EType::Path)
							{
								bIsXReversed = true;
								Rotator.Yaw+=90;
							}
						}
						else if (Cell->East()->Attribution!=Cell->Attribution&&Cell->South()->Attribution!=Cell->Attribution)
						{
							Rotator = FRotator(0,270,0);
							if(Cell->South()->Attribution==UMapGeneratorBase::EType::Path)
							{
								bIsXReversed = true;
								Rotator.Yaw+=90;
							}
						}
						else if (Cell->South()->Attribution!=Cell->Attribution&&Cell->West()->Attribution!=Cell->Attribution)
						{
							Rotator = FRotator(0,0,0);
							if(Cell->West()->Attribution==UMapGeneratorBase::EType::Path)
							{
								bIsXReversed = true;
								Rotator.Yaw+=90;
							}
						}
						else if (Cell->West()->Attribution!=Cell->Attribution&&Cell->North()->Attribution!=Cell->Attribution)
						{
							Rotator = FRotator(0,90,0);
							if(Cell->North()->Attribution==UMapGeneratorBase::EType::Path)
							{
								bIsXReversed = true;
								Rotator.Yaw+=90;
							}
						}
					}
					else if(Count1==2&&Count2==2)
					{
						MeshType = AMapUnitBase::EMeshType::GateCornerTwo;
						if (Cell->North()->Attribution!=Cell->Attribution&&Cell->East()->Attribution!=Cell->Attribution)
						{
							Rotator = FRotator(0,270,0);
						}
						else if (Cell->East()->Attribution!=Cell->Attribution&&Cell->South()->Attribution!=Cell->Attribution)
						{
							Rotator = FRotator(0,0,0);
						}
						else if (Cell->South()->Attribution!=Cell->Attribution&&Cell->West()->Attribution!=Cell->Attribution)
						{
							Rotator = FRotator(0,90,0);
						}
						else if (Cell->West()->Attribution!=Cell->Attribution&&Cell->North()->Attribution!=Cell->Attribution)
						{
							Rotator = FRotator(0,180,0);
						}
					}
				}
				else //そうでないとき
				{
					int32 Count = 0;
					if(Cell->North()->Attribution!=Cell->Attribution)Count++;
					if(Cell->East() ->Attribution!=Cell->Attribution)Count++;
					if(Cell->West() ->Attribution!=Cell->Attribution)Count++;
					if(Cell->South()->Attribution!=Cell->Attribution)Count++;
					switch (Count)
					{
					case 0:
						MeshType = AMapUnitBase::EMeshType::RoomNormal;
						break;
					case 1:
						MeshType = AMapUnitBase::EMeshType::RoomSide;
						if(Cell->North()->Attribution!=Cell->Attribution)Rotator = FRotator(0,90,0);
						if(Cell->East() ->Attribution!=Cell->Attribution)Rotator = FRotator(0,180,0);
						if(Cell->West() ->Attribution!=Cell->Attribution)Rotator = FRotator(0,0,0);
						if(Cell->South()->Attribution!=Cell->Attribution)Rotator = FRotator(0,270,0);
						break;
					case 2:
						MeshType = AMapUnitBase::EMeshType::RoomCorner;
						if (Cell->North()->Attribution!=Cell->Attribution&&Cell->East()->Attribution!=Cell->Attribution)
						{
							Rotator = FRotator(0,90,0);
						}
						else if (Cell->East()->Attribution!=Cell->Attribution&&Cell->South()->Attribution!=Cell->Attribution)
						{
							Rotator = FRotator(0,180,0);
						}
						else if (Cell->South()->Attribution!=Cell->Attribution&&Cell->West()->Attribution!=Cell->Attribution)
						{
							Rotator = FRotator(0,270,0);
						}
						else if (Cell->West()->Attribution!=Cell->Attribution&&Cell->North()->Attribution!=Cell->Attribution)
						{
							Rotator = FRotator(0,0,0);
						}
						break;
					default:
						MeshType = AMapUnitBase::EMeshType::Null;
						break;
					}
				}
			}
			else if (Cell->Attribution==UMapGeneratorBase::EType::Path) //通路のとき
			{
				if(Cell->IsJunction) //交差点のとき
					{
					MeshType = AMapUnitBase::EMeshType::PathTJunction;
					if(Cell->North()->Attribution!=UMapGeneratorBase::EType::Path)
					{
						Rotator = FRotator(0,90,0);
					}
					else if(Cell->East() ->Attribution!=UMapGeneratorBase::EType::Path)
					{
						Rotator = FRotator(0,180,0);
					}
					else if(Cell->West() ->Attribution!=UMapGeneratorBase::EType::Path)
					{
						Rotator = FRotator(0,0,0);
					}
					else if(Cell->South()->Attribution!=UMapGeneratorBase::EType::Path)
					{
						Rotator = FRotator(0,270,0);
					}
					else
					{
						MeshType = AMapUnitBase::EMeshType::PathCrossRoad;
					}
					}
				else //交差点でないとき
					{
					if(Cell->IsCorner) //角のとき
						{
							MeshType = AMapUnitBase::EMeshType::PathCorner;
							if (Cell->North()->Direction==UMapGeneratorBase::EDirection::North&&Cell->East()->Direction==UMapGeneratorBase::EDirection::East)
							{
								Rotator = FRotator(0,90,0);
							}
							else if (Cell->East()->Direction==UMapGeneratorBase::EDirection::East&&Cell->South()->Direction==UMapGeneratorBase::EDirection::South)
							{
								Rotator = FRotator(0,180,0);
							}
							else if (Cell->South()->Direction==UMapGeneratorBase::EDirection::South&&Cell->West()->Direction==UMapGeneratorBase::EDirection::West)
							{
								Rotator = FRotator(0,270,0);
							}
							else if (Cell->West()->Direction==UMapGeneratorBase::EDirection::West&&Cell->North()->Direction==UMapGeneratorBase::EDirection::North)
							{
								Rotator = FRotator(0,0,0);
							}
						}
					else //直進路のとき
						{
							MeshType = AMapUnitBase::EMeshType::PathStraight;
							if (Cell->Direction==UMapGeneratorBase::EDirection::North)
							{
								Rotator = FRotator(0,0,0);
							}
							else if (Cell->Direction==UMapGeneratorBase::EDirection::East)
							{
								Rotator = FRotator(0,90,0);
							}
						}
					}
			}
			if(Cell->Attribution!=UMapGeneratorBase::EType::Wall && MeshType!=AMapUnitBase::EMeshType::Null)
			{
				AMapUnitBase* Base = GetWorld()->SpawnActor<AMapUnitBase>(Cie_Convert(y,x,CellSize/2), Rotator);
				Base->SetActorScale3D(FVector(CellSize/100*(bIsXReversed?-1:1),CellSize/100,CellSize/100));
				Base->SetMeshType(MeshType);
			}
		}
	}
	
}


void AGameMap_Alpha::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

FVector AGameMap_Alpha::Cie_Convert(const int32 PosY, const int32 PosX, const float Height) const
{
	return FVector((PosX+0.5)*CellSize,(PosY+0.5)*CellSize,Height);
}

TArray<int32> AGameMap_Alpha::Cie_Invert(const FVector PosVector)
{
	return {FMath::RoundToInt32(PosVector.Y),FMath::RoundToInt32(PosVector.X)};
}

