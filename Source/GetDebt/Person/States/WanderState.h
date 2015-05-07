// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Person/States/BaseState.h"
#include "WanderState.generated.h"

/**
 * 
 */
UCLASS()
class GETDEBT_API UWanderState : public UBaseState
{
	GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly, Category = WanderState)
    float ChanceToIdle;

    UWanderState(const FObjectInitializer& ObjectInitializer);

    void EnterState() override;

    void ExitState() override;

    /* Implement our own UpdateState to do things specific to our state every tick */
	void UpdateState(float DeltaSeconds) override;
	
	
};
