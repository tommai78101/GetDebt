// Fill out your copyright notice in the Description page of Project Settings.

#include "GetDebt.h"
#include "MainPlayerController.h"
#include "../GetDebtGameMode.h"
#include "../Waypoints/Waypoint.h"
#include "Machine/MainCore.h"

AMainPlayerController::AMainPlayerController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	this->PrimaryActorTick.bCanEverTick = true;

    //TODO(Robert) I think this might fix some issues we have with detection. If something breaks this may be why.
    //this->bAttachToPawn = true;

    //Note(Robert) Camera related
	//this->ControlRotation = FRotator(-45.0f, 0.0f, 0.0f);
	//this->RootComponent->SetWorldRotation(FRotator(-45.0f, 0.0f, 0.0f));
	this->IgnoreLookInput = false;
}

void AMainPlayerController::BeginPlay(){
	Super::BeginPlay();

	AGameMode* GameMode = this->GetWorld()->GetAuthGameMode();
	AGetDebtGameMode* GetDebtGameMode = CastChecked<AGetDebtGameMode>(GameMode);
	if (GetDebtGameMode != nullptr || GetDebtGameMode != NULL){
		this->MainBaseFrame = GetDebtGameMode->MainBaseFrame;
	}
    
    APawn* Pawn = this->GetPawn();

    if(Pawn != nullptr) {
        this->PlayerCameraManager->SetViewTarget(Pawn);
		//this->PlayerCameraManager->SetActorRotation(FRotator(-45.0f, 0.0f, 0.0f));
    }

	//Note(Robert) Camera related
	this->ControlRotation = FRotator(-45.0f, 0.0f, 0.0f);
	this->RootComponent->SetRelativeRotation(FRotator(-45.0f, 0.0f, 0.0f));

}

void AMainPlayerController::Tick(float DeltaTime){
	Super::Tick(DeltaTime);

    //Adjust max/min according to actor rotation
    APawn* Pawn = this->GetPawn();
    if(Pawn != nullptr) {
        float PawnYaw = this->GetPawn()->GetActorRotation().GetNormalized().Yaw;
        float OurAbsYaw = this->ControlRotation.GetNormalized().Yaw;
        float OurYaw = OurAbsYaw - PawnYaw;
        OurYaw = FRotator::NormalizeAxis(OurYaw);

        //Clamp camera to limits
        OurYaw = FMath::Clamp(OurYaw, MinYaw, MaxYaw);
        OurAbsYaw = OurYaw + PawnYaw;
        this->ControlRotation.Yaw = OurAbsYaw;

        float CurrentPitch = this->ControlRotation.Pitch;
        CurrentPitch = FRotator::NormalizeAxis(CurrentPitch);
        CurrentPitch = FMath::Clamp(CurrentPitch, MinPitch, MaxPitch);
        this->ControlRotation.Pitch = CurrentPitch;

        //Drift camera back into alignment
        if(OurYaw < MinCameraOffset) {
            OurYaw = FMath::InterpEaseIn(OurYaw, MinCameraOffset, DeltaTime, 1.0f);
        }

        if(OurYaw > MaxCameraOffset) {
            OurYaw = FMath::InterpEaseIn(OurYaw, MaxCameraOffset, DeltaTime, 1.0f);
        }

        OurAbsYaw = OurYaw + PawnYaw;
        this->ControlRotation.Yaw = OurAbsYaw;
    }
}

void AMainPlayerController::SetupInputComponent(){
	Super::SetupInputComponent();
}

AWaypoint* AMainPlayerController::CalculateFarthestWaypoint(){
	FVector OurLocation = this->GetPawn()->GetActorLocation();
	AGameMode* GameMode = this->GetWorld()->GetAuthGameMode();
	AGetDebtGameMode* GetDebtGameMode = CastChecked<AGetDebtGameMode>(GameMode);
	float MaxDistance = 0.0f;
	if (GetDebtGameMode != nullptr ){
		AWaypoint* FarthestWaypoint = nullptr;
        for(TActorIterator<AWaypoint> WaypointItr(this->GetWorld()); WaypointItr; ++WaypointItr) {
            //for (auto Iterator = GetDebtGameMode->WaypointsArray.CreateIterator(); Iterator; ++Iterator){
            AWaypoint* Waypoint = *WaypointItr;
            FVector WaypointLocation = Waypoint->GetActorLocation();
            float NewDistance = FVector::Dist(OurLocation, WaypointLocation);
            if(NewDistance > MaxDistance) {
                FarthestWaypoint = Waypoint;
                MaxDistance = NewDistance;
            }
        }

		if (FarthestWaypoint != nullptr){
			this->FarthestWaypoint = FarthestWaypoint;
			return this->FarthestWaypoint;
		}
	}
	return nullptr;
}
