// Fill out your copyright notice in the Description page of Project Settings.

#include "GetDebt.h"
#include "BaseWheel.h"


// Sets default values
ABaseWheel::ABaseWheel(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WheelMeshObject(TEXT("StaticMesh'/Game/Meshes/Test_Wheel.Test_Wheel'"));
	if (WheelMeshObject.Succeeded()){
		this->Donut = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Cylinder"));
		this->Donut->SetStaticMesh(WheelMeshObject.Object);
		this->Donut->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName); //Setting the physics actor profile.
		this->Donut->SetSimulatePhysics(true);
		this->Donut->SetAngularDamping(0.4f); //Adding friction to air resistance
		this->Donut->SetLinearDamping(0.4f); //Adding friction to force.
		this->Donut->BodyInstance.MassScale = 0.0f; //The wheel doesn't have to be that heavy, right?
		this->Donut->BodyInstance.MaxAngularVelocity = 800.0f; //Since I don't know how to compare this with real life counterparts, I'm going for 800f.
		this->Donut->SetNotifyRigidBodyCollision(false); //Receive rigid body collision events. (Hit, overlap, etc. I guess)
		this->Donut->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f)); //Wheel was so large when it's 0.5f across all axes.
		this->SetRootComponent(this->Donut);
	}
}

// Called when the game starts or when spawned
void ABaseWheel::BeginPlay()
{
	Super::BeginPlay();
	this->EnableInput(UGameplayStatics::GetPlayerController(this->GetWorld(), 0));

	//NOTE(Thompson): Make sure that for multiplayer, the index count will only affect the controller of that player index.
	APlayerController* Controller = UGameplayStatics::GetPlayerController(this->GetWorld(), 0);
	if (Controller){
		this->InputComponent = Controller->InputComponent;
		this->InputComponent->BindAxis("TurnWheel", this, &ABaseWheel::Turn);
		this->InputComponent->BindAxis("SteerWheel", this, &ABaseWheel::Steer);
	}

}

// Called every frame
void ABaseWheel::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

//void ABaseWheel::SetupInputComponent(UInputComponent* InputComponent){
//	Super::SetupInputComponent(InputComponent);
//
//	InputComponent->BindAxis("TurnWheel", this, &ABaseWheel::Turn);
//}

void ABaseWheel::Turn(float Value){
	//This is how you turn a wheel. You apply torque to the mesh, not arbitrary angles.
	//Torque unit is in Joule.
	//-1.0f is because we want it to go in reverse direction.
	//Value determines the direction.
	//50,000,000f Joule determines the torque force applied to get the mesh to spin in the X axis.
	//Force based steering, yay!
	/*

	Math example: 

	For example, if I have a 208kg cube which is 80cm x 80cm x 80cm the associated moment of 
	inertia will be 368058.469 This means that when I apply a torque of 10000 I should expect an 
	angular acceleration of 10000/368058.469 rad/s = .027 rad/s or about 1.5 degrees / second.
	
	*/

	//Try to get a rotator from anywhere that you want your vector to face towards.
	//In this case, I used my mesh rotation.
	FRotator Rotation = this->Donut->GetComponentRotation();
	
	//This is an arbitrary target vector you want to rotate. It can be any vector you designate.
	FVector Vector = FVector(1.0f, 0.0f, 0.0f);

	//To rotate, I call on FRotationMatrix class, pass in the rotator, and call on RotateVector()
	//to rotate a vector for me. Pass it in like this:
	Vector = Rotation.RotateVector(Vector);


	FVector Torque = FVector(0.0f, 1.0f, 0.0f);
	const float Scalar = -1.0f * Value * 50000000.0f;
	Torque *= Scalar;
	this->Donut->AddTorque(Torque);
}


void ABaseWheel::Steer(float Value){
	this->Donut->AddWorldRotation(FRotator(0.0f, Value, 0.0f));
}