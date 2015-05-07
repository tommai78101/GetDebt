// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
//#include "GameFramework/Character.h"
#include "Obstacle.generated.h"

UCLASS()
class GETDEBT_API AObstacle : public APawn
{
	GENERATED_BODY()

public:
	//PROPERTY
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Properties") UStaticMeshComponent* BodyMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Properties") USphereComponent* SphereCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force Based Steering") FVector Velocity;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force Based Steering") FVector Acceleration;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Force Based Steering") int32 TimerCountdown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Force Based Steering") int32 TimerInitialValue;

	//FUNCTION
	// Sets default values for this pawn's properties
	AObstacle(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	//Movement code logic
	UFUNCTION(BlueprintCallable, Category="Force Based Steering") virtual void Move();
	
};
