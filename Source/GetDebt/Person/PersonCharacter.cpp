// Fill out your copyright notice in the Description page of Project Settings.

#include "GetDebt.h"
#include "PersonCharacter.h"
#include "../Player/Machine/BaseFrame.h"
#include "../GetDebtGameMode.h"
#include "PersonAIController.h"

APersonCharacter::APersonCharacter(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UMovementComponent>(TEXT("Movement")))
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.    
	PrimaryActorTick.bCanEverTick = true;

	this->AIControllerClass = APersonAIController::StaticClass();

	this->BoxCollision = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, "CollisionComponent");
	this->BoxCollision->SetSimulatePhysics(true);
	this->BoxCollision->SetCollisionProfileName(TEXT("Pawn"));
	this->BoxCollision->SetEnableGravity(false);
	this->BoxCollision->SetLinearDamping(1.0f);
	this->BoxCollision->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	this->BoxCollision->SetRelativeLocation(FVector(0.0f, 0.0f, 50.0f));
	this->BoxCollision->bShouldUpdatePhysicsVolume = true;
	this->SetRootComponent(this->BoxCollision);

	this->SensingComponent = ObjectInitializer.CreateDefaultSubobject<UPawnSensingComponent>(this, TEXT("SensingComponent"));
    this->SensingComponent->SightRadius = 300.0f;
	this->MaxMoveSpeed = 15.0f;

	this->DebugArrow = ObjectInitializer.CreateDefaultSubobject<UArrowComponent>(this, TEXT("DebugArrow"));
	this->DebugArrow->SetRelativeLocation(FVector::ZeroVector);

	this->TargetLocation = FVector::ZeroVector;
}

// Called when the game starts or when spawned
void APersonCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Note(Thompson): Will return NULL if this is called on a network client.
	//We need to have some global references in our game, so each actor can interact with all of them nicely.
	AGetDebtGameMode* GetDebtMode = (AGetDebtGameMode*) this->GetWorld()->GetAuthGameMode();
	if (GetDebtMode != NULL || GetDebtMode != nullptr){
		GetDebtMode->PeopleArray.Push(this);
	}
}

// Called every frame
void APersonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AGameMode* GameMode = this->GetWorld()->GetAuthGameMode();
	AGetDebtGameMode* GetDebtMode = CastChecked<AGetDebtGameMode>(GameMode);
	ABaseFrame* MainBaseFrame = GetDebtMode->MainBaseFrame;
	APersonAIController* Controller = CastChecked<APersonAIController>(this->GetController());
	if (Controller == nullptr){
		check(0 && "Unable to obtain PersonAIController.");
	}

	//If we aren't at the target location
	if (!this->IsAtTargetLocation())
	{
		UpdateMovement(DeltaTime);
	}
    else if(Controller == nullptr) {
        //Zero out rotation
        if(BoxCollision != nullptr) {
            FRotator CurrentRotation = this->BoxCollision->GetComponentRotation();
            FRotator NewRotation = CurrentRotation;
            NewRotation.Pitch = 0;
            NewRotation.Roll = 0;
            NewRotation.Yaw = 0;
            FVector ActualMomentum = FVector::ZeroVector;
            ActualMomentum.Z = -500.0f;
            this->BoxCollision->AddForce(ActualMomentum / DeltaTime);

            this->BoxCollision->SetWorldRotation(FRotator(0.0f, NewRotation.Yaw, 0.0f));
        }
    }

	if (this->GetActorLocation().Z > 70.0f){
		this->BoxCollision->SetEnableGravity(true);
	}
	else {
		this->BoxCollision->SetEnableGravity(false);
	}
}

// Called to bind functionality to input
void APersonCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
}

bool APersonCharacter::CanSeeMachine(){
	//Note (Robert) New code using PawnSensing
	if (this->GetController() != nullptr){
		this->TempController = CastChecked<APersonAIController>(this->GetController());
		if ((this->TempController != nullptr) && (this->TempController->ClosestPlayerController != nullptr) && (this->TempController->ClosestPlayerController->GetPawn() != nullptr)){
			//FIXME (Thompson): Unhandled exception, where the variable, Controller, is optimized away by Visual Studio. You need to add UPROPERTY() to Controller, else
			//it will get garbage collected by UE API.
			ABaseFrame* MainBaseFrame = CastChecked<ABaseFrame>(this->TempController->ClosestPlayerController->GetPawn());
			if (MainBaseFrame != nullptr){
				FHitResult HitResult;
				FCollisionQueryParams CollisionParams(FName(TEXT("Line Trace")), true, this);
				CollisionParams.bTraceComplex = true;
				CollisionParams.bTraceAsyncScene = true;
				CollisionParams.bReturnPhysicalMaterial = false;
				FCollisionObjectQueryParams ObjectParams;
				ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);
				this->GetWorld()->LineTraceSingle(HitResult, this->GetActorLocation(), MainBaseFrame->GetActorLocation(), CollisionParams, ObjectParams);

				if (HitResult.bBlockingHit){
					return this->SensingComponent->HasLineOfSightTo(MainBaseFrame);
				}
				else {
					float Distance = FVector::Dist(this->GetActorLocation(), MainBaseFrame->GetActorLocation());
					if (Distance < 600.0f){
						return true;
					}
					else {
						return this->SensingComponent->HasLineOfSightTo(MainBaseFrame);
					}
				}
			}
		}
	}
