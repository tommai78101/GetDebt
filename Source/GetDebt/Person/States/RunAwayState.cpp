// Fill out your copyright notice in the Description page of Project Settings.

#include "GetDebt.h"
#include "../../Waypoints/Waypoint.h"
#include "../PersonAIController.h"
#include "Person/PersonCharacter.h"
#include "RunAwayState.h"

void URunAwayState::EnterState()
{
	//LOG("Enter RunAwayState");
    TimeSinceLastFarthestWaypointCalc = 0.0f;
}

void URunAwayState::ExitState()
{
	//LOG("Exit RunAwayState");
}

void URunAwayState::UpdateState(float DeltaSeconds){

	//TODO Check if machine is visible
	//If not visible, switch to Hide

	//Run away
    //LOG("We are running away!");
    if(this->StateMachine != nullptr && this->StateMachine->OwningController != nullptr) {
        APawn* PawnCheck = this->StateMachine->OwningController->GetPawn();
        if(PawnCheck != nullptr) {
            APersonCharacter* Pawn = CastChecked<APersonCharacter>(PawnCheck);
            if(Pawn != nullptr) {
                if(!Pawn->CanSeeMachine()) {
                    //LOG("We can no longer see the machine, time to hide!");
                    this->StateMachine->SwitchState(EFSMState::EFSM_Hide);
                    return;
                }
            }
        }

        if(TimeSinceLastFarthestWaypointCalc > 5.0f) {
            TimeSinceLastFarthestWaypointCalc = 0.0f;

            AWaypoint* FarthestWaypoint = this->StateMachine->OwningController->ClosestPlayerController->CalculateFarthestWaypoint();

            if(FarthestWaypoint != nullptr) {
                TArray<AWaypoint*> Path = this->StateMachine->OwningController->CalculatePathToTargetWaypoint(FarthestWaypoint);
                if(PawnCheck != nullptr) {
                    APersonCharacter* Pawn = CastChecked<APersonCharacter>(PawnCheck);
                    if(Pawn != nullptr) {
                        //Pawn->TargetLocation = FarthestWaypoint->GetActorLocation();
                        this->StateMachine->OwningController->CurrentPath = Path;
                    }
                }
            }
        } else {
            TimeSinceLastFarthestWaypointCalc += DeltaSeconds;
        }
    }

}
