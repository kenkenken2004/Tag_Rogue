// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterChaser.h"

ACharacterChaser::ACharacterChaser() {
	//������ݒ�
	Role = "Chaser";

	//���b�V����ݒ�
	SetMesh(TEXT("/Game/Characters/OriginalCharacter/FloatingRobot/FloatingRobot"));

	//behaviour��ݒ�
	BehaviourNumber = 2;
	BH2.MaxSpeed = 300;
	BH2.TurboSpeed = 600;
	BH2.RotateSpeed = 0.6;
	ReloadWalkSpeed();

	//������悤�ɂ���
	CanMove = true;
}