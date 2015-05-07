// Fill out your copyright notice in the Description page of Project Settings.

#include "GetDebt.h"
#include "../GetDebtGameMode.h"
#include "../Player/Machine/BaseFrame.h"
#include "FlockObstacle.h"

FVector MaxClampVector(FVector Vector, float MaxValue){
	FVector Result;
	Result.X = FMath::Max<float>(Vector.X, MaxValue);
	Result.Y = FMath::Max<float>(Vector.Y, MaxValue);
	Result.Z = FMath::Max<float>(Vector.Z, MaxValue);
	return Result;
}

AFlockObstacle::AFlockObstacle(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	this->SphereCollider->SetSphereRadius(78.0f);
	this->SphereCollider->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialObject(TEXT("MaterialInstanceConstant'/Game/VehicleAdv/Materials/MaterialInstances/SolidOrange.SolidOrange'"));
	if (MaterialObject.Succeeded()){
		this->BodyMesh->SetMaterial(0, MaterialObject.Object->GetMaterial());
	}

    static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMeshObject(TEXT("SkeletalMesh'/Game/Animations/Robot_Dog/Final_Dog.Final_Dog'"));
    static ConstructorHelpers::FObjectFinder<UAnimationAsset> AnimationAsset(TEXT("AnimSequence'/Game/Animations/Robot_Dog/Final_Dog_Anim.Final_Dog_Anim'"));
    if(SkeletalMeshObject.Succeeded() && AnimationAsset.Succeeded()){
        this->BodyMesh->DestroyComponent(false);
        this->BodyMesh = nullptr;
        this->SkeletalMesh = ObjectInitializer.CreateDefaultSubobject<USkeletalMeshComponent>(this, TEXT("SkeletalMesh"));
        this->SkeletalMesh->SetSkeletalMesh(SkeletalMeshObject.Object);
        this->SkeletalMesh->SetAnimationMode(EAnimationMode::AnimationSingleNode);
        this->SkeletalMesh->SetAnimation(AnimationAsset.Object);
        this->SkeletalMesh->PlayAnimation(AnimationAsset.Object, true);
        this->SkeletalMesh->AnimationData.AnimToPlay = AnimationAsset.Object;
        this->SkeletalMesh->AnimationData.bSavedLooping = true;
        //this->SkeletalMesh->AnimationData->AnimToPlay = AnimationAsset.Object;
        this->SkeletalMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -77.0f));
        this->SkeletalMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
        this->SkeletalMesh->SetRelativeScale3D(FVector(2.0f));
        this->SkeletalMesh->AttachTo(this->SphereCollider);
    }

	this->RootComponent->bAbsoluteScale = true;
	this->RootComponent->SetWorldScale3D(FVector(2.0f));

	/*this->DebugArrow = ObjectInitializer.CreateDefaultSubobject<UArrowComponent>(this, TEXT("DebugArrow"));
	this->DebugArrow->SetRelativeLocation(FVector::ZeroVector);
	this->DebugArrow->ArrowSize = 1.0f;
	this->DebugArrow->ArrowColor = FColor::Green;
	this->DebugArrow->SetEditorScale(1.0f);
	this->DebugArrow->SetWorldScale3D(FVector(0.01f));*/
}

