// Fill out your copyright notice in the Description page of Project Settings.


#include "Compass.h"

#include "Net/UnrealNetwork.h"

UCompass::UCompass()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UCompass::BeginPlay()
{
	Super::BeginPlay();
	Initialize();
}

void UCompass::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (IsValid(OwnerPlayer)&&IsValid(OwnerPlayer->Enemy)&&IsValid(CompassMaterial))
	{
		Direction = (OwnerPlayer->GetControlRotation().Yaw)/360;
		EnemyDirection = ((OwnerPlayer->Enemy->GetActorLocation()-OwnerPlayer->GetActorLocation()).Rotation().Yaw) / 360 - Direction;
		UpdateCompass();
	}
}

void UCompass::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UCompass, OwnerPlayer);
	DOREPLIFETIME(UCompass, Direction);
	DOREPLIFETIME(UCompass, EnemyDirection);
}

void UCompass::UpdateCompass_Implementation()
{
	if (IsValid(CompassMaterial))
	{
		CompassMaterial->SetScalarParameterValue(TEXT("Direction"),Direction);
		CompassMaterial->SetScalarParameterValue(TEXT("EnemyDirection"),EnemyDirection);
	}
}

void UCompass::Initialize_Implementation()
{
	OwnerPlayer = static_cast<ACharacterBase*>(GetAttachmentRootActor());
	GameInstance = UTag_RogueGameInstance::GetInstance();
	CompassMaterial = CreateAndSetMaterialInstanceDynamic(0);
}
