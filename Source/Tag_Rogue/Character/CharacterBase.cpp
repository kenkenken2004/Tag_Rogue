// Copyright Epic Games, Inc. All Rights Reserved.

#include "CharacterBase.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Net/UnrealNetwork.h"
#include "Tag_Rogue/Interface/LimitCountComponent.h"
#include "Tag_Rogue/Interface/MiniMapComponent.h"

//////////////////////////////////////////////////////////////////////////
// ACharacterBase

ACharacterBase::ACharacterBase()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rate for input
	TurnRateGamepad = 50.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	
	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	MiniMap = CreateDefaultSubobject<UMiniMapComponent>(TEXT("MiniMap"));
	MiniMap->SetupAttachment(CameraBoom);
	MiniMap->SetRelativeLocation(FVector(200,0,-30));
	MiniMap->SetRelativeRotation(FRotator(0,270,0));
	MiniMap->SetRelativeScale3D(FVector(0.3,0.3,0.3));
	MiniMap->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	LimitCount = CreateDefaultSubobject<ULimitCountComponent>(TEXT("LimitCount"));
	LimitCount->SetupAttachment(CameraBoom);
	LimitCount->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	MiniMap->SetIsReplicated(true);
	LimitCount->SetIsReplicated(true);
	MiniMap->SetOnlyOwnerSee(true);
	LimitCount->SetOnlyOwnerSee(true);
	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

//////////////////////////////////////////////////////////////////////////
// Input

void ACharacterBase::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	//PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	//PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	//PlayerInputComponent->BindAxis("Move Forward / Backward", this, &ACharacterBase::MoveForward);
	//PlayerInputComponent->BindAxis("Move Right / Left", this, &ACharacterBase::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	//PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	//PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &ACharacterBase::TurnAtRate);
	//PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	//PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &ACharacterBase::LookUpAtRate);

	// handle touch devices
	//PlayerInputComponent->BindTouch(IE_Pressed, this, &ACharacterBase::TouchStarted);
	//PlayerInputComponent->BindTouch(IE_Released, this, &ACharacterBase::TouchStopped);
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

void ACharacterBase::BeginPlay()
{
	Super::BeginPlay();
	if(!HasAuthority())return;
	GameInstance = UTag_RogueGameInstance::GetInstance();
	MiniMap->InitializeByServer();
	MiniMap->Initialize();
	//MiniMap->SetRelativeLocation(FVector(100,0,-40));
	//MiniMap->SetRelativeRotation(FRotator(0,270,60));
	//MiniMap->SetRelativeScale3D(FVector(0.3,0.3,0.3));
	LimitCount->Initialize();
	LimitCount->UpdateNumbers();
}

void ACharacterBase::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	DeltaSecond = DeltaSeconds;
	TimeSinceCreated+=DeltaSeconds;
	if(!HasAuthority())return;
	
	float EnemyDistance = 1000000000;
	float EnemyRotation = 0;
	if(IsValid(Enemy))
	{
		const FVector RelativeLocation = Enemy->GetActorLocation() - GetActorLocation();
		EnemyDistance = RelativeLocation.Size();
		EnemyRotation = RelativeLocation.Rotation().Yaw - Controller->GetControlRotation().Yaw;
	}
	MiniMap->EnemyDirection = - EnemyRotation / 360;
	MiniMap->EnemyDistance = EnemyDistance;
	MiniMap->UpdateMapDirection();
	//MiniMap->AddRelativeLocation(FVector(0,0,3*DeltaSeconds*FMath::Cos(TimeSinceCreated/1.0*2*PI)));
	LimitCount->CheckShouldUpdateNumbers(DeltaSeconds);
	//LimitCount->AddRelativeLocation(FVector(0,0,3*DeltaSeconds*FMath::Cos(TimeSinceCreated/0.5*2*PI)));
}

void ACharacterBase::MoveForward_Implementation(const float Value)
{
	if ((Controller != nullptr) && (Value != 0.0f))
	{
		// find out which way is forward
		const FRotator Rotation = GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value*DeltaSecond*PawnMoveSpeed);

		// Acceleration
		if (PawnMoveSpeed<PawnMoveMaxSpeed)
		{
			PawnMoveSpeed+=DeltaSecond*PawnMoveAcceleration;
		}
		else
		{
			PawnMoveSpeed = PawnMoveMaxSpeed;
		}
	}else if(Value==0.0f)
	{
		PawnMoveSpeed=0;
	}
}

void ACharacterBase::MoveRight_Implementation(const float Value)
{
	if ( (Controller != nullptr) && (Value != 0.0f) )
	{
		// add movement in that direction
		AddControllerYawInput(Value*DeltaSecond*PawnRotateSpeed);

		// Acceleration
		if (PawnRotateSpeed<PawnRotateMaxSpeed)
		{
			PawnRotateSpeed+=DeltaSecond*PawnRotateAcceleration;
		}
		else
		{
			PawnRotateSpeed = PawnRotateMaxSpeed;
		}
	}else if(Value==0.0f)
	{
		PawnRotateSpeed=0;
	}
	
}
