// Fill out your copyright notice in the Description page of Project Settings.

#include "GetDebt.h"
#include "FiniteStateMachine.h"
#include "BaseState.generated.h"

#pragma once

//Note(Robert) This forward declaration is needed to prevent circular dependency with the headers
class AFiniteStateMachine;

/**
 * Base class for all states
 * Update should be called OnTick
 * Enter and Exit should be called as appropriate
 */

UCLASS(Blueprintable, BlueprintType)
class GETDEBT_API UBaseState : public UObject
{
    GENERATED_BODY()

public:
    /* Properties */

    //The StateMachine the state is in
    UPROPERTY(EditAnywhere)
    AFiniteStateMachine* StateMachine;

    /* Functions */
    UBaseState(const FObjectInitializer& ObjectInitializer);
	UBaseState(const FObjectInitializer& ObjectInitializer, AFiniteStateMachine* OurStateMachine);

    UFUNCTION(BlueprintCallable, Category = "State Machine")
    virtual void EnterState();

    UFUNCTION(BlueprintCallable, Category = "State Machine")
    virtual void UpdateState(float DeltaSeconds);

    UFUNCTION(BlueprintCallable, Category = "State Machine")
    virtual void ExitState();

	//Note(Thompson): Do we need a destructor?
	//virtual ~UBaseState();
};
