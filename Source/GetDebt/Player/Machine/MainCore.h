// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
//#include "../MainPlayerController.h"
#include "MainCore.generated.h"

UCLASS()
class GETDEBT_API AMainCore : public ADefaultPawn
{
	GENERATED_BODY()
	
    //Note(Thompson): Everything will be public for the sake of easier debugging and programming. 
public:	

	//PROPERTIES
	//Camera components and other required stuffs.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=CustomCameraProperties) USpringArmComponent* CameraSpringArm;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=CustomCameraProperties) UCameraComponent* CameraMainView;

	//FUNCTIONS
	// Sets default values for this actor's properties
	AMainCore(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	//Mapping inputs
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	//Axis movement functions
	void MoveForward(float Value) override;
    void MoveRight(float Value) override;

	//Blueprint Implementable Events
	UFUNCTION(BlueprintImplementableEvent, Category = CustomFunctions, meta=(FriendlyName=CustomTestFunction)) 
    virtual void TickFunction();
	
	//Custom functions
	void ShowMessage();
};
