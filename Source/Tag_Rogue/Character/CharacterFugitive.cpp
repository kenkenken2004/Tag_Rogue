// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterFugitive.h"
#include "Components/CapsuleComponent.h"

ACharacterFugitive::ACharacterFugitive() {
	//������ݒ�
	Role = "Fugitive";

	//���b�V����ݒ�
	SetMesh(TEXT("/Game/Characters/OriginalCharacter/CleaningRobot/CleaningRobot"));

	//behaviour��ݒ�
	BehaviourNumber = 2;
	BH2.MaxSpeed = 300;
	BH2.TurboSpeed = 600;
	BH2.RotateSpeed = 0.6;
	ReloadWalkSpeed();

	//������悤�ɂ���
	CanMove = true;

	//�Փˎ��̏������Z�b�g
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACharacterFugitive::OnOverlapBegin);
}

void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	//�Փˑ��肪CharacterBase�ł���ARole��Chaser�������ꍇ�ɁA
	//���������ۂ������������蓮�����~�߂��肷��B�\��B
}