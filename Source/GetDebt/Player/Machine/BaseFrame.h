// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Pawn.h"
#include "AngleFrame.h"
#include "GetDebtGameMode.h"
#include "Person/PersonCharacter.h"
#include "BaseFrame.generated.h"

UCLASS()
class GETDEBT_API ABaseFrame : public APawn
{
	GENERATED_BODY()

public:
	//PROPERTIES
    UPROPERTY(EditAnywhere, BlueprintReadWrite) UStaticMeshComponent* ClawMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UStaticMeshComponent* FrameMeshComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UBoxComponent* FrameCollider;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CustomProperties) USceneComponent* ClawCollider;
	//Camera
	UPROPERTY(EditAnywhere, BlueprintReadWrite) USpringArmComponent* SpringArm;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) UCameraComponent* MainCamera;
	//Wheels
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<UStaticMeshComponent*> Wheels;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) TArray<UPhysicsConstraintComponent*> Axles;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) int32 WheelCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FVector TorqueDirection;
	//Frame structure
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CustomProperties) TArray<AAngleFrame*> AngleFrameArray;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=CustomProperties) TArray<UPhysicsConstraintComponent*> PhysicsConstraintArray;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = CustomProperties) TArray<FName> ConstraintNameArray;
	//Capture area
	UPROPERTY(EditAnywhere, BlueprintReadWrite) USphereComponent* CaptureArea;
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite) USoundCue* CaptureSound;
    UPROPERTY(EditAnywhere, BlueprintReadWrite) UAudioComponent* EngineSound;

	// Sets default values for this pawn's properties
	ABaseFrame(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//When constructing instances of the base frame...
	virtual void OnConstruction(const FTransform& Transform) override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	//Input functions
	UFUNCTION() void Turn(float Value);
	UFUNCTION() void Steer(float Value);
	UFUNCTION() void Swivel(float Value);

	//Collision/Capture functions
	UFUNCTION(BlueprintCallable, Category=Capture)
	void OnBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	//Checking functions
	UFUNCTION(BlueprintCallable, Category = "Actions")
	bool PersonCaught(ABaseFrame* CatchingPlayer, APersonCharacter* CaughtPerson);

    //Camera
    UFUNCTION(BlueprintCallable, Category = Camera)
    void AddCameraZoom(float Val);
};
