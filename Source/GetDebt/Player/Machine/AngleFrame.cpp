// Fill out your copyright notice in the Description page of Project Settings.

#include "GetDebt.h"
#include "AngleFrame.h"

AAngleFrame::AAngleFrame(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer){
	//Always enabled.
	this->PrimaryActorTick.bCanEverTick = true;
	this->OldLocation = FVector::ZeroVector;

	this->FrameCollider = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("FrameBoxCollider"));
	if (this->FrameCollider){
		this->FrameCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		this->FrameCollider->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
		this->FrameCollider->SetSimulatePhysics(true);
		this->FrameCollider->SetAngularDamping(50.0f);
		this->SetActorEnableCollision(true);
		this->SetRootComponent(this->FrameCollider);
	}

	static ConstructorHelpers::FObjectFinder<UStaticMesh> FrameMeshObject(TEXT("StaticMesh'/Game/Meshes/Test_Cube_Mesh.Test_Cube_Mesh'"));
	if (FrameMeshObject.Succeeded()){
		this->FrameMeshComponent = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Frame"));
		this->FrameMeshComponent->SetStaticMesh(FrameMeshObject.Object);
		this->FrameMeshComponent->SetRelativeLocation(FVector::ZeroVector);
		this->FrameMeshComponent->SetAngularDamping(10.0f);
		this->FrameMeshComponent->AttachTo(this->GetRootComponent());
	}

	this->MaxSwivelAngle = 180.0f;
	this->MinSwivelAngle = -180.0f;
}

void AAngleFrame::BeginPlay(){
	this->CurrentSwivelAngle = (this->MaxSwivelAngle + this->MinSwivelAngle) / 2.0f;
}

void AAngleFrame::OnConstruction(const FTransform& Transform){
	this->FrameCollider->SetMassScale(NAME_None, 0.2f);
}

void AAngleFrame::Tick(float Delta){
	Super::Tick(Delta);
	//this->FrameCollider->AddForce(FVector(0.0f, 0.0f, 100000.0f));
}

void AAngleFrame::SetupPlayerInputComponent(class UInputComponent* InputComponent){
	Super::SetupPlayerInputComponent(InputComponent);
}


void AAngleFrame::PositiveSwivel(float Value){
	if (Value > 0.0f){
		//LOG("It swivels positively.");
	}
}

void AAngleFrame::NegativeSwivel(float Value){
	if (Value < 0.0f){
		//LOG("It swivels negatively.");
	}
}

void AAngleFrame::SetNumberTag(int32 Value){
	this->NumberTag = Value;
}