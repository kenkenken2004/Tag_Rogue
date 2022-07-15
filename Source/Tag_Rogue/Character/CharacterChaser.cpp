// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterChaser.h"

ACharacterChaser::ACharacterChaser() {
	//–ğŠ„‚ğİ’è
	Role = "Chaser";

	//ƒƒbƒVƒ…‚ğİ’è
	SetMesh(TEXT("/Game/Characters/OriginalCharacter/FloatingRobot/FloatingRobot"));

	//behaviour‚ğİ’è
	BehaviourNumber = 2;
	BH2.MaxSpeed = 300;
	BH2.TurboSpeed = 600;
	BH2.RotateSpeed = 0.6;
	ReloadWalkSpeed();

	//“®‚¯‚é‚æ‚¤‚É‚·‚é
	CanMove = true;
}