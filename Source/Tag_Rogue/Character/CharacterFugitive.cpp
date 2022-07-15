// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterFugitive.h"
#include "Components/CapsuleComponent.h"

ACharacterFugitive::ACharacterFugitive() {
	//–ğŠ„‚ğİ’è
	Role = "Fugitive";

	//ƒƒbƒVƒ…‚ğİ’è
	SetMesh(TEXT("/Game/Characters/OriginalCharacter/CleaningRobot/CleaningRobot"));

	//behaviour‚ğİ’è
	BehaviourNumber = 2;
	BH2.MaxSpeed = 300;
	BH2.TurboSpeed = 600;
	BH2.RotateSpeed = 0.6;
	ReloadWalkSpeed();

	//“®‚¯‚é‚æ‚¤‚É‚·‚é
	CanMove = true;
}
