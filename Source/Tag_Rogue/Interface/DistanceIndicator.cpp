// Fill out your copyright notice in the Description page of Project Settings.


#include "DistanceIndicator.h"

#include "Net/UnrealNetwork.h"

UDistanceIndicator::UDistanceIndicator()
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UDistanceIndicator::BeginPlay()
{
	Super::BeginPlay();
	Initialize();
}

void UDistanceIndicator::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (IsValid(OwnerPlayer)&&IsValid(OwnerPlayer->Enemy)&&IsValid(IndicatorMaterial))
	{
		Distance = (OwnerPlayer->GetActorLocation()-OwnerPlayer->Enemy->GetActorLocation()).Length();
		Closeness = FMath::Min(1,FMath::Pow((StandardMaxDistance/Distance),0.5));
		UpdateIndicator();
	}
}

void UDistanceIndicator::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UDistanceIndicator, OwnerPlayer);
	DOREPLIFETIME(UDistanceIndicator, Distance);
	DOREPLIFETIME(UDistanceIndicator, Closeness);

}

void UDistanceIndicator::Initialize_Implementation()
{
	OwnerPlayer = static_cast<ACharacterBase*>(GetAttachmentRootActor());
	GameInstance = UTag_RogueGameInstance::GetInstance();
	IndicatorMaterial = CreateAndSetMaterialInstanceDynamic(0);
}

void UDistanceIndicator::UpdateIndicator_Implementation()
{
	if (IsValid(IndicatorMaterial))
	{
		IndicatorMaterial->SetScalarParameterValue(TEXT("Closeness"),Closeness);
	}
}