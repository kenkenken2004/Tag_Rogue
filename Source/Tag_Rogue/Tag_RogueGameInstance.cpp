// Fill out your copyright notice in the Description page of Project Settings.


#include "Tag_RogueGameInstance.h"

UTag_RogueGameInstance::UTag_RogueGameInstance()
{/*
	ObjectLibrary = UObjectLibrary::CreateLibrary(UWorld::StaticClass(), true, true);
	ObjectLibrary->AddToRoot();
	ObjectLibrary->LoadAssetDataFromPath(TEXT("/Game"));
	ObjectLibrary->LoadAssetsFromAssetData();
	
	for(int32 i=0;i<AssetDatas.Num();i++)
	{
		FAssetData& AssetData = AssetDatas[i];
		UE_LOG(LogTemp, Log, TEXT("%s"), *AssetData.GetFullName())
	}*/
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

void UTag_RogueGameInstance::AddClass(FString)
{
}

void UTag_RogueGameInstance::AddObject(FString)
{
}

