// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "Person/States/BaseState.h"
#include "RunAwayState.generated.h"
/**
 * 
 */
UCLASS()
class GETDEBT_API URunAwayState : public UBaseState
{
    GENERATED_BODY()
public:

    UPROPERTY(VisibleAnywhere)
    float TimeSinceLastFarthestWaypointCalc;

    void EnterState() override;

    void ExitState() override;

    void UpdateState(float DeltaSeconds) override;
};
