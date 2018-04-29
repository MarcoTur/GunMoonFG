// Fill out your copyright notice in the Description page of Project Settings.

#include "Terminal_C.h"


// Sets default values
ATerminal_C::ATerminal_C()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATerminal_C::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATerminal_C::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATerminal_C::TerminalManager_Implementation()
{
	if (TerminalHealth <= 0)
	{

	}
}

