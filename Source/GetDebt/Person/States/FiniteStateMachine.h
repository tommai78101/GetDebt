// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Containers/Map.h"
#include "GameFramework/Info.h"
//#include "Person/States/BaseState.h"
#include "FiniteStateMachine.generated.h"

//Note(Robert) This forward declaration is needed to prevent circular dependency with the headers
class UBaseState;
class APersonAIController;

UENUM(BlueprintType)
enum class EFSMState : uint8
{
	EFSM_RunAway	UMETA(DisplayName="Run Away"),
	EFSM_Hide		UMETA(DisplayName="Hide"),
	EFSM_Idle		UMETA(DisplayName="Idle"),
	EFSM_Wander		UMETA(DisplayName="Wander")
};

 // so TMap will work properly with our enum
 inline uint8 GetTypeHash(const EFSMState EFSM)
 {
     return (uint8)EFSM;
 }

UCLASS(Blueprintable, BlueprintType)
class GETDEBT_API AFiniteStateMachine : public AInfo
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFiniteStateMachine(const FObjectInitializer& ObjectInitializer);

	//PROPERTIES
    UPROPERTY(BlueprintReadOnly, EditDefaultsOnly, Category = "State Machine")
        TEnumAsByte<EFSMState> CurrentState;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "State Machine")
        UBaseState* CurrentStateObject;

    UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "State Machine")
        APersonAIController* OwningController;

    //A TMap can't be a UProperty
    /* The State and their Corresponding Class Implementation. These need to registered manually in the constructor */
        TMap<EFSMState, UClass*> StateClassMap;
        
    //Functions
    UFUNCTION(BlueprintCallable, Category = "State Machine")
        void SwitchState(EFSMState NewState);

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;
	
};
