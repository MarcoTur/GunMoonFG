// Fill out your copyright notice in the Description page of Project Settings.

#include "TurretEnemy_C.h"
#include "Classes/GameFramework/Character.h"
#include "Classes/Components/SkeletalMeshComponent.h"
#include "Classes/Components/SceneComponent.h"
#include "Engine/Engine.h"
#include "Kismet/KismetMathLibrary.h"
#include "Public/DrawDebugHelpers.h"

// Sets default values
ATurretEnemy_C::ATurretEnemy_C()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TurretComponentManagment();
}

// Called when the game starts or when spawned
void ATurretEnemy_C::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATurretEnemy_C::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATurretEnemy_C::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

FHitResult ATurretEnemy_C::TurretFireSystem()
{
	const int32 RandomSeed = FMath::Rand();
	FRandomStream WeaponRandomStream(RandomSeed);
	const float CurrentSpread = TurretFireSpread;
	const float SpreadCone = FMath::DegreesToRadians(TurretFireSpread * 0.5);
	FVector AimDir = EnemyTurretBarrel->GetComponentRotation().Vector();
	FVector TraceStart = EnemyTurretBarrel->GetComponentLocation() + EnemyTurretBarrel->GetForwardVector() * 15;
	FVector ShootDir = WeaponRandomStream.VRandCone(AimDir, SpreadCone, SpreadCone);
	FVector TraceEnd = TraceStart + ShootDir * 600;
	FHitResult Impact(TraceStart, TraceEnd);

	FCollisionQueryParams TraceParam(FName(TEXT("WeaponTag")), true, Instigator);
	TraceParam.bTraceAsyncScene = true;
	TraceParam.bReturnPhysicalMaterial = true;
	TraceParam.AddIgnoredActor(this);

	FHitResult Hit(ForceInit);
	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECC_WorldStatic, TraceParam);

	if (Hit.bBlockingHit)
	{
		DrawDebugLine(GetWorld(), TraceStart, Hit.ImpactPoint, FColor::Red, false, .5, 10, 5);
	}
	else
	{
		DrawDebugLine(GetWorld(), TraceStart, Hit.TraceEnd, FColor::Red, false, .5, 10, 5);
	}

	return Hit;
}

void ATurretEnemy_C::TurretComponentManagment()
{
	EnemyTurretBarrel = CreateOptionalDefaultSubobject<USceneComponent>(TEXT("EnemyTurretBarrel"));
	EnemyTurretBarrel->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("ult_weapon_l_Barrel"));
}

void ATurretEnemy_C::EnemyDeathHandler()
{
	if (EnemyHealth <= 0)
	{
		Destroy();
	}
}

