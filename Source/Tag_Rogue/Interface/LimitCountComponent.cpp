// Fill out your copyright notice in the Description page of Project Settings.


#include "LimitCountComponent.h"

#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
ULimitCountComponent::ULimitCountComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	// ...
}


// Called when the game starts
void ULimitCountComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerPlayer = static_cast<ACharacterBase*>(GetAttachmentRootActor());
}


// Called every frame
void ULimitCountComponent::TickComponent(const float DeltaTime, const ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	AddRelativeLocation(FVector(0,0,3*DeltaTime*FMath::Cos(OwnerPlayer->TimeSinceCreated/0.5*2*PI)));
	// ...
}

void ULimitCountComponent::CheckShouldUpdateNumbers_Implementation(const float DeltaTime)
{
	if(FMath::CeilToInt32(GameInstance->FloatRemainingTime) < GameInstance->IntRemainingTime)
	{
		GameInstance->IntRemainingTime = FMath::CeilToInt32(GameInstance->FloatRemainingTime);
		UpdateNumbers();
	}
}

void ULimitCountComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ULimitCountComponent, DisplayMesh);
	DOREPLIFETIME(ULimitCountComponent, DigitLeft);
	DOREPLIFETIME(ULimitCountComponent, DigitRight);
	DOREPLIFETIME(ULimitCountComponent, OwnerPlayer);
}

void ULimitCountComponent::UpdateNumbers_Implementation() const
{
	FString Left = TEXT("Tr2n_");
	Left.AppendInt('0'+GameInstance->IntRemainingTime/10);
	FString Right = TEXT("Tr2n_");
	Right.AppendInt('0'+GameInstance->IntRemainingTime%10);
	UTexture* TLeft = GameInstance->GetAssetObject<UTexture>(FName(Left));
	UTexture* TRight = GameInstance->GetAssetObject<UTexture>(FName(Right));
    DigitLeft->SetTextureParameterValue(TEXT("Letter"), TLeft);
    DigitRight->SetTextureParameterValue(TEXT("Letter"), TRight);
	UE_LOG(LogTemp, Warning, TEXT("%s"), *OwnerPlayer->GetName())
}

void ULimitCountComponent::Initialize_Implementation()
{
	GameInstance = static_cast<UTag_RogueGameInstance*>(GetOwner()->GetGameInstance());
	GameInstance->LoadAssets();
	DisplayMesh = GameInstance->GetAssetObject<UStaticMesh>(TEXT("CountDisplay"));
	SetStaticMesh(DisplayMesh);
	DigitLeft = CreateAndSetMaterialInstanceDynamic(0);
	DigitRight = CreateAndSetMaterialInstanceDynamic(1);
	SetRelativeLocation(FVector(120,0,-20));
	SetRelativeRotation(FRotator(0,270,30));
	SetRelativeScale3D(FVector(0.1,0.1,0.1));
}
