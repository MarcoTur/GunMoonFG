// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter_C.generated.h"

USTRUCT()
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()
		//Both Weapon
	UPROPERTY(EditDefaultsOnly, Category = BothWeapon)
		float BarrelVForwardRange = 15.0f;

	//Right weapon aka RW
	UPROPERTY(EditDefaultsOnly, Category = RightWeapon)
		float RightWeaponRange = 2000.0f;
	UPROPERTY(EditDefaultsOnly, Category = LeftWeapon)
		int32 RightMaxAmmo = 6;

	//Left weapon aka LW
	UPROPERTY(EditDefaultsOnly, Category = LeftWeapon)
		float LeftWeaponRange = 260.0f;
	UPROPERTY(EditDefaultsOnly, Category = LeftWeapon)
		float ShotgunSpread = 36.0f;
	UPROPERTY(EditDefaultsOnly, Category = LeftWeapon)
		int32 LeftMaxAmmo = 3;
};

UCLASS()
class GUNMOON_API APlayerCharacter_C : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter_C();

	

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Movement Functions
	UFUNCTION(BlueprintCallable, Category = "Cpp Function")
		void MoveForward(float Value);
	UFUNCTION(BlueprintCallable, Category = "Cpp Function")
		void MoveRight(float Value);
	UFUNCTION(BlueprintCallable, Category = "Cpp Function")
		void Turn(float Value);

	//This the functions for the both weapons 
	UFUNCTION(BlueprintCallable, Category = "Cpp Function")
		void FireRightWeapon();
	UFUNCTION(BlueprintCallable, Category = "Cpp Function")
		FHitResult RightWeaponTrace();
	UFUNCTION(BlueprintCallable, Category = "Cpp Function")
		void FireLeftWeapon();
	UFUNCTION(BlueprintCallable, Category = "Cpp Function")
		FHitResult LeftWeaponTrace();
	UFUNCTION(BlueprintCallable, Category = "Cpp Function")
		void RWeaponReload();
	UFUNCTION(BlueprintCallable, Category = "Cpp Function")
		void LWeaponReload();

	//Creating and instansiating the variables for the SpringArm and Camera
	UFUNCTION(BlueprintCallable, Category = "Cpp Function")
		void CameraManager();

	//This is where the Component are created and attached to the sockets.
	UFUNCTION(BlueprintCallable, Category = "Cpp Function")
		void WeaponManager();

	UPROPERTY(EditDefaultsOnly)
		FWeaponData WeaponConfig;

	//Blueprint variables
	UPROPERTY(BlueprintReadWrite, Category = Weapon)
		int32 RWeaponMaxAmmo = WeaponConfig.RightMaxAmmo;
	UPROPERTY(BlueprintReadWrite, Category = Weapon)
		int32 LWeaponMaxAmmo = WeaponConfig.LeftMaxAmmo;
	UPROPERTY(BlueprintReadWrite, Category = Weapon)
		int32 ShotgunSpreadBP = WeaponConfig.ShotgunSpread;

	//Get component functions
	FORCEINLINE class UCameraComponent* GetPlayerCamera() const { return PlayerCamera; }
	FORCEINLINE class USpringArmComponent* GetPlayerCameraBoom() const { return PlayerCameraBoom; }
	FORCEINLINE class USceneComponent* GetRightWeaponBarrel() const { return RightWeaponBarrel; }
	FORCEINLINE class USceneComponent* GetLeftWeaponBarrel() const { return LeftWeaponBarrel; }
private:
	//Instasiating Components
	UPROPERTY(VisibleAnywhere, Category = "Camera")
		class UCameraComponent* PlayerCamera;
	UPROPERTY(VisibleAnywhere, Category = "Camera")
		class USpringArmComponent* PlayerCameraBoom;

	//Instasiating the Scene Components
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		class USceneComponent* RightWeaponBarrel;
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		class USceneComponent* LeftWeaponBarrel;

	//Delay timer vars
	UPROPERTY(VisibleAnywhere, Category = TimeHandle)
		FTimerHandle ReloadTimehandler;
	UPROPERTY(VisibleAnywhere, Category = TimeHandle)
		bool IsReloading = true;
	UPROPERTY(VisibleAnywhere, Category = TimeHandle)
		float ReloadTime = 5.0f;
};