// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "BaseWheel.generated.h"


//NOTE(Thompson): May want to consider using AFloatingPawn for non-gravity-affected movements.
UCLASS()
class GETDEBT_API ABaseWheel : public APawn
{
	GENERATED_BODY()
	
public:	
	//PROPERTIES
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=CustomProperties) UStaticMeshComponent* Donut;

	// Sets default values for this actor's properties
	ABaseWheel(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//Setup inputs
	//virtual void SetupInputComponent(UInputComponent* InputComponent) override;

	UFUNCTION() void Turn(float Value);
	UFUNCTION() void Steer(float Value);
};
