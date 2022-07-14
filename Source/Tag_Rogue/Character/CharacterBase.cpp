/*
追う側と逃げる側双方の元となるクラス
共通要素を設定したり、共通する動きを実装したりする
子クラスでは、それぞれに独自の要素(メッシュや、システム上の役割)を実装する
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
	//コライダーの形を初期化
	GetCapsuleComponent()->InitCapsuleSize(ColliderDiameter, ColliderHeight);

	//歩く速さの初期値を適用
	ReloadWalkSpeed();

	//本番要らなさそうな設定
	//マウスを動かすと視点が動くが、このときカメラしか動かさないようにする
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;
	//動いている方向を向かせる
	GetCharacterMovement()->bOrientRotationToMovement = false;
	

	///*よくわからん設定を一旦ポイ
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
	//*/
	

	// CameraBoomを設定
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetCapsuleComponent());
	CameraBoom->TargetArmLength = 100.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// カメラを設定
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = true;

	/*一旦MiniMapとLimitCountはなしで
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
	//SkeletalMeshで作成
	RobotMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("RobotMesh"));
	USkeletalMesh* MeshTmp = LoadObject<USkeletalMesh>(NULL, *path, NULL, LOAD_None, NULL);
	RobotMesh->SetSkeletalMesh(MeshTmp);
	//Rootにアタッチ
	RobotMesh->SetupAttachment(RootComponent);
	//地面に着くようにする
	RobotMesh->SetRelativeLocation(FVector(0, 0, -ColliderHeight));
}

void ACharacterBase::ReloadWalkSpeed() {
	switch (BehaviourNumber) {
	case 1:
		GetCharacterMovement()->MaxWalkSpeed = BH1.MaxSpeed;
		break;
	}
	
}


//////////////////////////////////////////////////////////////////////////
// Input

void ACharacterBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	//使用したもの
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ACharacterBase::MoveForward);//前後の動きをバインド
	PlayerInputComponent->BindAxis("Move Right / Left", this, &ACharacterBase::MoveRight);
	PlayerInputComponent->BindAction("Action1", IE_Pressed, this, &ACharacterBase::Action1);//アクション1ボタンが押されたらアクション1


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
	
	//Behaviourを初期設定　派生クラスから好きに番号を変えたり、BH1の中身を変えたりしてよい
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
		

		//ターボ関連
		if (BH1.IsTurbo) {
			BH1.TurboTimer += DeltaSeconds;
			if (BH1.TurboTimer >= BH1.TurboDuration) {
				GetCharacterMovement()->MaxWalkSpeed = BH1.MaxSpeed;
				BH1.TurboTimer = 0;
				BH1.IsTurbo = false;
			}
		}
		//回転関連
		if (BH1.RotateDir != 0) {
			float PredictedRotateSum = BH1.RotateSum + BH1.RotateSpeed * DeltaSeconds;
			UE_LOG(LogTemp, Log, TEXT("%f"), PredictedRotateSum);
			if (PredictedRotateSum >= 90) {
				Controller->SetControlRotation(Controller->GetControlRotation()+FRotator(0.f, (90 - BH1.RotateSum) * BH1.RotateDir, 0.f));
				BH1.RotateDir = 0;
				BH1.RotateSum = 0;
			}
			else {
				Controller->SetControlRotation(Controller->GetControlRotation()+FRotator(0.f, BH1.RotateSpeed * DeltaSeconds * BH1.RotateDir, 0.f));
				BH1.RotateSum += BH1.RotateSpeed * DeltaSeconds;
			}
		}
		//前に進む
		if (BH1.RotateDir == 0) {
			FVector ForwardVector = FRotationMatrix(FRotator(0, Controller->GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::X);
			AddMovementInput(ForwardVector, 1);
		}
		break;
	case 2:
		//ターボ終了
		if (BH1.IsTurbo) {
			BH1.TurboTimer += DeltaSeconds;
			if (BH1.TurboTimer >= BH1.TurboDuration) {
				GetCharacterMovement()->MaxWalkSpeed = BH1.MaxSpeed;
				BH1.TurboTimer = 0;
				BH1.IsTurbo = false;
			}
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
		//回転を開始する
		if (BH1.RotateDir == 0) {
			if (Value > 0)BH1.RotateDir = 1;
			else if (Value < 0)BH1.RotateDir = -1;
		}
		break;

	case 2:
		//左右に回転する


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
	case 2:
		if (!BH1.IsTurbo) {
			GetCharacterMovement()->MaxWalkSpeed = BH1.TurboSpeed;
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