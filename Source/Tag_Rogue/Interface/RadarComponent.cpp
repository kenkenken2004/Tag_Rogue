// Fill out your copyright notice in the Description page of Project Settings.


#include "RadarComponent.h"

#include "Net/UnrealNetwork.h"

URadarComponent::URadarComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void URadarComponent::BeginPlay()
{
	Super::BeginPlay();
	Initialize();
}

void URadarComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (IsValid(OwnerPlayer)&&IsValid(OwnerPlayer->Enemy)&&IsValid(RadarMaterial))
	{
		const FVector RelativePosition = OwnerPlayer->Enemy->GetActorLocation() - OwnerPlayer->GetActorLocation();
		EnemyDistance = RelativePosition.Size();
		EnemyDirection = - (RelativePosition.Rotation().Yaw  - OwnerPlayer->GetControlRotation().Yaw) / 360;
		UpdateRadar();
	}
}

void URadarComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(URadarComponent, OwnerPlayer);
	DOREPLIFETIME(URadarComponent, EnemyDistance);
	DOREPLIFETIME(URadarComponent, EnemyDirection);
}

void URadarComponent::UpdateRadar_Implementation()
{
	if (IsValid(RadarMaterial))
	{
		RadarMaterial->SetScalarParameterValue(TEXT("EnemyDistance"),EnemyDistance);
		RadarMaterial->SetScalarParameterValue(TEXT("EnemyDirection"),EnemyDirection);
	}
}

void URadarComponent::Initialize_Implementation()
{
	OwnerPlayer = static_cast<ACharacterBase*>(GetAttachmentRootActor());
	GameInstance = UTag_RogueGameInstance::GetInstance();
	RadarMaterial = CreateAndSetMaterialInstanceDynamic(0);
}
