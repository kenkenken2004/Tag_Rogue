// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor.h"


// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}
#pragma optimize("", off)


// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Error, TEXT("Let's Start!"));
	
	URogueBeta_MapGenerator uMapBeta = URogueBeta_MapGenerator(30, 30, 35, 15, 3);
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

