// Fill out your copyright notice in the Description page of Project Settings.


#include "TestActor.h"


// Sets default values
ATestActor::ATestActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}
#pragma optimize("", off)

class Hoge{
private:
	int number;
public:
	Hoge(int num);
	int baka;
};

Hoge::Hoge(int num) {
	number = num * 2;
}

// Called when the game starts or when spawned
void ATestActor::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Error, TEXT("Let's Start!"));
	int32 a = 1;
	//auto hoge1 = NewObject<Hoge>();
	Hoge hoge2 = Hoge(5);
	hoge2->baka = 10;
	
	//UMapGeneratorBase uMapBase = UMapGeneratorBase(30, 30);
	//int output = uMapBase.GetCell(1, 1)->AreaIndex;
	//UE_LOG(LogTemp, Error, TEXT("%d"), output);
	URogueBeta_MapGenerator uMapBeta = URogueBeta_MapGenerator(30, 30);
}

// Called every frame
void ATestActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

