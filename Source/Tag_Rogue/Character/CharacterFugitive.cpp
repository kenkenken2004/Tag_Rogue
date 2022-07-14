// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterFugitive.h"
#include "Components/CapsuleComponent.h"

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

	//衝突時の処理をセット
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ACharacterFugitive::OnOverlapBegin);
}

void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult){
	//衝突相手がCharacterBaseであり、RoleがChaserだった場合に、
	//負けたっぽい処理をしたり動きを止めたりする。予定。
}