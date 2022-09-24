// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Tag_Rogue/Tag_RogueGameInstance.h"
#include "CharacterBase.generated.h"

UCLASS(Config=Game)
class ACharacterBase : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	

public:
	ACharacterBase();
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Input)
	float TurnRateGamepad;
	float TimeSinceCreated = 0;
	float DeltaSecond = 0;

	float PawnMoveSpeed = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PawnMoveMaxSpeed = 700;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PawnMoveAcceleration = 250;

	float PawnRotateSpeed = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PawnRotateMaxSpeed = 60;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PawnRotateAcceleration = 20;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	ACharacterBase* Enemy = nullptr;
	UPROPERTY()
	UTag_RogueGameInstance* GameInstance;
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	/** Called for forwards/backward input */
	UFUNCTION(BlueprintCallable, Client,Reliable)
	void MoveForward(float Value);

	/** Called for side to side input */
	UFUNCTION(BlueprintCallable, Client, Reliable)
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	UFUNCTION(BlueprintCallable)
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	UFUNCTION(BlueprintCallable)
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);

protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	UFUNCTION(BlueprintCallable)
	void SpawnRandom();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps)const override;
};

