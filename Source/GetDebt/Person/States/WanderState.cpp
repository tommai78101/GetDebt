// Fill out your copyright notice in the Description page of Project Settings.

#include "GetDebt.h"
#include "Person/PersonAIController.h"
#include "Person/States/FiniteStateMachine.h"
#include "Person/PersonCharacter.h"
#include "WanderState.h"

UWanderState::UWanderState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	ChanceToIdle = 50.0f;
}

void UWanderState::EnterState()
{
	//LOG("Enter WanderState");
}

void UWanderState::ExitState() {
	//LOG("Exit WanderState");
}

void UWanderState::UpdateState(float DeltaSeconds)
{
	APersonCharacter* Pawn = CastChecked<APersonCharacter>(this->StateMachine->OwningController->GetPawn());

	if (Pawn->CanSeeMachine()) {
		//LOG("We've spotted the machine. RunAway!");
		this->StateMachine->SwitchState(EFSMState::EFSM_RunAway);
	}
	else {
		if (Pawn->IsAtTargetLocation()) {

			float RandDiceRoll = FMath::FRandRange(0.0f, 100.0f);

			if (RandDiceRoll <= ChanceToIdle) {
				//LOG("Let's idle instead!");
				this->StateMachine->SwitchState(EFSMState::EFSM_Idle);
			}
			else {
				//LOG("Picking new Wander location");

				//Wander to a nearby waypoint randomly
				AWaypoint* ClosestWaypoint = this->StateMachine->OwningController->FindClosestWaypoint(); //Start

				int32 Index = FMath::RandRange(0, ClosestWaypoint->AccessibleWaypoints.Num() - 1); //RandRange is Inclusive
				AWaypoint* TargetWaypoint = ClosestWaypoint->AccessibleWaypoints[Index]; //Random Target

				//this->StateMachine->OwningController->CalculatePathToTargetWaypoint(TargetWaypoint);
				TArray<AWaypoint*> Path = this->StateMachine->OwningController->CalculatePathToTargetWaypoint(TargetWaypoint);
				this->StateMachine->OwningController->CurrentPath = Path; //Move To Target
				//this->StateMachine->OwningController->Move
			}
		}
	}
}

