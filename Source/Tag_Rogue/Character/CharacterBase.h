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
//関数、構造体
//

	

public:
	ACharacterBase();
	
protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	void SetMesh(FString path);//メッシュをセットする　派生クラスから呼び出す

	void ReloadWalkSpeed();//歩く速さを編集したのを適用する

	//旧Third-human-4の動きをさせるときの変数
	//基本的にずっと前進している
	//左右ボタンを押すとその方向に90度回転する
	//ターボあり
	//回転時は立ち止まる
	struct Behaviour1 {
		float MaxSpeed = 300;
		float TurboSpeed = 500;
		float TurboDuration = 3;//ターボする長さ
		float TurboTimer = 0;//ターボしている長さ
		bool IsTurbo = false;;//今ターボしているか
		float RotateSpeed = 180;//回転する速さ(deg/s)
		float RotateSum = 0;//今回の一連の回転で、既に回った角度
		float RotateDir = 0;//回る方向(オイラー角Z正方向に回転するとき正)
	};

	//左右を押すと回転、前を押すと進む、後ろを押すと戻るという従来通りのやつ
	//回転に連動してカメラは動き、常にキャラクターを背後から撮影するようにする
	struct Behaviour2 {
		float MaxSpeed = 300;
		float TurboSpeed = 500;
		float TurboDuration = 3;//ターボする長さ
		float TurboTimer = 0;//ターボしている長さ
		bool IsTurbo = false;//今ターボしているか]
		float RotateSpeed = 1;//回転する速さを表す係数
	};

	//入力を受け取るやつ
	void MoveForward(float Value);

	void MoveRight(float Value);

	void Action1();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	
//
//変数
//
protected:
	//数字パラメーター
	const float ColliderHeight = 50.0f;
	const float ColliderDiameter = 25.0f;
	int BehaviourNumber = 1;//操作性の番号　各キーによる入力は、この番号によって自動的に分岐する
	FString Role;//ゲーム内での役割(ChaserかFugitive)

	//クラス内で共有する一時変数
	Behaviour1 BH1;
	Behaviour2 BH2;
	bool CanMove;//その時動ける状態か(ゲーム終了後などにfalse)
	
	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	

public:

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* RobotMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"), Replicated)
		class UMiniMapComponent* MiniMap;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"), Replicated)
		class ULimitCountComponent* LimitCount;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		APawn* Enemy = nullptr;

	float TimeSinceCreated = 0;

	UFUNCTION(BlueprintCallable)
	void SpawnRandom();
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty> &OutLifetimeProps)const override;
};