void AFlockObstacle::BeginPlay(){
	Super::BeginPlay();

	this->Velocity = FVector::ZeroVector;
	this->Acceleration = FVector::ZeroVector;
	this->TargetOffset = FVector(50.0f, 0.0f, 0.0f) + this->GetActorLocation();
	this->MaxSpeed = 1.0f;
	this->ForceStrength = 0.0f;
	this->BitwiseFlag = 0x0;
	this->FlockingRadius = 500.0f;
	this->SphereCollider->SetMassScale(NAME_None, 0.1f);
	this->SphereCollider->SetAngularDamping(0.1f);
	this->SphereCollider->SetLinearDamping(0.1f);
	this->SphereCollider->BodyInstance.UpdateDampingProperties();
	this->SkeletalMesh->SetAngularDamping(0.1f);
	this->SkeletalMesh->SetLinearDamping(0.1f);
	this->SkeletalMesh->BodyInstance.UpdateDampingProperties();
	this->FlatRotation = FRotator::ZeroRotator;
	this->CountDown = 10;

	//this->SetFlag(0x8);

	AGameMode* GameMode = this->GetWorld()->GetAuthGameMode();
	AGetDebtGameMode* GetDebtMode = CastChecked<AGetDebtGameMode>(GameMode);
	if (GetDebtMode){
		GetDebtMode->FlockingObstaclesArray.Add((AFlockObstacle*) this);
		this->FlockObstaclesArray = GetDebtMode->FlockingObstaclesArray;
	}

	this->SphereCollider->SetSimulatePhysics(true);

	//this->DebugArrow->Activate();
}

void AFlockObstacle::Move(){
	return;
}

void AFlockObstacle::Tick(float Delta){
	Super::Tick(Delta);

	this->MoveUpdate(Delta);

#ifdef DEBUG
	this->TargetOffset = FVector(150.0f, 0.0f, 0.0f);
	this->TargetOffset = this->FlatRotation.RotateVector(this->TargetOffset);
	DrawDebugLine(this->GetWorld(), this->GetActorLocation(), this->TargetOffset + this->GetActorLocation(),  FColor::Green, false, (-1.0f), (uint8)'\000', 15.0f);
#endif
}

void AFlockObstacle::MoveUpdate(float Delta){
	this->UpdateObstaclesArray();
	FVector PlayerLocation(0.0f);

	if (this->CountDown == 0){
		this->CountDown = COUNTDOWN_VALUE;

		//Original velocity setup. Do not erase.
		//this->Velocity = FVector::ZeroVector;
		//this->Velocity += this->Wander() * 1.8f;
		//this->Velocity = FVector::ZeroVector;

		APlayerController* Controller = UGameplayStatics::GetPlayerController(this->GetWorld(), 0);
		if ((Controller != nullptr || Controller != NULL) && (Controller->GetPawn() != nullptr || Controller->GetPawn() != NULL)){
			//Player
			PlayerLocation = Controller->GetPawn()->GetActorLocation();
		}
		else {
			//In case player did not spawn into the level.
			this->Velocity = FVector::ZeroVector;
			this->Velocity += this->Wander() * 0.03f;
			this->Velocity += this->Cohesion() * -2.5f;
			this->Velocity += this->Separation() * 2.5f;
		}

		if (PlayerLocation.Size() != 0.0f){
			if ((FVector::Dist(PlayerLocation, this->GetActorLocation()) < 600.0f) || (FVector::Dist(PlayerLocation, this->GetActorLocation()) > 4000.0f)){
				this->Velocity = PlayerLocation - this->GetActorLocation();
				this->Velocity += this->Wander() * 0.03f;
				this->Velocity += this->Cohesion() * -2.5f;
				this->Velocity += this->Separation() * 2.5f;
			}
			else {
				//this->Velocity = PlayerLocation - this->GetActorLocation();
				this->Velocity += this->Wander() * 2.5f;
				this->Velocity += this->Cohesion();
				this->Velocity += this->Separation();
			}
		}

		this->Velocity.Normalize();
		this->Velocity.Z = 0.0f;
	}
	else {
		this->CountDown--;
	}

	this->Velocity *= this->MaxSpeed;

	FVector Momentum = (this->SphereCollider->BodyInstance.GetBodyMass() * this->Velocity) / Delta;
	this->SphereCollider->AddForce(Momentum);


	
	this->FlatRotation = this->Velocity.Rotation();
	this->FlatRotation.Pitch = this->FlatRotation.Roll = 0.0f;

	//this->SetActorRotation(this->Alignment());
	this->SphereCollider->SetWorldRotation(this->FlatRotation);
}

void AFlockObstacle::SetFlag(uint16 Flag){
	this->BitwiseFlag = Flag;
}


