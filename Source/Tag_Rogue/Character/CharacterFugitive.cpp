// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterFugitive.h"

ACharacterFugitive::ACharacterFugitive() {
	UE_LOG(LogTemp, Warning, TEXT("aaaaa"));
	//メッシュを設定
	SetMesh(TEXT("/Game/Characters/OriginalCharacter/CleaningRobot/CleaningRobot"));

	//パラメーターを設定
	MaxSpeed = 300;
	TurboSpeed = 600;
	ReloadVariables();
}
