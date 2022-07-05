// Fill out your copyright notice in the Description page of Project Settings.


#include "Tag_RogueGameInstance.h"

#include "AssetRegistry/AssetRegistryModule.h"

UTag_RogueGameInstance::UTag_RogueGameInstance()
{
	AssetDatas = TMap<FName, FAssetData>();
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

void UTag_RogueGameInstance::LoadAssets(const FName Path)
{
	const FAssetRegistryModule& AssetRegistryModule = FModuleManager::LoadModuleChecked<FAssetRegistryModule>(FName(TEXT("AssetRegistry")));
	const IAssetRegistry& AssetRegistry = AssetRegistryModule.Get();
	TArray<FAssetData> AssetDatasInstant;
	AssetRegistry.GetAssetsByPath(Path, AssetDatasInstant);
	for(int32 i=0;i<AssetDatasInstant.Num();i++)
	{
		AssetDatas.Add(AssetDatasInstant[i].AssetName,AssetDatasInstant[i]);
	}
}