FVector AFlockObstacle::Seek(const FVector Target){
	FVector Desired = Target - this->GetActorLocation();
	Desired.Normalize();
	FVector VelocityCopy(this->Velocity);
	VelocityCopy.Normalize();
	return (Desired - VelocityCopy);
}

FVector AFlockObstacle::Arrive(const FVector Target){
	float Distance = FMath::Abs<float>(FVector::Dist(this->GetActorLocation(), Target));
	if (Distance <= 0.0001f){
		return FVector::ZeroVector;
	}

	const float DECELERATION = 2000.0f;
	float Speed = Distance / DECELERATION;
	Speed = FMath::Min<float>(Speed, this->MaxSpeed);

	FVector Desired = Target - this->GetActorLocation();
	FVector Result = (Desired - this->Velocity);
	return Result;
}


FVector AFlockObstacle::Flee(const FVector Target){
	FVector Desired = this->GetActorLocation() - Target;
	Desired.Normalize();
	FVector VelocityCopy(this->Velocity);
	VelocityCopy.Normalize();
	return (Desired - VelocityCopy);
}


FVector AFlockObstacle::Pursue(AFlockObstacle* Obstacle, FVector Target){
	FVector	Distance = Obstacle->GetActorLocation() - this->GetActorLocation();
	float Bearing = (this->GetActorRotation().Vector().HeadingAngle() * Obstacle->GetActorRotation().Vector().HeadingAngle());
	if (Bearing > 0 && Bearing < -0.95){
		return Seek(Target);
	}
	
	float Delta = FMath::Abs<float>(Distance.Size()) / (this->MaxSpeed + FMath::Abs<float>(Obstacle->Velocity.Size()));
	return this->Seek(Obstacle->GetActorLocation() + (Obstacle->Velocity * Delta));
}

FVector AFlockObstacle::Evade(AFlockObstacle* Obstacle, FVector Target){
	FVector	Distance = Obstacle->GetActorLocation() - this->GetActorLocation();
	float Delta = FMath::Abs<float>(Distance.Size()) / (this->MaxSpeed + FMath::Abs<float>(Obstacle->Velocity.Size()));
	return this->Flee(Obstacle->GetActorLocation() + (Obstacle->Velocity * Delta));
}

void AFlockObstacle::Hide(){
}

FVector AFlockObstacle::Interpose(){
	FVector Average = FVector::ZeroVector;
	for (auto Iterator = this->FlockObstaclesArray.CreateIterator(); Iterator; ++Iterator){
		Average += (*Iterator)->GetActorLocation();
	}
	Average /= this->FlockObstaclesArray.Num();
	return this->Seek(Average);
}

FVector AFlockObstacle::Wander(){
	FVector CenterCircle = FVector(250.0f, 0.0f, 0.0f);
	CenterCircle = this->FlatRotation.RotateVector(CenterCircle);
	FVector TargetRadius = FVector(100.0f, 0.0f, 0.0f);
	FRotator Random = FRotator(0.0f, FMath::FRandRange(-180.0f, 180.0f), 0.0f);
	TargetRadius = Random.RotateVector(TargetRadius);
	FVector WanderTarget = TargetRadius + (CenterCircle + this->GetActorLocation());
	return WanderTarget - this->GetActorLocation();
}

void AFlockObstacle::AvoidObstacle(){
}

void AFlockObstacle::AvoidObstacles(){
}


FVector AFlockObstacle::Cohesion(){
	FVector Center = this->CohesionCenter;
	int32 Count = 0;
	for (auto Iterator = this->FlockObstaclesArray.CreateIterator(); Iterator; ++Iterator){
		AFlockObstacle* Obstacle = *Iterator;
		if (FMath::Abs<float>(FVector::Dist(this->GetActorLocation(), Obstacle->GetActorLocation())) < 500.0f){
			Center += Obstacle->GetActorLocation();
			Count++;
		}
	}
	if (Count > 0){
		Center /= Count;
	}
	this->CohesionCenter = Center;
	return this->Seek(this->CohesionCenter);
}

