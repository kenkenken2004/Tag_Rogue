// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CharacterBase.generated.h"

UCLASS(config=Game)
class ACharacterBase : public ACharacter
{
	GENERATED_BODY()

//
//�֐��A�\����
//

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	void SetMesh(FString path);//���b�V�����Z�b�g����@�h���N���X����Ăяo��

	void ReloadVariables();//���������Ȃǂ̃p�����[�^�[���A���̃����o�ϐ��̒l�ɐݒ肷��

	//��Third-human-4�̓�����������Ƃ��̕ϐ�
	//��{�I�ɂ����ƑO�i���Ă���
	//���E�{�^���������Ƃ��̕�����90�x��]����
	//�^�[�{����
	//��]���͗����~�܂�
	struct Behaviour1 {
		float TurboDuration;//�^�[�{���钷��
		float TurboTimer;//�^�[�{���Ă��钷��
		bool IsTurbo;//���^�[�{���Ă��邩
		float RotateSpeed;//��]���鑬��(deg/s)
		float RotateSum;//����̈�A�̉�]�ŁA���ɉ�����p�x
		float RotateDir;//������(�I�C���[�pZ�������ɉ�]����Ƃ���)
	};

	//���͂��󂯎����
	void MoveForward(float Value);

	void MoveRight(float Value);

	void Action1();
	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	
//
//�ϐ�
//
protected:
	//�����p�����[�^�[
	const float ColliderHeight = 50.0f;
	const float ColliderDiameter = 25.0f;
	float MaxSpeed = 300;
	float TurboSpeed = 500;
	int BehaviourNumber = 1;//���쐫�̔ԍ��@�e�L�[�ɂ����͂́A���̔ԍ��ɂ���Ď����I�ɕ��򂷂�

	//�N���X���ŋ��L����ꎞ�ϐ�
	Behaviour1 BH1;
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	

public:
	ACharacterBase();

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* RobotMesh;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class UMiniMapComponent* MiniMap;
	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	//class ULimitCountComponent* LimitCount;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;
	float TimeSinceCreated = 0;

};

