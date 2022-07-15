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
#include "Net/UnrealNetwork.h"
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

	//今のところよくわからん設定・要らなさそうな設定
	//よくわからん設定
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
	//要らなさそうな設定
	//マウスを動かすと視点が動くが、このときカメラしか動かさないようにする
	bUseControllerRotationPitch = true;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = true;

	//動いている方向を向かせる
	GetCharacterMovement()->bOrientRotationToMovement = false;
	// CameraBoomを設定
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(GetCapsuleComponent());
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// カメラを設定
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = true;

	MiniMap = CreateDefaultSubobject<UMiniMapComponent>(TEXT("MiniMap"));
	MiniMap->SetupAttachment(CameraBoom);
	MiniMap->SetRelativeLocation(FVector(200,0,-30));
	MiniMap->SetRelativeRotation(FRotator(0,270,0));
	MiniMap->SetRelativeScale3D(FVector(0.3,0.3,0.3));
	MiniMap->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	

	LimitCount = CreateDefaultSubobject<ULimitCountComponent>(TEXT("LimitCount"));
	LimitCount->SetupAttachment(CameraBoom);
	LimitCount->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	

	//Behaviourを初期設定　派生クラスから好きに番号を変えたり、BHnの中身を変えたりしてよい
	BehaviourNumber = 1;
	
	//派生クラスのタイミングで動き出せるように、一旦止める
	CanMove = false;

	MiniMap->SetIsReplicated(true);
	LimitCount->SetIsReplicated(true);
	MiniMap->SetOnlyOwnerSee(true);
	LimitCount->SetOnlyOwnerSee(true);
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)

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
	case 2:
		GetCharacterMovement()->MaxWalkSpeed = BH2.MaxSpeed;
		break;
	}
	
}

void ACharacterBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	//ホスト側キャラクターとクライアント側キャラクターとで、異なるキーをバインドする
	if (HasAuthority()) {//ホスト側
		UE_LOG(LogTemp, Error, TEXT("a"));
		PlayerInputComponent->BindAxis("Move Forward / Backward Host", this, &ACharacterBase::MoveForward);//前後の動きをバインド
		PlayerInputComponent->BindAxis("Move Right / Left Host", this, &ACharacterBase::MoveRight);
		PlayerInputComponent->BindAction("Action1 Host", IE_Pressed, this, &ACharacterBase::Action1);//アクション1ボタンが押されたらアクション1
	}
	else {//クライアント側
		PlayerInputComponent->BindAxis("Move Forward / Backward Client", this, &ACharacterBase::MoveForward);//前後の動きをバインド
		PlayerInputComponent->BindAxis("Move Right / Left Client", this, &ACharacterBase::MoveRight);
		PlayerInputComponent->BindAction("Action1 Client", IE_Pressed, this, &ACharacterBase::Action1);//アクション1ボタンが押されたらアクション1

	}

}

void ACharacterBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ACharacterBase, MiniMap);
	DOREPLIFETIME(ACharacterBase, LimitCount);
}

void ACharacterBase::SpawnRandom()
{
	UTag_RogueGameInstance* Instance = UTag_RogueGameInstance::GetInstance();
	Instance->InitializeMapBuilders();
	int32 X=0;int32 Y=0;
	while (Instance->MapGenerator->GetCell(Y,X)->Attribution==UMapGeneratorBase::EType::Wall)
	{
		X = FMath::RandRange(0,Instance->MapGenerator->MapWidth-1);
		Y = FMath::RandRange(0,Instance->MapGenerator->MapHeight-1);
	}
	SetActorLocation(Instance->TerrainMaker->Cie_Convert(Y,X,Instance->CellSize));
}



void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	if(!HasAuthority())return;
	

	MiniMap->InitializeByServer();
	MiniMap->Initialize();
	MiniMap->SetRelativeLocation(FVector(100,0,-40));
	MiniMap->SetRelativeRotation(FRotator(0,270,60));
	MiniMap->SetRelativeScale3D(FVector(0.3,0.3,0.3));
	LimitCount->Initialize();
	LimitCount->UpdateNumbers();
}

void ACharacterBase::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	TimeSinceCreated+=DeltaSeconds;
	if (!HasAuthority())return;

	//Behaviour
	switch (BehaviourNumber)
	{
	case 1:
		//ターボ終了
		if (BH1.IsTurbo) {
			BH1.TurboTimer += DeltaSeconds;
			if (BH1.TurboTimer >= BH1.TurboDuration) {
				GetCharacterMovement()->MaxWalkSpeed = BH1.MaxSpeed;
				BH1.TurboTimer = 0;
				BH1.IsTurbo = false;
			}
		}
		if (!CanMove)break;
		//回転する・回転終了
		if (BH1.RotateDir != 0) {
			float PredictedRotateSum = BH1.RotateSum + BH1.RotateSpeed * DeltaSeconds;
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

	//if(!HasAuthority())return;
	float EnemyDistance = 1000000000;
	float EnemyRotation = 0;
	if(IsValid(Enemy))
	{
		const FVector RelativeLocation = Enemy->GetActorLocation() - GetActorLocation();
		EnemyDistance = RelativeLocation.Size();
		EnemyRotation = RelativeLocation.Rotation().Yaw - Controller->GetControlRotation().Yaw;
		UE_LOG(LogTemp, Warning, TEXT("%f"),EnemyRotation);
	}
	MiniMap->EnemyDirection = - EnemyRotation / 360;
	MiniMap->EnemyDistance = EnemyDistance;
	MiniMap->UpdateMapDirection();
	MiniMap->AddRelativeLocation(FVector(0,0,3*DeltaSeconds*FMath::Cos(TimeSinceCreated/1.0*2*PI)));
	LimitCount->CheckShouldUpdateNumbers(DeltaSeconds);
	LimitCount->AddRelativeLocation(FVector(0,0,3*DeltaSeconds*FMath::Cos(TimeSinceCreated/0.5*2*PI)));

}

void ACharacterBase::MoveForward(const float Value)
{
	switch (BehaviourNumber) {
	case 1:
		break;
	case 2:
		if (!CanMove)break;
		//前に進む
		FVector ForwardVector = FRotationMatrix(FRotator(0, Controller->GetControlRotation().Yaw, 0)).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardVector, Value);
		break;
	}

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
		if (!CanMove)break;
		//回転を開始する
		if (BH1.RotateDir == 0) {
			if (Value > 0)BH1.RotateDir = 1;
			else if (Value < 0)BH1.RotateDir = -1;
		}
		break;

	case 2:
		if (!CanMove)break;
		//左右に回転する
		Controller->SetControlRotation(Controller->GetControlRotation() + FRotator(0.f, BH2.RotateSpeed * Value, 0.f));

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