FVector AFlockObstacle::Cohesion2(FVector Target){
	FVector Center = Target;
	int32 Count = 0;
	for (auto Iterator = this->FlockObstaclesArray.CreateIterator(); Iterator; ++Iterator){
		AFlockObstacle* Obstacle = *Iterator;
		if (FMath::Abs<float>(FVector::Dist(this->GetActorLocation(), Obstacle->GetActorLocation())) < 500.0f){
			Center += Obstacle->GetActorLocation();
			Count++;
		}
	}
	if (Count > 0){
		Center /= Count;
	}
	return this->Seek(Center);
}

FVector AFlockObstacle::Separation(){
	//Get list of all flocking obstacles
	FVector TotalForce = FVector::ZeroVector;
	for (auto Iterator = this->FlockObstaclesArray.CreateIterator(); Iterator; ++Iterator){
		AFlockObstacle* Obstacle = *Iterator;
		if (Obstacle == this){
			continue;
		}
		if (this->CheckObstacleDistance(Obstacle)){
			FVector Direction = this->GetActorLocation() - Obstacle->GetActorLocation();
			Direction.Normalize();
			TotalForce += Direction / FMath::Abs<float>(Direction.Size());
		}
	}
	return TotalForce;
}

FRotator AFlockObstacle::Alignment(){
	//FRotator Average = FRotator::ZeroRotator;
	FRotator Average = this->GetRootComponent()->GetComponentRotation();
	int32 Count = 0;
	for (auto Iterator = this->FlockObstaclesArray.CreateIterator(); Iterator; ++Iterator){
		AFlockObstacle* Obstacle = *Iterator;
		if (Obstacle != this){
			if (FMath::Abs<float>(FVector::Dist(this->GetActorLocation(), Obstacle->GetActorLocation())) < 300.0f){
				FRotator Rotation = Obstacle->FlatRotation;
				Rotation.Pitch = Rotation.Roll = 0.0f;
				Average += Rotation;
				Count++;
			}
		}
	}
	if (Count > 0){
		Average.Yaw /= Count;
	}
	Average = Average.Clamp();
	Average.Pitch = Average.Roll = 0.0f;
	return Average;
}

void AFlockObstacle::SetFlockingRadius(float Radius){
	this->FlockingRadius = Radius;
}

bool AFlockObstacle::CheckSimulate(){
	bool Result = false;
	for (auto Iterator = this->FlockObstaclesArray.CreateIterator(); Iterator; ++Iterator){
		AFlockObstacle* Obstacle = *Iterator;
		if (Obstacle == this){
			continue;
		}
		float Distance = FVector::Dist(this->GetActorLocation(), Obstacle->GetActorLocation());
		if (Distance < this->FlockingRadius){
			Result = true;
		}
	}
	return Result;
}

bool AFlockObstacle::CheckAir(){
	bool Result = false;
	for (auto Iterator = this->FlockObstaclesArray.CreateIterator(); Iterator; ++Iterator){
		AFlockObstacle* Obstacle = *Iterator;
		if (Obstacle == this){
			continue;
		}
		Result = !(this->GetActorLocation().Z - Obstacle->GetActorLocation().Z == 0.0f);
	}
	return Result;
}

bool AFlockObstacle::CheckObstacleDistance(AFlockObstacle* Obstacle){
	const float Distance = FVector::Dist(Obstacle->GetActorLocation(), this->GetActorLocation());
	return (Distance < this->FlockingRadius);
}

void AFlockObstacle::UpdateObstaclesArray(){
	AGameMode* GameMode = this->GetWorld()->GetAuthGameMode();
	AGetDebtGameMode* GetDebtMode = CastChecked<AGetDebtGameMode>(GameMode);
	if (GetDebtMode){
		this->FlockObstaclesArray = GetDebtMode->FlockingObstaclesArray;
	}
}