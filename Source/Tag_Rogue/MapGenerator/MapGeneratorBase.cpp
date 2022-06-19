// Fill out your copyright notice in the Description page of Project Settings.


#include "MapGeneratorBase.h"

enum struct UMapGeneratorBase::ECellType
{
	Wall,
	Path,
	
};

struct UMapGeneratorBase::FCell
{
	int32 Px, Py;
	TArray<ECellType> Attribution;
	int32 ID;
	FCell(const int32 X, const int32 Y)
	{
		Px = X;Py = Y;
		Attribution = TArray<ECellType>();
		
		
	}
	
};

UMapGeneratorBase::UMapGeneratorBase()
{
	
}
