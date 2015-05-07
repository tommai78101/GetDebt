// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "GetDebt.h"
#include "GetDebtGameMode.h"

#include "Objects/GetDebtPawn.h"
#include "Objects/GetDebtHud.h"
#include "Objects/GetDebtWheelFront.h"

#include "Obstacle/Obstacle.h"

#include "Person/PersonCharacter.h"

#include "Player/Machine/MainCore.h"
#include "Player/Machine/BaseFrame.h"
#include "Player/Machine/BaseWheel.h"

#include "Player/MainPlayerController.h"

AGetDebtGameMode::AGetDebtGameMode(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	this->DefaultPawnClass = ABaseFrame::StaticClass();

    static ConstructorHelpers::FObjectFinder<UBlueprint> HudObject(TEXT("Blueprint'/Game/Blueprints/HUD/BP_GetDebtHUD.BP_GetDebtHUD'"));
	//static ConstructorHelpers::FObjectFinder<UBlueprint> HudObject(TEXT());
	if (HudObject.Succeeded()){
		this->HUDClass = (UClass*) HudObject.Object->StaticClass();
	}
	else {
		this->HUDClass = AGetDebtHud::StaticClass();
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> ControllerObject(TEXT("Blueprint'/Game/Blueprints/Player/BP_MainPlayerController.BP_MainPlayerController'"));

	//static ConstructorHelpers::FObjectFinder<UBlueprint> (TEXT());
	if (ControllerObject.Succeeded()){
		this->PlayerControllerClass = (UClass*)HudObject.Object->StaticClass();
	}
	else {
		this->PlayerControllerClass = AMainPlayerController::StaticClass();
	}

	this->PlayerOneScore = 0;
	this->PlayerTwoScore = 0;

	this->RemainingPeople = 0;

	this->TimeForLevelInSeconds = 120.0f;
	this->TimeRemaining = 0.0f;
	this->TimeLimitHit = false;
}

//IMPORTANT(Thompson):	Do not forget to override StartPlay() and call on StartMatch(). This is crucial if you want all actors in the world
//						to start moving around.
void AGetDebtGameMode::StartPlay(){
	//Entering Map State.
	LOG("Match State: " + this->GameState->GetMatchState().ToString());

	InitializeEnteringMap();

	Super::StartPlay();

	//Waiting to Start state: 
	LOG("Match State: " + this->GameState->GetMatchState().ToString());

	InitializeWaitingToStart();

	//FString Test = this->GetWorld()->GetMapName();
	if (this->GetWorld()->GetMapName().Contains("MainLevel")){
		this->TimeRemaining = this->TimeForLevelInSeconds;
		//this->GetWorldTimerManager().SetTimer(this, &AGetDebtGameMode::TimeForLevelUp, TimeForLevelInSeconds, false);
	}

	this->StartMatch();
}

void AGetDebtGameMode::Tick(float DeltaSeconds)
{
	if (this->GetWorld()->GetMapName().Contains("MainLevel")){
		TArray<AActor*> PersonArray;
		UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), APersonCharacter::StaticClass(), PersonArray);

		this->RemainingPeople = PersonArray.Num();

		if (this->RemainingPeople == 0)
		{
			this->EndGame();
		}
		else {

			if (this->TimeRemaining > 0)
			{
				this->TimeRemaining = this->TimeRemaining - DeltaSeconds;

				if (this->TimeRemaining <= 0)
				{
					this->TimeForLevelUp();
				}
			}
		}
	}
}

void AGetDebtGameMode::InitializeEnteringMap(){
	LOG("Initializing. This state is the Entering Map state.");
}

void AGetDebtGameMode::InitializeWaitingToStart(){
	LOG("Initializing. This state is the Waiting to Start state.");
}

//bool AGetDebtGameMode::CanPersonSeePlayer(ABaseFrame* HuntingPlayer, FVector PlayerEyePosition, APersonCharacter* HuntedPerson, FVector PersonEyePosition){
//	FVector PlayerEyeLocation = HuntingPlayer->GetActorLocation() + PlayerEyePosition;
//	FVector HuntedPersonEyeLocation = HuntedPerson->GetActorLocation() + PersonEyePosition;
//
//	TArray<FHitResult> OutHits;
//
//	FCollisionQueryParams CollisionParams;
//	CollisionParams.bFindInitialOverlaps = true;
//
//	FCollisionObjectQueryParams ObjectParams;
//	ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_PhysicsBody);
//
//#ifdef DEBUG
//	DrawDebugLine(this->GetWorld(), HuntedPersonEyeLocation, PlayerEyeLocation, FColor::Blue, false, 2.5f, (uint8)'\000', 1.0f);
//#endif
//
//	if (this->GetWorld()->LineTraceMulti(OutHits, HuntedPersonEyeLocation, PlayerEyeLocation, CollisionParams, ObjectParams)){
//		for (auto Iterator = OutHits.CreateIterator(); Iterator; ++Iterator){
//			FHitResult HitResult = *Iterator;
//			if (HitResult.GetActor()->IsA<ABaseFrame>()){
//				return true;
//			}
//		}
//	}
//	return false;
//}

//Game lose
void AGetDebtGameMode::EndGame()
{
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this->GetWorld(), 0);
	AMainPlayerController* Controller = CastChecked<AMainPlayerController>(PlayerController);

	Controller->EndGame();
	//this->Player
}

void AGetDebtGameMode::TimeForLevelUp()
{
	this->TimeLimitHit = true;
	this->EndGame();
}