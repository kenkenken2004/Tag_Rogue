// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterFugitive.h"

ACharacterFugitive::ACharacterFugitive() {
	UE_LOG(LogTemp, Warning, TEXT("aaaaa"));
	//ƒƒbƒVƒ…‚ğİ’è
	SetMesh(TEXT("/Game/Characters/OriginalCharacter/CleaningRobot/CleaningRobot"));

	//behaviour‚ğİ’è
	BehaviourNumber = 1;
	BH1.MaxSpeed = 300;
	BH1.TurboSpeed = 600;
	ReloadWalkSpeed();
}
