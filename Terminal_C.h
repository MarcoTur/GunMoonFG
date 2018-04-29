// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Terminal_C.generated.h"

UCLASS()
class GUNMOON_API ATerminal_C : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATerminal_C();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//Creates a Custom Event that can be used in Blueprint
	UFUNCTION(BlueprintNativeEvent, Category = "Cpp Function")
		void TerminalManager();
		void TerminalManager_Implementation();

	UPROPERTY(BlueprintReadWrite, Category = Terminal)
		float TerminalHealth = 1.0f;
	
	
};
