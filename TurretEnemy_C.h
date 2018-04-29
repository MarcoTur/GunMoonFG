// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TurretEnemy_C.generated.h"

UCLASS()
class GUNMOON_API ATurretEnemy_C : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATurretEnemy_C();

	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BLueprintCallable, Category = "Cpp Function")
		FHitResult TurretFireSystem();
	UFUNCTION(BLueprintCallable, Category = "Cpp Function")
		void TurretComponentManagment();
	UFUNCTION(BLueprintCallable, Category = "Cpp Function")
		void EnemyDeathHandler();

	UPROPERTY(BlueprintReadWrite, Category = Turret)
		float TurretFireSpread = 35.0f;
	UPROPERTY(BlueprintReadWrite, Category = Turret)
		float EnemyHealth = 1;


	FORCEINLINE class USceneComponent* GetEnemyTurretBarrel() const { return EnemyTurretBarrel; }
private:
	UPROPERTY(VisibleAnywhere, Category = Weapon)
		class USceneComponent* EnemyTurretBarrel;


};
