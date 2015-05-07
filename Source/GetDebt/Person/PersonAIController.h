// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "Person/States/FiniteStateMachine.h"
#include "Waypoints/Waypoint.h"
#include "Player/MainPlayerController.h"
#include "PersonAIController.generated.h"


//
//inline bool HighLowPredicate(const AWaypoint* A, const AWaypoint* B){
//	return ((A->GivenCost + A->HeuristicCost) > (B->GivenCost + B->HeuristicCost));
//}


/**
 *
 */
UCLASS()
class GETDEBT_API APersonAIController : public AAIController
{
	GENERATED_BODY()

public:
	//CONSTRUCTOR
	APersonAIController(const FObjectInitializer& ObjectInitializer);


	//PROPERTIES
	/* Our StateMachine */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "AI")
		AFiniteStateMachine* StateMachine;

	/* The closest AMainPlayerController */
	UPROPERTY(VisibleAnywhere, Category = "AI | Player Controller")
		AMainPlayerController* ClosestPlayerController;

	UPROPERTY(VisibleAnywhere, Category = "AI | Player Controller")
		AMainPlayerController* OtherPlayerController;

	UPROPERTY(VisibleAnywhere, Category = "AI | Navigation")
		TArray<AWaypoint*> CurrentPath;

	//UPROPERTY(VisibleAnywhere, Category = "AI|Navigation")
	//AWaypoint* TargetWaypoint;

	/* Tick count so we can selectively do updates every so often */
	float TimeSinceLastControllerUpdate = 0;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;


	//FUNCTIONS

	/* Find the closest AMainPlayerController*/
	UFUNCTION(BlueprintCallable, Category = "AI")
		void UpdateClosestPlayerController();

	/* Find the closest waypoint to ourselves */
	UFUNCTION(BlueprintCallable, Category = "AI | Navigation")
		AWaypoint* FindClosestWaypoint();

	/* Find the most desirable waypoint using A* Star */
	UFUNCTION(BlueprintCallable, Category = "AI | Navigation")
		TArray<AWaypoint*> CalculatePathToTargetWaypoint(AWaypoint* TargetWaypoint);

	/* Apply enough force to move towards the waypoint */
	UFUNCTION(BlueprintCallable, Category = "AI | Navigation")
		void TravelTowardsWaypoint(AWaypoint* Waypoint);


};
