// Fill out your copyright notice in the Description page of Project Settings.

#include "MapUnitBase.h"

// Sets default values
AMapUnitBase::AMapUnitBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMapUnitBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMapUnitBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

