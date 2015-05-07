// Fill out your copyright notice in the Description page of Project Settings.

#include "GetDebt.h"
#include "Person/PersonAIController.h"
#include "Person/States/WanderState.h"
#include "Person/States/IdleState.h"
#include "Person/States/RunAwayState.h"
#include "Person/States/HideState.h"
#include "Person/States/BaseState.h"
#include "FiniteStateMachine.h"


// Sets default values
AFiniteStateMachine::AFiniteStateMachine(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Start on Wander
	this->CurrentState = EFSMState::EFSM_Idle;

	//A Map of States to Classes
	StateClassMap = TMap<EFSMState, UClass*>();
	StateClassMap.Add(EFSMState::EFSM_Wander, UWanderState::StaticClass());
	StateClassMap.Add(EFSMState::EFSM_Idle, UIdleState::StaticClass());
	StateClassMap.Add(EFSMState::EFSM_RunAway, URunAwayState::StaticClass());
	StateClassMap.Add(EFSMState::EFSM_Hide, UHideState::StaticClass());

	//First state is Wander
	this->CurrentStateObject = ObjectInitializer.CreateDefaultSubobject<UWanderState>(this, TEXT("CurrentState"));
	this->CurrentStateObject->StateMachine = this;
	//this->CurrentStateObject->EnterState(this);

	if (this->GetOwner() != nullptr){
		this->OwningController = CastChecked<APersonAIController>(this->GetOwner());
	}
}

// Called when the game starts or when spawned
void AFiniteStateMachine::BeginPlay()
{
	Super::BeginPlay();

	if (this->CurrentStateObject != nullptr){
		this->CurrentStateObject->EnterState();
	}
}

// Called every frame
void AFiniteStateMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (this->CurrentStateObject != nullptr){
		this->CurrentStateObject->UpdateState(DeltaTime);
		//if (this->CurrentStateObject->GetClass()->IsChildOf(UBaseState::StaticClass())){
		//	this->CurrentStateObject->UpdateState();
		//}
	}

}

void AFiniteStateMachine::SwitchState(EFSMState NewState){
	if (this->CurrentStateObject != nullptr){
		this->CurrentStateObject->ExitState();
	}
	UClass* NewStateClass = StateClassMap.FindChecked(NewState);
	UBaseState* NewStateObject = NewObject<UBaseState>(this, NewStateClass);
	NewStateObject->StateMachine = this;

	NewStateObject->EnterState();
	this->CurrentStateObject = NewStateObject;
}
