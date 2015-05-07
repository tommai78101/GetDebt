// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "GameFramework/Actor.h"
#include "Player/MainPlayerController.h"
#include "GameFramework/Info.h"
#include "Waypoint.generated.h"

/**
 * A waypoint representing a node that the AI uses to do A* Star.
 */
UCLASS()
class GETDEBT_API AWaypoint : public AInfo
{
	GENERATED_BODY()

public:
    /* A list of waypoints that are available if you are at this waypoint */
    UPROPERTY(EditAnywhere, Category = Waypoints)
        TArray<AWaypoint*> AccessibleWaypoints;

    /* Time since last update to prevent updating too often */
     //UPROPERTY(VisibleAnywhere, Category = Waypoints)
        //float static TimeSinceLastUpdateForGivenValue;

#ifdef DEBUG
    UPROPERTY(EditDefaultsOnly, Category = Debug)
    UStaticMeshComponent* WaypointSphere;
#endif

    /* Max HeuristicCost */
    //UPROPERTY(VisibleAnywhere, Category = Waypoints)
	static float MaxHeuristicCost;

    UPROPERTY(EditAnywhere, Category = Waypoints)
        float Priority;

    /* Heuristic cost to use for A* Star, which ranges from MaxGivenCost to 0.0f, depending how close a player is. The higher the number, the closer a player is to the waypoint. */
    UPROPERTY(EditAnywhere, Category = Waypoints)
        float HeuristicCost = 0.0f;

	// Sets default values for this actor's properties
	AWaypoint(const FObjectInitializer& ObjectInitializer);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

    UFUNCTION(BlueprintCallable, Category = Waypoints)
    static void UpdateGivenValueForAllWaypoints(TArray<AMainPlayerController*> PlayerControllers, UWorld* World);
};

 struct WaypointPriorityPredicate
 {
 public: 
     bool operator()(const AWaypoint& A, const AWaypoint& B) const
     {
         // lower priorities float to the top
         return A.Priority < B.Priority;
     }
 };