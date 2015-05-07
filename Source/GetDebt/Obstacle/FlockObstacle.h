// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Obstacle/Obstacle.h"
#include "FlockObstacle.generated.h"

UENUM()
namespace EActionType {
	enum Type {
		Seek	UMETA(DisplayName = "Seek"),
	};
}



/**
 *
 */
UCLASS()
class GETDEBT_API AFlockObstacle : public AObstacle
{
	GENERATED_BODY()
public:
	//Properties
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Properties") USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Properties") FVector TargetOffset;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Properties") float ForceStrength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Properties") float MaxSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Properties") uint16 BitwiseFlag;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Properties") float FlockingRadius;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Custom Properties") TArray<AFlockObstacle*> FlockObstaclesArray;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Properties") uint16 CountDown;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Properties") FVector CohesionCenter;
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Properties") UArrowComponent* DebugArrow;
	
	//Properties that are not attached to parent
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Custom Properties") FRotator FlatRotation;

	const uint16 COUNTDOWN_VALUE = 10;

//-------------------------------------------------------------------------------------------------------------
	//Functions
	AFlockObstacle(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay();

	virtual void Tick(float Delta);

	void SetFlag(uint16 EnumFlag);

//-------------------------------------------------------------------------------------------------------------	
	//Steering behaviors
	void Move() override;

	UFUNCTION() void MoveUpdate(float Delta);

	UFUNCTION() virtual FVector Seek(FVector Target);

	UFUNCTION() virtual FVector Flee(FVector Target);
	
	UFUNCTION() virtual FVector Arrive(FVector Target);
	
	UFUNCTION() virtual FVector Pursue(AFlockObstacle* Obstacle, FVector Target);
	
	UFUNCTION() virtual FVector Evade(AFlockObstacle* Obstacle, FVector Target);
	
	UFUNCTION() virtual void Hide();

	UFUNCTION() virtual FVector Interpose();

	UFUNCTION() virtual FVector Wander();

	UFUNCTION() virtual void AvoidObstacle();

	UFUNCTION() virtual void AvoidObstacles();

	//Flocking
	UFUNCTION() virtual FVector Cohesion();

	UFUNCTION() virtual FVector Cohesion2(FVector Target);

	UFUNCTION() virtual FVector Separation();

	UFUNCTION() virtual FRotator Alignment();

	UFUNCTION() virtual void SetFlockingRadius(float Radius);

//-------------------------------------------------------------------------------------------------------------	
	//Helper functions
	UFUNCTION() bool CheckSimulate();

	UFUNCTION() bool CheckAir();

	UFUNCTION() bool CheckObstacleDistance(AFlockObstacle* Obstacle);

	UFUNCTION() void UpdateObstaclesArray();
};
