// Fill out your copyright notice in the Description page of Project Settings.

#include "GetDebt.h"
#include "Person/PersonAIController.h"
#include "Person/PersonCharacter.h"
#include "IdleState.h"

UIdleState::UIdleState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	ChanceToWander = 20.0f;
    TimeToIdle = 3.0f;
    TimeInIdle = 0.0f;
}

void UIdleState::EnterState()
{
	//LOG("Enter IdleState");
    TimeInIdle = 0.0f;
}

void UIdleState::ExitState()
{
	//LOG("Exit IdleState");
}

void UIdleState::UpdateState(float DeltaSeconds)
{
	APersonCharacter* Pawn = CastChecked<APersonCharacter>(this->StateMachine->OwningController->GetPawn());

	if (Pawn->CanSeeMachine()) {
		//LOG("We've spotted the machine. RunAway!");
		this->StateMachine->SwitchState(EFSMState::EFSM_Hide);
	}
	else {

        if(TimeInIdle >= TimeToIdle) {
            float RandDiceRoll = FMath::FRandRange(0.0f, 100.0f);

            if(RandDiceRoll <= ChanceToWander) {
                //LOG("Let's wander instead!");
                this->StateMachine->SwitchState(EFSMState::EFSM_Wander);
            } else {
                TimeInIdle = 0.0f;
            }
        }
		else {
            TimeInIdle += DeltaSeconds;
			//LOG("We are idling");
			//TODO: Play Idle Animation
			//TODO: After Idle Animation finished, randomly decide to wander again
		}
	}
}