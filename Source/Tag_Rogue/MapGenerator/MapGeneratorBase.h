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
	enum struct ECellType;
	enum struct ESpaceType;
	struct FCell
	{
		int32 Py, Px;
		int32 ID;
		ECellType Attribution;
		FCell(int32, int32, int32);
		FCell(int32, int32, int32, ECellType);
		void ChangeAttr(ECellType);
		
	};
	struct FRect
	{
		FCell LeftTopCell;
		FCell RightBottomCell;
		int32 Height;
		int32 Width;
		FRect(FCell, FCell);
	};
	struct FSpace
	{
		FRect Rect;
		ESpaceType Attribution;
		FSpace(FRect, ESpaceType);
	};
	int32 MapHeight, MapWidth;
	TArray<FCell> CellList;
	TArray<TArray<int>> MapMatrix;
public:
	UMapGeneratorBase(int32, int32);
	FCell GetCell(int32 PosY, int32 PosX);
	
};
