// Fill out your copyright notice in the Description page of Project Settings.

#include "GetDebt.h"
#include "MainCore.h"
#include "Objects/GetDebtWheelFront.h"
#include "Objects/GetDebtWheelRear.h"
#include "BaseWheel.h"

#include "../MainPlayerController.h"

// Sets default values
AMainCore::AMainCore(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	//this->MainBody = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("StaticMeshBody"));
    //this->GetMeshComponent()->SetSimulatePhysics(true);

	this->GetMeshComponent()->ResetRelativeTransform();
	this->GetRootComponent()->ResetRelativeTransform();


	UBoxComponent* CubeCollider = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("CubeColliderComponent"));
	if (CubeCollider){
		this->SetRootComponent(CubeCollider);
		this->GetRootComponent()->SetRelativeLocation(FVector::ZeroVector);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> FrameMeshObject(TEXT("StaticMesh'/Game/Meshes/Test_Cube_Mesh.Test_Cube_Mesh'"));
	if (FrameMeshObject.Succeeded()){
		this->GetMeshComponent()->SetStaticMesh(FrameMeshObject.Object);
		this->GetMeshComponent()->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
		this->GetMeshComponent()->SetRelativeLocation(FVector::ZeroVector);
	}
    this->GetMeshComponent()->SetOwnerNoSee(false);
	//this->SetRootComponent(this->MainBody);

	this->CameraSpringArm = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("CameraSpringArmExtension"));
	//this->CameraSpringArm->SetRelativeLocation(FVector(-80.0f, 0.0f, 120.0f));
	//this->CameraSpringArm->SetWorldRotation(FRotator(0.0f, -10.0f, 0.0f));
    this->CameraSpringArm->SocketOffset = FVector::ZeroVector;
	this->CameraSpringArm->SetRelativeLocation(FVector::ZeroVector);
    this->CameraSpringArm->bUsePawnControlRotation = true;
	this->CameraSpringArm->TargetArmLength = 0.0f;
	this->CameraSpringArm->AttachTo(this->GetRootComponent());

	this->CameraMainView = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("CameraMain"));
	this->CameraMainView->AttachTo(this->CameraSpringArm, USpringArmComponent::SocketName);
	this->CameraMainView->bUsePawnControlRotation = true;
    //this->CameraMainView->GetSocketTransform()


    //Note(Robert) This is needed for the AI to detect the machine
    //CollisionComp->BodyInstance.SetCollisionProfileName("Machine");
    //this->GetCollisionComponent()->SetCollisionProfileName("Machine");
}

// Called when the game starts or when spawned
void AMainCore::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMainCore::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void AMainCore::SetupPlayerInputComponent(UInputComponent* InputComponent){
	//Note(Thompson): Assertion checking macro
	check(InputComponent);

    InputComponent->BindAxis("MoveForward", this, &AMainCore::MoveForward);
    InputComponent->BindAxis("MoveRight", this, &AMainCore::MoveRight);

    InputComponent->BindAxis("LookUp", this, &AMainCore::AddControllerPitchInput);
    InputComponent->BindAxis("LookRight", this, &AMainCore::AddControllerYawInput);
}

void AMainCore::MoveForward(float Value){
    FRotator OurRotation = this->GetController()->GetControlRotation();
	OurRotation.Pitch = 0.0f;
    FVector Direction = OurRotation.RotateVector(FVector(1.0f, 0.0f, 0.0f));
    this->AddMovementInput(Direction, Value);
}

void AMainCore::MoveRight(float Value){
    FRotator OurRotation = this->GetController()->GetControlRotation();
    FVector Direction = OurRotation.RotateVector(FVector(0.0f, 1.0f, 0.0f));
    AddMovementInput(Direction, Value);
}

void AMainCore::ShowMessage(){
}