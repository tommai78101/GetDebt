// Fill out your copyright notice in the Description page of Project Settings.

#include "GetDebt.h"
#include "FiniteStateMachine.h"
#include "WanderState.h"
#include "BaseState.h"

UBaseState::UBaseState(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
    //Note(Robert) This is how we can get access to the parent actor, StateMachine->Parent
	//FIXME(Thompson): CrashReportClient.exe: Cast of Package //Script//GetDebt to FiniteStateMachine failed
	//StateMachine = CastChecked<AFiniteStateMachine>(this->GetOuter());
}

UBaseState::UBaseState(const FObjectInitializer& ObjectInitializer, AFiniteStateMachine* OurStateMachine) : UBaseState(ObjectInitializer) {
    //Note(Robert) This is how we can get access to the parent actor, StateMachine->Parent
    //FIXME(Thompson): CrashReportClient.exe: Cast of Package //Script//GetDebt to FiniteStateMachine failed
    //StateMachine = CastChecked<AFiniteStateMachine>(this->GetOuter());

    this->StateMachine = OurStateMachine;

    //Call other constructor
    //this(ObjectInitializer);
}

//Note(Thompson): Do we need a destructor?
//UBaseState::~UBaseState()
//{
//}

void UBaseState::EnterState()
{
	//this->StateMachine = StateMachine;
}

void UBaseState::UpdateState(float DeltaSeconds)
{
    check(0 && "UBaseState::UpdateState needs to be overriden");
}

void UBaseState::ExitState()
{
	this->StateMachine = nullptr;
}