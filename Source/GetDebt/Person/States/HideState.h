// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Person/States/BaseState.h"
#include "HideState.generated.h"
/**
 * 
 */
UCLASS()
class GETDEBT_API UHideState : public UBaseState
{
    GENERATED_BODY()
public:

    UPROPERTY(VisibleAnywhere, Category = HideState)
    float TimeSinceStartedHiding;

    UPROPERTY(VisibleAnywhere, Category = HideState)
    float TimeToWaitBeforeIdling;

    UHideState(const FObjectInitializer& ObjectInitializer);
	UHideState(const FObjectInitializer& ObjectInitializer, AFiniteStateMachine* OurStateMachine);

    void EnterState() override;

    void ExitState() override;

    void UpdateState(float DeltaSeconds) override;
};
