// Fill out your copyright notice in the Description page of Project Settings.


#include "LockOnCursor.h"

#include "Camera/CameraComponent.h"
#include "Net/UnrealNetwork.h"

ULockOnCursor::ULockOnCursor()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void ULockOnCursor::BeginPlay()
{
	Super::BeginPlay();
	Initialize();
}

void ULockOnCursor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (IsValid(OwnerPlayer)&&IsValid(OwnerPlayer->Enemy)&&IsValid(CursorMaterial))
	{
		if (DoesIndicate)
		{
			FVector EnemyVector = OwnerPlayer->Enemy->GetActorLocation()-OwnerPlayer->GetFollowCamera()->GetComponentLocation();
			EnemyVector = EnemyVector*GetAttachParent()->GetRelativeLocation().X/EnemyVector.Dot(OwnerPlayer->GetFollowCamera()->GetForwardVector()) + OwnerPlayer->GetFollowCamera()->GetComponentLocation() - GetAttachParent()->GetComponentLocation();
			const float Limit = FMath::Tan(FMath::Acos(0.9))*GetAttachParent()->GetRelativeLocation().X;
			UVposY = EnemyVector.Dot(-GetAttachParent()->GetRightVector()) / (Limit*11/6) * 1.2;
			UVposX = -EnemyVector.Dot(-GetAttachParent()->GetForwardVector()) / Limit * 0.4;
		}
		UpdateCursor();
	}
}

void ULockOnCursor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ULockOnCursor, OwnerPlayer);
	DOREPLIFETIME(ULockOnCursor, DoesIndicate);
	DOREPLIFETIME(ULockOnCursor, UVposX);
	DOREPLIFETIME(ULockOnCursor, UVposY);
}

void ULockOnCursor::UpdateCursor_Implementation()
{
	if (IsValid(CursorMaterial))
	{
		CursorMaterial->SetScalarParameterValue(TEXT("PosX"),UVposX);
		CursorMaterial->SetScalarParameterValue(TEXT("PosY"),UVposY);
		CursorMaterial->SetScalarParameterValue(TEXT("Opacity"),DoesIndicate?1:0);
	}
}

void ULockOnCursor::Initialize_Implementation()
{
	OwnerPlayer = static_cast<ACharacterBase*>(GetAttachmentRootActor());
	GameInstance = UTag_RogueGameInstance::GetInstance();
	CursorMaterial = CreateAndSetMaterialInstanceDynamic(0);
}
