// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterFugitive.h"

ACharacterFugitive::ACharacterFugitive() {
	//役割を設定
	Role = "Fugitive";

	//メッシュを設定
	SetMesh(TEXT("/Game/Characters/OriginalCharacter/CleaningRobot/CleaningRobot"));

	//behaviourを設定
	BehaviourNumber = 2;
	BH2.MaxSpeed = 300;
	BH2.TurboSpeed = 600;
	BH2.RotateSpeed = 0.6;
	ReloadWalkSpeed();

	//動けるようにする
	CanMove = true;
}
