// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "AngleFrame.generated.h"

/**
 * 
 */
UCLASS()
class GETDEBT_API AAngleFrame : public APawn
{
	GENERATED_BODY()
public:

	//PROPERTIES
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Properties") UStaticMeshComponent* FrameMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Basic Properties") UBoxComponent* FrameCollider;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame Properties") float MinSwivelAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame Properties") float MaxSwivelAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame Properties") float CurrentSwivelAngle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Frame Properties") int32 NumberTag;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Frame Properties") FVector OldLocation;

	//FUNCTIONS
	AAngleFrame(const FObjectInitializer& ObjectInitializer);
	virtual void BeginPlay() override;
	virtual void OnConstruction(const FTransform& Transform);
	virtual void Tick(float Delta) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	//Input functions
	UFUNCTION(BlueprintCallable, Category="Swivel Actions") void PositiveSwivel(float Value);
	UFUNCTION(BlueprintCallable, Category = "Swivel Actions") void NegativeSwivel(float Value);

	//Other public functions
	UFUNCTION(BlueprintCallable, Category = "Others") void SetNumberTag(int32 Value);
	
};

