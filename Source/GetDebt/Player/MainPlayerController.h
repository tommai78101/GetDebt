// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/PlayerController.h"
//#include "Machine/MainCore.h"
#include "Machine/BaseFrame.h"
//#include "Waypoints/Waypoint.h"
#include "MainPlayerController.generated.h"

//Forward declaration to avoid circular dependency
class AWaypoint;

/**
 * 
 */
UCLASS()
class GETDEBT_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	//Properties
	/*UPROPERTY(VisibleAnywhere, Category=CustomProperties) 
    AMainCore* MainCorePawn;*/

	UPROPERTY(VisibleAnywhere, Category=CustomProperties)
	ABaseFrame* MainBaseFrame;

	UPROPERTY(VisibleAnywhere, Category=CustomProperties)
	TArray<AWaypoint*> WaypointsArray;

    UPROPERTY(VisibleAnywhere, Category=Waypoints)
    AWaypoint* FarthestWaypoint;

    UPROPERTY(VisibleAnywhere, Category=Waypoints)
    float DistanceToFarthestWaypoint;

    UPROPERTY(EditDefaultsOnly, Category = Camera)
    float MaxYaw = 90.0f;

    UPROPERTY(EditDefaultsOnly, Category = Camera)
    float MinYaw = -90.0f;

    UPROPERTY(EditDefaultsOnly, Category = Camera)
    float MinPitch = -45.0f;

    UPROPERTY(EditDefaultsOnly, Category = Camera)
    float MaxPitch = -15.0f;

    UPROPERTY(EditDefaultsOnly, Category = Camera)
    float MinCameraOffset = -15.0f;

    UPROPERTY(EditDefaultsOnly, Category = Camera)
    float MaxCameraOffset = 15.0f;

    UPROPERTY(EditDefaultsOnly, Category = Camera)
    float CameraAdjustSpeed = 15.0f;

	//Constructor
	AMainPlayerController(const FObjectInitializer& ObjectInitializer);
	
	//Tick
	virtual void Tick(float Delta) override;

	//BeginPlay
	virtual void BeginPlay() override;

	//Overriding SetupInputComponent
	void SetupInputComponent() override;

	//Game over
	UFUNCTION(BlueprintImplementableEvent, Category = GameEnd)
		void EndGame();

	//IMPORTANT(Thompson): We need to calculate the farthest point in the map.
	UFUNCTION() AWaypoint* CalculateFarthestWaypoint();
	//UFUNCTION(BlueprintImplementableEvent, Category = "Waypoints") bool CalculateFarthestWaypoint(TArray<AWaypoint> Waypoints);

};