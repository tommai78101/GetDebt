// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

/*
Info about match states:

The GameMode contains a state machine that tracks the state of the Match, or the general gameplay flow. To query the current state, you can use GetMatchState, or wrappers like HasMatchStarted, IsMatchInProgress, and HasMatchEnded. Here are the possible Match states:
•EnteringMap is the initial state. Actors are not yet ticking and the world has not been fully initialized. It will transition to the next state when things are fully loaded.
•WaitingToStart is the next state, and HandleMatchIsWaitingToStart is called when entering. Actors are ticking, but players have not yet spawned in. It transitions to the next state if ReadyToStartMatch returns true, or if someone calls StartMatch.
•InProgress is the next state, and HandleMatchHasStarted is called when entering, which calls BeginPlay on all Actors. Normal gameplay is in progress. It transitions to the next state if ReadyToEndMatch returns true, or if someone calls EndMatch.
•WaitingPostMatch is the next state, and HandleMatchHasEnded is called when entering. Actors are still ticking but new players are not accepted. It transitions to the next state when a map transfer starts.
•LeavingMap is the last state, and HandleLeavingMap is called when entering. The match stays in this state while transfering to a new map.
•Aborted is the failure state, started from AbortMatch. This is set when there is an unrecoverable error.
By default, the Match state will almost always be InProgress. But, individual games can override this behavior to build a multiplayer game with more complicated rules.

*/




#pragma once
#include "GameFramework/GameMode.h"
//#include "Person/PersonCharacter.h"
//#include "Player/Machine/BaseFrame.h"
#include "GetDebtGameMode.generated.h"

UCLASS(minimalapi)
class AGetDebtGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	//Properties
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Score)
		int32 PlayerOneScore;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Score)
		int32 PlayerTwoScore;

	//Game level properties
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game Level Stuff") 
	class ABaseFrame* MainBaseFrame;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game Level Stuff")
	TArray<class APersonCharacter*> PeopleArray;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game Level Stuff")
		TArray<class AWaypoint*> WaypointsArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flocking")
		TArray<class AFlockObstacle*> FlockingObstaclesArray;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Game Mode Timer")
		float TimeForLevelInSeconds;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game Mode Timer")
		float TimeRemaining;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game Mode Timer")
		bool TimeLimitHit;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Game Mode")
		int32 RemainingPeople;

	AGetDebtGameMode(const FObjectInitializer& ObjectInitializer);

	//BeginPlay stuff.
	virtual void StartPlay() override;

	//Tick
	virtual void Tick(float DeltaSeconds) override;

	//Initializing function.
	UFUNCTION(BlueprintCallable, Category = "Game Level Stuff")
		void InitializeEnteringMap();

	UFUNCTION(BlueprintCallable, Category = "Game Level Stuff")
		void InitializeWaitingToStart();

	//Game over
	UFUNCTION(BlueprintCallable, Category = "Game End")
		void EndGame();

	UFUNCTION(BlueprintCallable, Category = "Game End")
		void TimeForLevelUp();

};



