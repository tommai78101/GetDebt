// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Info.h"
#include "Waypoint.h"
#include "WaypointManager.generated.h"

/**
 * 
 */
UCLASS()
class GETDEBT_API AWaypointManager : public AInfo
{
public:

	GENERATED_BODY()

	/* Constructor */
	AWaypointManager(const FObjectInitializer& ObjectInitializer);
	
	///* A list of all the Waypoints in the map, generated at runtime */
	//UPROPERTY(VisibleAnywhere, Category = Waypoints)
	//	TArray<AWaypoint*> ListOfWaypoints;
	
	void BeginPlay() override;
    void Tick(float DeltaTime) override;

    UPROPERTY(VisibleAnywhere, Category = Waypoints)
    float TimeSinceWaypointUpdate = 0.0f;
};
