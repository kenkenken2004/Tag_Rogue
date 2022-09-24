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
	if(!GetAttachmentRootActor()->HasAuthority())return;
	Initialize();
	UpdateNumbers();
}


// Called every frame
void ULimitCountComponent::TickComponent(const float DeltaTime, const ELevelTick TickType,
                                         FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	CheckShouldUpdateNumbers(DeltaTime);
}

void ULimitCountComponent::CheckShouldUpdateNumbers(const float DeltaTime)
{
	if(GameInstance->bShouldSChangeNumbers)
	{
		DigitLeftNumber = GameInstance->IntRemainingTime/10;
		DigitRightNumber = GameInstance->IntRemainingTime%10;
	}
	ColorLerp = GameInstance->DoesTimerStopped?1.0:0.0;
	UpdateNumbers();
}

void ULimitCountComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ULimitCountComponent, DisplayMesh);
	DOREPLIFETIME(ULimitCountComponent, OwnerPlayer);
	DOREPLIFETIME(ULimitCountComponent, DigitLeftNumber);
	DOREPLIFETIME(ULimitCountComponent, DigitRightNumber);
	DOREPLIFETIME(ULimitCountComponent, ColorLerp);
}

void ULimitCountComponent::UpdateNumbers_Implementation()
{
	if(!IsValid(GameInstance))Initialize();
	FString Left = TEXT("Tr2n_");
	Left.AppendInt('0'+DigitLeftNumber);
	FString Right = TEXT("Tr2n_");
	Right.AppendInt('0'+DigitRightNumber);
	UTexture* TLeft = GameInstance->GetAssetObject<UTexture>(FName(Left));
	UTexture* TRight = GameInstance->GetAssetObject<UTexture>(FName(Right));
    DigitLeft->SetTextureParameterValue(TEXT("Letter"), TLeft);
    DigitRight->SetTextureParameterValue(TEXT("Letter"), TRight);
	DigitLeft->SetScalarParameterValue(TEXT("ColorLerp"), ColorLerp);
	DigitRight->SetScalarParameterValue(TEXT("ColorLerp"), ColorLerp);
}

void ULimitCountComponent::Initialize_Implementation()
{
	OwnerPlayer = static_cast<ACharacterBase*>(GetAttachmentRootActor());
	GameInstance = UTag_RogueGameInstance::GetInstance();
	GameInstance->LoadAssets();
	DigitLeft = CreateAndSetMaterialInstanceDynamic(0);
	DigitRight = CreateAndSetMaterialInstanceDynamic(1);
}
