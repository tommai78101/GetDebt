// Fill out your copyright notice in the Description page of Project Settings.

#include "GetDebt.h"
#include "Obstacle.h"


// Sets default values
AObstacle::AObstacle(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer.SetDefaultSubobjectClass<UMovementComponent>(TEXT("ObstacleBasicMovement")))
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->TimerCountdown = this->TimerInitialValue = 3;

	//Uniform scale
	FVector Scale = FVector(1.0f);

	//Setting basic mesh and collider
	this->SphereCollider = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("SphereCollider"));
	//this->SphereCollider->SetRelativeScale3D(Scale * 2.0f);
	this->SphereCollider->SetWorldScale3D(Scale * 1.5f);
	this->SphereCollider->SetRelativeLocation(FVector::ZeroVector);
    this->SphereCollider->SetCollisionProfileName(TEXT("Pawn"));
	this->SphereCollider->SetSimulatePhysics(true);
	this->SphereCollider->SetSphereRadius(38.5f);
	this->SetRootComponent(this->SphereCollider);


	static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMeshObject(TEXT("StaticMesh'/Game/Meshes/Sphere_Mesh.Sphere_Mesh'"));
	if (BodyMeshObject.Succeeded()){
		this->BodyMesh = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("BodyMesh"));
		this->BodyMesh->SetStaticMesh(BodyMeshObject.Object);
		this->BodyMesh->SetRelativeLocation(FVector::ZeroVector);
		this->BodyMesh->AttachTo(this->SphereCollider);
	}

	//Other properties
	this->Acceleration = FVector::ZeroVector;
	this->Velocity = FVector::ZeroVector;
}

// Called when the game starts or when spawned
void AObstacle::BeginPlay()
{
	Super::BeginPlay();
	Move();
}

// Called every frame
void AObstacle::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

	if (this->TimerCountdown > 0){
		this->TimerCountdown--;
	}
	else {
		this->TimerCountdown = this->TimerInitialValue;
		this->Acceleration = FVector::ZeroVector;
		Move();
	}

	////Must not let the velocity reach zero. If it's zero, it
	//this->Velocity += this->Acceleration;
	//if (this->Velocity.IsNearlyZero()){
	//	this->Velocity += this->Acceleration * 40.0f;
	//}

	////Clamping the velocity down to top speeds of 400.0f, which is the default value for all Actors.
	//this->Velocity = this->Velocity.GetClampedToMaxSize(40.0f);

	////There is a possibility that float values are NaN.
	//if (FMath::IsNaN(this->Velocity.X)){
	//	this->Velocity.X = 0.0f;
	//}
	//if (FMath::IsNaN(this->Velocity.Y)){
	//	this->Velocity.Y = 0.0f;
	//}
	//if (FMath::IsNaN(this->Velocity.Z)){
	//	this->Velocity.Z = 0.0f;
	//}

	//F = ma. 35F is needed to push the obstacle around at a reasonable speed.
	//Momentum = mv.
	float Length;
	FVector Throwaway;
	this->Acceleration.ToDirectionAndLength(Throwaway, Length);
	if (Length < 64.0f){
		this->SphereCollider->AddForce(this->Acceleration * this->SphereCollider->GetMass() * 64.0f);
	}
}

// Called to bind functionality to input
void AObstacle::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

}

void AObstacle::Move(){
	float Distance = FVector::Dist(this->GetActorLocation(), FVector::ZeroVector);
	//Note(Thompson): Debug only
	//LOG("Obstacle Distance: " + FString::SanitizeFloat(Distance));
	FRotator Rotation;
	if (Distance > 3600.0f){
		FVector OriginTarget = FVector::ZeroVector - this->GetActorLocation();
		OriginTarget.Normalize();
		Rotation = FRotationMatrix::MakeFromX(OriginTarget).Rotator();
		Rotation.Pitch = 0.0f;
		Rotation.Roll = 0.0f;
	}
	else if (Distance > 2500.0f) {
		Rotation.Pitch = 0.0f;
		Rotation.Roll = 0.0f;
		Rotation = Rotation.Add(0.0f, FMath::FRandRange(-180.0f, 180.0f), 0.0f);
	}
	else {
		//Since ball is rolling, actor rotation is more linearly random.
		Rotation = this->GetActorRotation();
	}
	FVector Forward(FMath::FRand(), FMath::FRand(), 0.0f);
	Forward = Rotation.RotateVector(Forward);
	Forward.Normalize();
	this->Acceleration = Forward * (FMath::FRand() * 256.0f);
	//this->SetActorRotation(Rotation);
}