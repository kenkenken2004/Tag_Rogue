// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterFugitive.h"

ACharacterFugitive::ACharacterFugitive() {
	UE_LOG(LogTemp, Warning, TEXT("aaaaa"));
	//メッシュを設定
	SetMesh(TEXT("/Game/Characters/OriginalCharacter/CleaningRobot/CleaningRobot"));

	//behaviourを設定
	BehaviourNumber = 1;
	BH1.MaxSpeed = 300;
	BH1.TurboSpeed = 600;
	ReloadWalkSpeed();
}