#ifdef DEBUG
	if (this->TempController == nullptr){
		//LOG("TempController is NULL.");
	}
	else if (this->TempController->ClosestPlayerController == nullptr) {
		//LOG("Closest player controller is NULL.");
	}
	else if (this->TempController->ClosestPlayerController->GetPawn() == nullptr) {
		//LOG("Pawn returned from closest player controller is NULL.");
	}
#endif
	return false;
}

void APersonCharacter::MoveTowardsTarget(FVector NewTargetLocation)
{
	//TODO(Robert) Do we even need this function?
	//Note(Thompson): We might need to.
	this->TargetLocation = NewTargetLocation;
	this->TargetLocation.Z = 0.0f;

	/*FVector StartingLocation = this->GetActorLocation();*/
}

void APersonCharacter::UpdateMovement(float DeltaTime)
{
	FVector CurrentLocation = this->GetActorLocation(); //Optimized away.

	FVector TempTargetLocation(this->TargetLocation);
	FVector TempCurrentLocation(CurrentLocation);
	TempTargetLocation.Z = TempCurrentLocation.Z = 0.0f;



	float DistancetoTargetLocation = FVector::Dist(TempTargetLocation, TempCurrentLocation);
	float Deceleration = 100.0f;
	float Speed = DistancetoTargetLocation / Deceleration;

	Speed = FMath::Min(Speed, MaxMoveSpeed);

	FVector ToTarget = TempTargetLocation - TempCurrentLocation;

	//Calculate new velocity
	FVector ChangeInVelocityNeeded = ToTarget * Speed / DistancetoTargetLocation;
    //ChangeInVelocityNeeded.Z = this->GetWorld()->GetGravityZ();
    ChangeInVelocityNeeded.Z = 0;

	//Note(Thompson): FVector to FRotator
	FVector TempDistance = TempCurrentLocation - TempTargetLocation;
	TempDistance.Normalize();
	FRotator NewRotation = TempDistance.Rotation();

	//Note(Thompson): Nullifying pitch and roll.
	NewRotation.Pitch = 0.0f;
	NewRotation.Roll = 0.0f;

	////Apply new Rotation
	//this->ClientSetRotation(NewRotation); //TODO Can we make this not instant? Should we?


	if (BoxCollision != nullptr) {
		float Mass = this->BoxCollision->GetMass();
		FVector ActualMomentum = ChangeInVelocityNeeded * Mass;
        ActualMomentum.Z = 0;
		this->BoxCollision->AddForce(ActualMomentum / DeltaTime);

		FRotator ChangedRotation = ChangeInVelocityNeeded.Rotation();
		ChangedRotation.Pitch = ChangedRotation.Roll = 0.0f;
		this->BoxCollision->SetWorldRotation(ChangedRotation);
	}

#ifdef DEBUG
	//DrawDebugLine(this->GetWorld(), this->GetActorLocation(), this->TargetLocation, FColor::Magenta, false, -1.0f, (uint8)'\000', 15.0f);
#endif
}

bool APersonCharacter::IsAtTargetLocation(){
	bool Result = (FVector::Dist(this->TargetLocation, this->GetActorLocation()) < 250.0f);
	return Result;
}

bool APersonCharacter::IsPersonInRangeToPlayer(ABaseFrame* HuntingPlayer, APersonCharacter* HuntedPerson, float Distance){
	FVector PlayerLocation = HuntingPlayer->GetActorLocation();
	FVector PersonLocation = HuntedPerson->GetActorLocation();

#ifdef DEBUG
	DrawDebugSphere(this->GetWorld(), PersonLocation, Distance, 32, FColor::Blue, false, 2.5f);
#endif

	if (FMath::Abs<float>((FVector::Dist(PlayerLocation, PersonLocation)) <= Distance)){
		return true;
	}
	return false;
}