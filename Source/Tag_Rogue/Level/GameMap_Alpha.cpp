// Fill out your copyright notice in the Description page of Project Settings.
#include "GameMap_Alpha.h"

#include "GameFramework/GameSession.h"
#include "Kismet/GameplayStatics.h"
#include "Tag_Rogue/Interface/LimitCountComponent.h"

AGameMap_Alpha::AGameMap_Alpha()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;
}

void AGameMap_Alpha::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(IsValid(GameInstance))
	{
		if (!GameInstance->DoesTimerStopped)
		{
			if(GameInstance->FloatRemainingTime>0)
			{
				GameInstance->bShouldSChangeNumbers = false;
				GameInstance->FloatRemainingTime -= DeltaSeconds;
				if(FMath::CeilToInt32(GameInstance->FloatRemainingTime) < GameInstance->IntRemainingTime)
				{
					GameInstance->IntRemainingTime = FMath::CeilToInt32(GameInstance->FloatRemainingTime);
					GameInstance->bShouldSChangeNumbers = true;
				}
			}else if(GameInstance->IntRemainingTime==0 && GameInstance->Settlement == ESettlement::Yet)
			{
				GameInstance->FugitiveWon();
			}
		}
	}
}


void AGameMap_Alpha::BeginPlay()
{
	Super::BeginPlay();
	Initialize(GetGameInstance());
}

void AGameMap_Alpha::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	SaveConfig();
}

void AGameMap_Alpha::Initialize_Implementation(UGameInstance* GameIns)
{
	GameInstance = static_cast<UTag_RogueGameInstance*>(GameIns);
	GameInstance->LoadAssets();
	GameInstance->InitializeMapBuilders();
	GameInstance->TerrainMaker->Build();
}