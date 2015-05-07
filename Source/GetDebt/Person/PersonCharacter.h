// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "PersonCharacter.generated.h"

UCLASS()
class GETDEBT_API APersonCharacter : public APawn
{
	GENERATED_BODY()

		//Temp property, do not rely or use this!
		UPROPERTY() class APersonAIController* TempController;

public:
	// Sets default values for this character's properties
	//APersonCharacter();
	//
	/*UPROPERTY(EditDefaultsOnly)
		USkeletalMeshComponent* MeshComponent;*/

	////Note(Robert) Placeholder mesh
	//UPROPERTY(EditDefaultsOnly) UStaticMeshComponent* SkeletalMesh;

	/* AI Component to use when detecting a player */
	UPROPERTY(EditDefaultsOnly)
		UPawnSensingComponent* SensingComponent;

	/* New collision component */
	UPROPERTY(EditDefaultsOnly)
		UBoxComponent* BoxCollision;

	/* Arrow to tell which way is forward for the person */
	UPROPERTY(EditDefaultsOnly)
		UArrowComponent* DebugArrow;

	UPROPERTY(EditDefaultsOnly)
		float MaxMoveSpeed;

	UPROPERTY(VisibleAnywhere)
		FVector TargetLocation;

	/* Function to determine if we can see a player or machine in front of us. Expensive? */
	UFUNCTION(BlueprintCallable, Category = "Machine")
		bool CanSeeMachine();

	UFUNCTION(BlueprintCallable, Category = "Conditions")
		bool IsPersonInRangeToPlayer(ABaseFrame* HuntingPlayer, APersonCharacter* HuntedPerson, float Radius);

	UFUNCTION(BlueprintCallable, Category = Movement)
		void MoveTowardsTarget(FVector NewTargetLocation);

	UFUNCTION(BlueprintCallable, Category = Movement)
		void UpdateMovement(float DeltaTime);

	UFUNCTION(BlueprintCallable, Category = Movement)
		bool IsAtTargetLocation();

	//Constructor
	APersonCharacter(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;
};
