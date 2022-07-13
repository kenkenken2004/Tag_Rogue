/*
�ǂ����Ɠ����鑤�o���̌��ƂȂ�N���X
���ʗv�f��ݒ肵����A���ʂ��铮�������������肷��
�q�N���X�ł́A���ꂼ��ɓƎ��̗v�f(���b�V����A�V�X�e����̖���)����������
*/

#include "CharacterBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Tag_Rogue/Interface/LimitCountComponent.h"
#include "Tag_Rogue/Interface/MiniMap.h"
#include "Tag_Rogue/Interface/MiniMapComponent.h"

//////////////////////////////////////////////////////////////////////////
// ACharacterBase

ACharacterBase::ACharacterBase()
{
	//�R���C�_�[�̌`��������
	GetCapsuleComponent()->InitCapsuleSize(ColliderDiameter, ColliderHeight);

	//�p�����[�^�[�̏����l��K�p
	ReloadVariables();

	//�{�ԗv��Ȃ������Ȑݒ�
	//�}�E�X�𓮂����Ǝ��_���������A���̂Ƃ��J���������������Ȃ��悤�ɂ���
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;
	//�����Ă����������������
	GetCharacterMovement()->bOrientRotationToMovement = false;
	

	/*�悭�킩���ݒ����U�|�C
	// set our turn rate for input
	TurnRateGamepad = 50.f;

	

	// Configure character movement
	 // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	*/
	

	// CameraBoom��ݒ�
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetCapsuleComponent());
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// �J������ݒ�
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = true;

	/*��UMiniMap��LimitCount�͂Ȃ���
	MiniMap = CreateDefaultSubobject<UMiniMapComponent>(TEXT("MiniMap"));
	MiniMap->SetupAttachment(CameraBoom);
	MiniMap->SetRelativeLocation(FVector(200,0,-30));
	MiniMap->SetRelativeRotation(FRotator(0,270,0));
	MiniMap->SetRelativeScale3D(FVector(0.3,0.3,0.3));
	MiniMap->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	

	LimitCount = CreateDefaultSubobject<ULimitCountComponent>(TEXT("LimitCount"));
	LimitCount->SetupAttachment(CameraBoom);
	LimitCount->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	*/
}


void ACharacterBase::SetMesh(FString path) {
	//SkeletalMesh�ō쐬
	RobotMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RobotMesh"));
	USkeletalMesh* MeshTmp = LoadObject<USkeletalMesh>(NULL, *path, NULL, LOAD_None, NULL);
	RobotMesh->SetSkeletalMesh(MeshTmp);
	//Root�ɃA�^�b�`
	RobotMesh->SetupAttachment(RootComponent);
	//�n�ʂɒ����悤�ɂ���
	RobotMesh->SetRelativeLocation(FVector(0, 0, -ColliderHeight));
}

void ACharacterBase::ReloadVariables() {
	GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
}


//////////////////////////////////////////////////////////////////////////
// Input

void ACharacterBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	//�g�p��������
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ACharacterBase::MoveForward);//�O��̓������o�C���h
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ACharacterBase::MoveRight);
	PlayerInputComponent->BindAction("Action1", IE_Pressed, this, &ACharacterBase::Action1);//�A�N�V����1�{�^���������ꂽ��A�N�V����1


	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	//PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ACharacterBase::TurnAtRate);
	//PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ACharacterBase::LookUpAtRate);

	// handle touch devices
	PlayerInputComponent->BindTouch(IE_Pressed, this, &ACharacterBase::TouchStarted);
	PlayerInputComponent->BindTouch(IE_Released, this, &ACharacterBase::TouchStopped);
}




void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	//MiniMap->SetRelativeLocation(FVector(100,0,-40));
	//MiniMap->SetRelativeRotation(FRotator(0,270,60));
	//MiniMap->SetRelativeScale3D(FVector(0.3,0.3,0.3));
	
	//�eBehaviour�̏����ݒ�
	BH1.TurboDuration = 3;
	BH1.RotateSpeed = 30;
	BH1.RotateDir = BH1.RotateSum = BH1.TurboTimer = 0;
	BH1.IsTurbo = false;

	//Behaviour��ݒ�
	BehaviourNumber = 1;
}

void ACharacterBase::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	TimeSinceCreated+=DeltaSeconds;

	//Behaviour
	switch (BehaviourNumber)
	{
	case 1:
		

		//�^�[�{�֘A
		if (BH1.IsTurbo) {
			BH1.TurboTimer += DeltaSeconds;
			if (BH1.TurboTimer >= BH1.TurboDuration) {
				GetCharacterMovement()->MaxWalkSpeed = MaxSpeed;
				BH1.TurboTimer = 0;
				BH1.IsTurbo = false;
			}
		}
		//��]�֘A
		if (BH1.RotateDir != 0) {
			float PredictedRotateSum = BH1.RotateSum + BH1.RotateSpeed * DeltaSeconds;
			UE_LOG(LogTemp, Log, TEXT("%f"), PredictedRotateSum);
			if (PredictedRotateSum >= 90) {
				RootComponent->AddWorldRotation(FRotator(0.f, (90 - BH1.RotateSum) * BH1.RotateDir, 0.f));
				BH1.RotateDir = 0;
				BH1.RotateSum = 0;
			}
			else {
				RootComponent->AddWorldRotation(FRotator(0.f, BH1.RotateSpeed * DeltaSeconds * BH1.RotateDir, 0.f));
				BH1.RotateSum += BH1.RotateSpeed * DeltaSeconds;
			}
		}
		//�O�ɐi��
		if (BH1.RotateDir == 0) {
			FVector ForwardVector = FRotationMatrix(FRotator(0, Controller->GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::X);
			AddMovementInput(ForwardVector, 1);
		}
		break;
	}
}

void ACharacterBase::MoveForward(float Value)
{
	/*
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
	*/
}

void ACharacterBase::MoveRight(float Value)
{
	switch (BehaviourNumber)
	{
	case 1:
		//��]���J�n����
		if (BH1.RotateDir == 0) {
			if (Value > 0)BH1.RotateDir = 1;
			else if (Value < 0)BH1.RotateDir = -1;
		}
		break;
	}
	/*
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);
	
		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
	*/
}

void ACharacterBase::Action1() {
	switch (BehaviourNumber)
	{
	case 1:
		if (!BH1.IsTurbo) {
			GetCharacterMovement()->MaxWalkSpeed = TurboSpeed;
			BH1.IsTurbo = true;
		}
		break;
	}
}

void ACharacterBase::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	Jump();
}

void ACharacterBase::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	StopJumping();
}

void ACharacterBase::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void ACharacterBase::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}