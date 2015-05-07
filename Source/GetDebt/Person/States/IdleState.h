// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Person/States/BaseState.h"
#include "IdleState.generated.h"
/**
 * 
 */
UCLASS()
class GETDEBT_API UIdleState : public UBaseState
{
    GENERATED_BODY()
public:
    UPROPERTY(EditDefaultsOnly, Category = IdleState)
    float ChanceToWander;
    
    UPROPERTY(EditDefaultsOnly, Category = IdleState)
    float TimeToIdle;

    UPROPERTY(VisibleAnywhere, Category = IdleState)
    float TimeInIdle;

    UIdleState(const FObjectInitializer& ObjectInitializer);

    void EnterState() override;

    void ExitState() override;

    void UpdateState(float DeltaSeconds) override;
};
