// Fill out your copyright notice in the Description page of Project Settings.

#include "GetDebt.h"
#include "Person/PersonCharacter.h"
#include "Person/PersonAIController.h"
#include "HideState.h"


UHideState::UHideState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    
    TimeSinceStartedHiding = 0.0f;
    TimeToWaitBeforeIdling = 10.0f;
}

UHideState::UHideState(const FObjectInitializer& ObjectInitializer, AFiniteStateMachine* OurStateMachine): UHideState(ObjectInitializer)
{
}


void UHideState::EnterState(){
    //LOG("Enter HideState");
    TimeSinceStartedHiding = 0.0f;
}

void UHideState::ExitState(){
	//LOG("Exit HideState");
}

void UHideState::UpdateState(float DeltaSeconds){

	//LOG("We are hiding!");
	APawn* Pawn = this->StateMachine->OwningController->GetPawn();

	if (Pawn != nullptr) {
		APersonCharacter* PersonPawn = CastChecked<APersonCharacter>(Pawn);

		//TODO: Check if we can see the machine
		//If we can, switch to RunAway
		//If we can't, see if X amount of time has elapsed and if true, switch to idle
		//Note: X is a time amount TBD later

		if (PersonPawn->CanSeeMachine()) {
			//LOG("We've spotted the machine. Back to running away.");
			this->StateMachine->SwitchState(EFSMState::EFSM_RunAway);
		} else {
			TimeSinceStartedHiding += DeltaSeconds;

			if (TimeSinceStartedHiding > TimeToWaitBeforeIdling) {
				//LOG("It's been " + FString::SanitizeFloat(TimeToWaitBeforeIdling) + " seconds so we are going back to idle.")
					this->StateMachine->SwitchState(EFSMState::EFSM_Idle);
			}
		}
	}
}