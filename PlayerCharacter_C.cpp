// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter_C.h"
#include "TurretEnemy_C.h"
#include "Terminal_C.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Classes/Components/InputComponent.h"
#include "Classes/Components/SceneComponent.h"
#include "Classes/GameFramework/Character.h"
#include "Classes/Components/SkeletalMeshComponent.h"
#include "Classes/Engine/World.h"
#include "Engine/Engine.h"
#include "Camera/CameraComponent.h"
#include "Public/DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
APlayerCharacter_C::APlayerCharacter_C()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CameraManager();
	WeaponManager();
}

// Called when the game starts or when spawned
void APlayerCharacter_C::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APlayerCharacter_C::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void APlayerCharacter_C::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis("MoveForward", this, &APlayerCharacter_C::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &APlayerCharacter_C::MoveRight);
	InputComponent->BindAxis("Turn", this, &APlayerCharacter_C::Turn);

	InputComponent->BindAction("RightWeapon", IE_Pressed, this, &APlayerCharacter_C::FireRightWeapon);
	InputComponent->BindAction("LeftWeapon", IE_Pressed, this, &APlayerCharacter_C::FireLeftWeapon);
	InputComponent->BindAction("ReloadRightWeapon", IE_Pressed, this, &APlayerCharacter_C::RWeaponReload);
	InputComponent->BindAction("ReloadLeftWeapon", IE_Pressed, this, &APlayerCharacter_C::LWeaponReload);
}

void APlayerCharacter_C::MoveForward(float Value)
{
	FVector ForwardDirection = GetPlayerCamera()->GetForwardVector() * 2;
	AddMovementInput(ForwardDirection, Value);
}

void APlayerCharacter_C::MoveRight(float Value)
{
	FVector StrafeDirection = GetPlayerCamera()->GetRightVector();
	AddMovementInput(StrafeDirection, Value);
}

void APlayerCharacter_C::Turn(float Value)
{
	AddControllerYawInput(Value);
}

void APlayerCharacter_C::FireRightWeapon()
{
	if (RWeaponMaxAmmo > 0)
	{
		RWeaponMaxAmmo--;
	}
}

void APlayerCharacter_C::RWeaponReload()
{
	RWeaponMaxAmmo = WeaponConfig.RightMaxAmmo; 
}

FHitResult APlayerCharacter_C::RightWeaponTrace()
{
	//presets the LineTrace variables
	FHitResult Hit, HitBounce;
	FVector TraceStart = RightWeaponBarrel->GetComponentLocation() + RightWeaponBarrel->GetForwardVector() * WeaponConfig.BarrelVForwardRange;
	FVector TraceEnd = RightWeaponBarrel->GetComponentLocation() + RightWeaponBarrel->GetForwardVector() * WeaponConfig.RightWeaponRange;

	GetWorld()->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECollisionChannel::ECC_WorldStatic, FCollisionQueryParams::DefaultQueryParam);

	//If LineTrace hits a surface it will bounce
	if (Hit.bBlockingHit)
	{
		DrawDebugLine(GetWorld(), Hit.TraceStart, Hit.ImpactPoint, FColor::Green, false, .5, 5.0f);
		GetWorld()->LineTraceSingleByChannel(HitBounce, Hit.ImpactPoint + Hit.Normal, (TraceEnd - TraceStart).MirrorByVector(Hit.ImpactNormal) + Hit.ImpactPoint,
			ECollisionChannel::ECC_WorldStatic, FCollisionQueryParams::DefaultQueryParam);
		//Checks who can be damaged by the lineTrace.
		ATurretEnemy_C* Turret = Cast<ATurretEnemy_C>(Hit.GetActor());
		if (Turret)
		{
			Turret->EnemyHealth--;
			Turret->EnemyDeathHandler();
		}
		ATerminal_C* Terminal = Cast<ATerminal_C>(Hit.GetActor());
		if (Terminal)
		{
			Terminal->TerminalHealth--;
			Terminal->TerminalManager();
		}

		if (HitBounce.bBlockingHit)	//If LineTrace hits another surface it will registrer it as hitting another surface but not bounce
		{
			DrawDebugLine(GetWorld(), HitBounce.TraceStart, HitBounce.Location, FColor::Green, false, .5, 12.0f);
			//Checks who can be damaged by the lineTrace.
			ATurretEnemy_C* Turret = Cast<ATurretEnemy_C>(HitBounce.GetActor());
			if (Turret)
			{
				Turret->EnemyHealth--;
				Turret->EnemyDeathHandler();
			}
			ATerminal_C* Terminal = Cast<ATerminal_C>(HitBounce.GetActor());
			if (Terminal)
			{
				Terminal->TerminalHealth--;
				Terminal->TerminalManager();
			}
		}
		else
		{
			DrawDebugLine(GetWorld(), HitBounce.TraceStart, HitBounce.TraceEnd, FColor::Red, false, .5, 12.0f);
		}
	}
	else
	{
		DrawDebugLine(GetWorld(), Hit.TraceStart, Hit.TraceEnd, FColor::Red, false, .5, 12.0f);
	}

	return Hit;
}

void APlayerCharacter_C::FireLeftWeapon()
{
	if (LWeaponMaxAmmo > 0)
	{
		LWeaponMaxAmmo--;
	}
}

void APlayerCharacter_C::LWeaponReload()
{
	LWeaponMaxAmmo = WeaponConfig.LeftMaxAmmo;
}

//creates a linetrace with a randomized spread in the shape of a cone.
FHitResult APlayerCharacter_C::LeftWeaponTrace()
{
	const int32 RandomSeed = FMath::Rand();
	FRandomStream WeaponRandomStream(RandomSeed);
	const float CurrentSpread = WeaponConfig.ShotgunSpread;
	const float SpreadCone = FMath::DegreesToRadians(WeaponConfig.ShotgunSpread * 0.5);
	FVector AimDir = LeftWeaponBarrel->GetComponentRotation().Vector();
	FVector TraceStart = LeftWeaponBarrel->GetComponentLocation() + LeftWeaponBarrel->GetForwardVector() * WeaponConfig.BarrelVForwardRange;
	FVector ShootDir = WeaponRandomStream.VRandCone(AimDir, SpreadCone, SpreadCone);
	FVector TraceEnd = TraceStart + ShootDir * WeaponConfig.LeftWeaponRange;
	FHitResult Impact(TraceStart, TraceEnd);
	
	FCollisionQueryParams TraceParam(FName(TEXT("WeaponTag")),true, Instigator);
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

void APlayerCharacter_C::CameraManager()
{
	//creating and initillizing springarm for the player camera.
	PlayerCameraBoom = CreateOptionalDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	PlayerCameraBoom->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepWorldTransform);
	PlayerCameraBoom->TargetArmLength = 1050.f;
	PlayerCameraBoom->bDoCollisionTest = false;
	PlayerCameraBoom->bAbsoluteRotation = true;

	//creating and initillizing player camera.
	PlayerCamera = CreateOptionalDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->AttachToComponent(PlayerCameraBoom, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	PlayerCamera->bUsePawnControlRotation = false;
}

void APlayerCharacter_C::WeaponManager()
{
	//Creates scene components to attach to skeleton mesh
	RightWeaponBarrel = CreateOptionalDefaultSubobject<USceneComponent>(TEXT("RightWeaponBarrel"));
	LeftWeaponBarrel = CreateOptionalDefaultSubobject<USceneComponent>(TEXT("LeftWeaponBarrel"));
}

