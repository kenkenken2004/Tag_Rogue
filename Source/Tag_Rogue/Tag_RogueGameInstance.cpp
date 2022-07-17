// Fill out your copyright notice in the Description page of Project Settings.


#include "Tag_RogueGameInstance.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "Level/GameMap_Alpha.h"

UTag_RogueGameInstance::UTag_RogueGameInstance()
{
	AssetDatas = TMap<FName, FAssetData>();
}

UTag_RogueGameInstance::~UTag_RogueGameInstance()
{
}

UTag_RogueGameInstance* UTag_RogueGameInstance::GetInstance()
{
	if (GEngine)return Cast<UTag_RogueGameInstance>(GEngine->GetWorldContextFromGameViewport(GEngine->GameViewport)->OwningGameInstance);
	return nullptr;
}

void UTag_RogueGameInstance::DisplayDebugMessage(const FString Message)
{
	if(GEngine)GEngine->AddOnScreenDebugMessage(-1,5.0f,FColor::Green,Message);
}

void UTag_RogueGameInstance::LoadAssets()
{
	if(bIsAssetDataLoaded)return;
	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName(TEXT("AssetRegistry")));
	const IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
	for(int32 i=0;i<AssetsPathArray.Num();i++)
	{
		TArray<FAssetData> AssetDatasInstant;
		AssetRegistry.GetAssetsByPath(AssetsPathArray[i], AssetDatasInstant);
		for(int32 j=0;j<AssetDatasInstant.Num();j++)
		{
			AssetDatas.Add(AssetDatasInstant[j].AssetName,AssetDatasInstant[j]);
		}
	}
	bIsAssetDataLoaded = true;
}

void UTag_RogueGameInstance::InitializeMapBuilders()
{
	if(IsValid(MapGenerator))return;
	MapGenerator = NewObject<URogueAlpha_MapGenerator>(GetWorld());
	MapGenerator->Construct(GameMapHeight,GameMapWidth);
	MapGenerator->SetStructureParam(UMapGeneratorBase::EType::Plaza, PlazaSize, PlazaSize, PlazaNum);
	MapGenerator->SetStructureParam(UMapGeneratorBase::EType::Room, RoomSize, RoomSize, RoomNum);
	TerrainMaker = NewObject<UTerrainMaker>(GetWorld());
	TerrainMaker->Construct(MapGenerator,CellSize);
	MapGenerator->BuildMap();
	MapGenerator->GetStructureString();
	FloatRemainingTime = GameTimeLimit;
	IntRemainingTime = GameTimeLimit;
}

void UTag_RogueGameInstance::ChaserWon()
{
	if(Settlement==ESettlement::Yet)
	{
		Settlement=ESettlement::Chaser;
		OnChaserWon();
		static_cast<AGameMap_Alpha*>(GetWorld()->GetLevelScriptActor())->OnGameEnd();
	}
}

void UTag_RogueGameInstance::FugitiveWon()
{
	if(Settlement==ESettlement::Yet)
	{
		Settlement=ESettlement::Fugitive;
		OnFugitiveWon();
		static_cast<AGameMap_Alpha*>(GetWorld()->GetLevelScriptActor())->OnGameEnd();
	}
}
