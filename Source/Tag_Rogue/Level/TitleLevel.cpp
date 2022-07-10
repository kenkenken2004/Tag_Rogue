// Fill out your copyright notice in the Description page of Project Settings.


#include "TitleLevel.h"

void ATitleLevel::BeginPlay()
{
	Super::BeginPlay();
	GameInstance = static_cast<UTag_RogueGameInstance*>(GetGameInstance());
	GameInstance->LoadAssets();
}
