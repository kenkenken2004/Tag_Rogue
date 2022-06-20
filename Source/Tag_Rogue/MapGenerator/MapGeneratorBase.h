// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "MapGeneratorBase.generated.h"
#define WALL false
#define PATH true

/**
 * 
 */
UCLASS()
class TAG_ROGUE_API UMapGeneratorBase : public UObject
{
	GENERATED_BODY()
private:
	enum struct EType; //マス・部屋の属性
	enum struct EDirection;//方向
	struct FCell //マスの構造体
	{
		int32 Py, Px;
		int32 Index;
		int32 AreaIndex = -1;
		EType Attribution;
		UMapGeneratorBase& Gen;
		FCell(int32, int32, int32, UMapGeneratorBase&);
		FCell(int32, int32, int32, EType, UMapGeneratorBase&);
		void ChangeAttr(EType);
		
	};
	struct FRect //マスの長方形の領域の構造体
	{
		int32 Index = -1;
		FCell *LeftTopCell;
		FCell *RightBottomCell;
		int32 Height;
		int32 Width;
		UMapGeneratorBase& Gen;
		FRect(FCell&,FCell&);
		TArray<FCell*> GetInnerBorderCells() const; //周上の内側のCellを取得
		TArray<FCell*> GetOuterBorderCells() const; //周上の外側のCellを取得
		TArray<FCell*> GetAllCells() const; //構成するCellを取得
	};
	struct FSpace: FRect //部屋などの構造体
	{
		EType Attribution; //部屋の種類
		FSpace(FRect, EType);
		void ChangeAttr(EType); //部屋の種類を変更
		bool CanPlace() const;
		void Place();
	};
	struct FPath: FRect //通路などの構造体
	{
		FRect *Node1;
		FRect *Node2;
		int32 Length;
		EDirection Direction;
		FPath(FRect*, FRect*, const FRect*);
		bool CanPlace() const;
		void Place();
	};
	struct FArea: FRect //仮想的な領域の構造体
	{
		FArea(FRect);
		TArray<FArea> Split(EDirection, int32);
		void Expand(EDirection, int32);
		void Expand(EDirection);
		void Expand();
		bool CanPlace() const;
		void Place();
	};
	
	int32 MapHeight, MapWidth; //マップのサイズ
	TArray<FSpace*> SpaceList;
	TArray<FArea*> AreaList;
	TArray<FPath*> PathList;
	TArray<FCell> CellList; //マスの実体が入っているリスト。
	TArray<TArray<int>> MapMatrix; //CellListへの参照indexが入っている、マップを表現した二次元リスト。
	
public:
	UMapGeneratorBase(int32, int32);
	FCell& GetCell(int32, int32);
};
