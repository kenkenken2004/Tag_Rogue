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
	enum struct ECellType; //マスの属性
	enum struct ESpaceType;//部屋の属性
	enum struct EDirection;//方向
	struct FCell //マスの構造体
	{
		int32 Py, Px;
		int32 Index;
		ECellType Attribution;
		FCell(int32, int32, int32);
		FCell(int32, int32, int32, ECellType);
		void ChangeAttr(ECellType);
		
	};
	struct FRect //マスの長方形の領域の構造体
	{
		FCell &LeftTopCell;
		FCell &RightBottomCell;
		int32 Height;
		int32 Width;
		FRect(const FCell&, const FCell&);
		TArray<FCell*> GetInnerBorderCells(UMapGeneratorBase&) const; //周上の内側のCellを取得
		TArray<FCell*> GetOuterBorderCells(UMapGeneratorBase&) const; //周上の外側のCellを取得
		TArray<FCell*> GetAllCells(UMapGeneratorBase&) const; //構成するCellを取得
	};
	struct FSpace: FRect //部屋などの構造体
	{
		ESpaceType Attribution; //部屋の種類
		FSpace(FRect, ESpaceType);
		void ChangeAttr(ESpaceType); //部屋の種類を変更
	};
	struct FPath: FRect //通路などの構造体
	{
		FRect &Node1, &Node2;
		int32 Length;
		EDirection Direction;
		FPath(FRect&, FRect&, FRect&);
		
	};
	struct FArea: FRect //仮想的な領域の構造体
	{
		FArea();
		void Expand(EDirection);
	};
	int32 MapHeight, MapWidth; //マップのサイズ
	TArray<FCell> CellList; //マスの実体が入っているリスト。
	TArray<TArray<int>> MapMatrix; //CellListへの参照indexが入っている、マップを表現した二次元リスト。
public:
	UMapGeneratorBase(int32, int32);
	FCell GetCell(int32, int32);
};
