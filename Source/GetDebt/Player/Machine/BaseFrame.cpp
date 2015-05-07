// Fill out your copyright notice in the Description page of Project Settings.

#include "GetDebt.h"
#include "BaseFrame.h"
#include "../../GetDebtGameMode.h"
#include "Person/PersonCharacter.h"
#include "Person/PersonAIController.h"


// Sets default values
ABaseFrame::ABaseFrame(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->TorqueDirection = FVector::ZeroVector;
	this->WheelCount = 2;
	FVector Positions[2] = {
		FVector(0.0f, 70.0f, 0.0f),
		FVector(0.0f, -70.0f, 0.0f)
	};
	FName WheelNames[2] = {
		FName("LeftWheel"),
		FName("RightWheel")
	};
	FName ConstraintNames[2] = {
		FName("LeftConstraint"),
		FName("RightConstraint")
	};

	this->FrameCollider = ObjectInitializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("FrameBoxCollider"));
	if (this->FrameCollider){
		this->FrameCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		this->FrameCollider->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
		this->FrameCollider->SetSimulatePhysics(true);
		//this->FrameCollider->SetEnableGravity(false);
		this->FrameCollider->SetAngularDamping(100.0f);
		//this->FrameCollider->SetLinearDamping(100.0f);
		//this->FrameCollider->SetMassScale(NAME_None, 2.5f);
		this->SetRootComponent(this->FrameCollider);
		this->GetRootComponent()->SetRelativeLocation(FVector::ZeroVector);
	}

	//static ConstructorHelpers::FObjectFinder<UStaticMesh> FrameMeshObject(TEXT("StaticMesh'/Game/Models/Objects/Hero/Machine_V05_Textured.Machine_V05_Textured'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FrameMeshObject(TEXT("StaticMesh'/Game/Models/Objects/Machine_Claw/machine_smooth_CLAW.machine_smooth_CLAW'"));
	if (FrameMeshObject.Succeeded()){
		this->FrameMeshComponent = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("Frame"));
		this->FrameMeshComponent->SetStaticMesh(FrameMeshObject.Object);
		this->FrameMeshComponent->SetRelativeLocation(FVector::ZeroVector);
		this->FrameMeshComponent->SetWorldRotation(FRotator(-90.0f, 0.0f, 0.0f));
        this->FrameMeshComponent->SetWorldScale3D(FVector(1.0f));
		this->FrameMeshComponent->SetAngularDamping(10.0f);
		//this->FrameMeshComponent->SetCollisionProfileName(UCollisionProfile::NoCollision_ProfileName);
		this->FrameMeshComponent->AttachTo(this->FrameCollider);
		//this->FrameMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);	}
    }

	static ConstructorHelpers::FObjectFinder<UStaticMesh> WheelMeshObject1(TEXT("StaticMesh'/Game/Models/Objects/Wheel/Static_Object_Wheel_90.Static_Object_Wheel_90'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WheelMeshObject2(TEXT("StaticMesh'/Game/Models/Objects/Wheel/Static_Object_Wheel_90_Rotated.Static_Object_Wheel_90_Rotated'"));
	if (WheelMeshObject1.Succeeded() && WheelMeshObject2.Succeeded()){
		
		for (int32 i = 0; i < this->WheelCount; i++){
			UStaticMeshComponent* MeshComponent = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, WheelNames[i]);
			if (i % this->WheelCount == 0){
				MeshComponent->SetStaticMesh(WheelMeshObject1.Object);
			}
			else {
				MeshComponent->SetStaticMesh(WheelMeshObject2.Object);
			}
			MeshComponent->SetSimulatePhysics(true);
			MeshComponent->SetAngularDamping(0.04f);
			MeshComponent->SetLinearDamping(0.04f);
			//MeshComponent->SetRelativeRotation(FRotator(90.0f, 90.0f, 0.0f));
			MeshComponent->SetCollisionProfileName(UCollisionProfile::PhysicsActor_ProfileName);
			//MeshComponent->AttachTo(this->GetRootComponent(), this->GetRootComponent()->AttachSocketName);
			MeshComponent->AttachParent = this->GetRootComponent();
			MeshComponent->SetRelativeLocation(Positions[i]);
			MeshComponent->SetRelativeScale3D(FVector(1.0f));

			UPhysicsConstraintComponent* PhysicsConstraintComponent = ObjectInitializer.CreateDefaultSubobject<UPhysicsConstraintComponent>(this, ConstraintNames[i]);
			PhysicsConstraintComponent->SetWorldLocation(this->GetActorLocation()); //Set world location.
			PhysicsConstraintComponent->AttachTo(this->GetRootComponent(), NAME_None, EAttachLocation::SnapToTarget); //Attach to root.
			PhysicsConstraintComponent->SetConstrainedComponents(this->FrameMeshComponent, NAME_None, MeshComponent, NAME_None);
			//PhysicsConstraintComponent->ConstraintInstance.bDisableCollision = true;
			PhysicsConstraintComponent->SetAngularSwing1Limit(EAngularConstraintMotion::ACM_Locked, 0.0f);
			PhysicsConstraintComponent->SetAngularSwing2Limit(EAngularConstraintMotion::ACM_Free, 0.0f);
			PhysicsConstraintComponent->SetAngularTwistLimit(EAngularConstraintMotion::ACM_Locked, 0.0f);

            MeshComponent->SetPhysicsMaxAngularVelocity(400.0f);

			this->Wheels.Add(MeshComponent);
		}
	}

	this->SpringArm = ObjectInitializer.CreateDefaultSubobject<USpringArmComponent>(this, TEXT("SpringArm"));
	if (this->SpringArm){
		this->SpringArm->SetRelativeLocation(FVector::ZeroVector); 
		this->SpringArm->TargetArmLength = 1500.0f; //Needs to make camera be further out.
		this->SpringArm->SetRelativeRotation(FRotator(-50.0f, 0.0f, 0.0f));
		this->SpringArm->bUsePawnControlRotation = true;
		this->SpringArm->bAbsoluteRotation = false; //This flag allows the spring arm to not get affected by rotation of parent component.
		this->SpringArm->bDoCollisionTest = true; //This flag stops objects from hitting and moving the spring arm unexpectedly.
		this->SpringArm->AttachTo(this->GetRootComponent());
	}

	this->MainCamera = ObjectInitializer.CreateDefaultSubobject<UCameraComponent>(this, TEXT("MainCamera"));
	if (this->MainCamera){
		this->MainCamera->SetRelativeLocation(FVector::ZeroVector);
		this->MainCamera->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
		this->MainCamera->bUsePawnControlRotation = false;
		this->MainCamera->AttachTo(this->SpringArm, USpringArmComponent::SocketName);
	}

	//Capture Area
	this->CaptureArea = ObjectInitializer.CreateDefaultSubobject<USphereComponent>(this, TEXT("Capture Area"));
	this->CaptureArea->SetSphereRadius(100.0f);
	this->CaptureArea->RelativeLocation = FVector(140.0f, 0.0f, 0.0f);
	this->CaptureArea->OnComponentBeginOverlap.AddDynamic(this, &ABaseFrame::OnBeginOverlap);
	this->CaptureArea->SetCollisionProfileName("OverlapOnlyPawn");
	this->CaptureArea->bShouldUpdatePhysicsVolume = true;
	this->CaptureArea->AttachTo(this->RootComponent);

    //this->CaptureSound = ObjectInitializer.CreateDefaultSubobject<USoundCue>(this, TEXT("EngineSound"));
    this->CaptureSound = CreateDefaultSubobject<USoundCue>(TEXT("CaptureSound"));
    static ConstructorHelpers::FObjectFinder<USoundCue> CaptureSoundFile(TEXT("SoundCue'/Game/Audio/Triggerables/CaptureCue.CaptureCue'"));
    if(CaptureSoundFile.Succeeded()) {
        this->CaptureSound = CaptureSoundFile.Object;
        //this->CaptureSound->Stop();
    }

    //this->EngineSound = ObjectInitializer.CreateDefaultSubobject<USoundCue>(this, TEXT("EngineSound"));
    
    static ConstructorHelpers::FObjectFinder<USoundCue> EngineSoundFile(TEXT("SoundCue'/Game/VehicleAdv/Sound/Engine_Loop_Cue.Engine_Loop_Cue'"));
    this->EngineSound = CreateDefaultSubobject<UAudioComponent>(TEXT("EngineSound"));
    if(EngineSoundFile.Succeeded()) {
        this->EngineSound->SetSound(EngineSoundFile.Object);
        this->EngineSound->SetFloatParameter(TEXT("RPM"), 0.0f);
    }

    //this->EngineSound->
	//this->GetPawnPhysicsVolume()->UpdateS

	this->GetRootComponent()->SetWorldScale3D(FVector(2.0f));
}

// Called when the game starts or when spawned
void ABaseFrame::BeginPlay()
{
	Super::BeginPlay();

	//Note(Thompson): Will return NULL if this is called on a network client.
	//We need to have some global references in our game, so each actor can interact with all of them nicely.
	AGetDebtGameMode* GetDebtMode = (AGetDebtGameMode*) this->GetWorld()->GetAuthGameMode();
	if (GetDebtMode != NULL || GetDebtMode != nullptr){
		GetDebtMode->MainBaseFrame = this;
	}
}

void ABaseFrame::OnConstruction(const FTransform& Transform){
	Super::OnConstruction(Transform);
}

// Called every frame
void ABaseFrame::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
	//this->FrameMeshComponent->SetWorldRotation(FRotator(-90.0f, 0.0f, 0.0f));

	FVector LeftWheelLocation = this->Wheels[0]->GetComponentLocation();
	FVector RightWheelLocation = this->Wheels[1]->GetComponentLocation();
	FVector Axle = LeftWheelLocation - RightWheelLocation;
	FVector RotatedYVector = FVector::CrossProduct(Axle, FVector::UpVector);
	this->FrameMeshComponent->SetWorldRotation(FRotator(0.0f, RotatedYVector.Rotation().Yaw, 0.0f));

    //Convert velocity to "RPM"
    FVector CurrentVelocity = this->GetVelocity();
    CurrentVelocity.Z = 0;
    float CurVelocity = CurrentVelocity.Size();

    this->EngineSound->SetFloatParameter(TEXT("RPM"), CurVelocity);
}

// Called to bind functionality to input
void ABaseFrame::SetupPlayerInputComponent(class UInputComponent* InputComponent) {
    Super::SetupPlayerInputComponent(InputComponent);

    InputComponent->BindAxis("TurnWheel", this, &ABaseFrame::Turn);
    InputComponent->BindAxis("SteerWheel", this, &ABaseFrame::Steer);
    InputComponent->BindAxis("Swivel", this, &ABaseFrame::Swivel);

    InputComponent->BindAxis("LookUp", this, &ABaseFrame::AddControllerPitchInput);
    InputComponent->BindAxis("LookRight", this, &ABaseFrame::AddControllerYawInput);

    InputComponent->BindAxis("CameraZoom", this, &ABaseFrame::AddCameraZoom);

}

void ABaseFrame::Turn(float Value){
	if ((this->GetController() != nullptr) && (Value != 0.0f)){
		for (UStaticMeshComponent* Wheel : this->Wheels){
			const float Scalar = Value * 50000000.0f * 7.0f;
			this->TorqueDirection = FVector(0.0f, 1.0f, 0.0f);  //Torque vector needs to be normalized before rotating, and then afterwards apply torque to component.
			FRotator Rotation = Wheel->GetComponentRotation();
			this->TorqueDirection = Rotation.RotateVector(this->TorqueDirection);
			FVector Torque = this->TorqueDirection * Scalar;
			Wheel->AddTorque(Torque);
		}
	}
}

void ABaseFrame::Steer(float Value){
	if ((this->GetController() != nullptr) && (Value != 0.0f)){
		const float Scalar = Value * 50000000.0f * 1.6f * 7.0f;
		this->TorqueDirection = FVector(0.0f, 1.0f, 0.0f);  //Torque vector needs to be normalized before rotating, and then afterwards apply torque to component.
		FRotator Rotation = this->FrameMeshComponent->GetComponentRotation();
		this->TorqueDirection = Rotation.RotateVector(this->TorqueDirection);
		FVector Torque = this->TorqueDirection * Scalar;
		this->Wheels[0]->AddTorque(Torque * -1.0f);
		this->Wheels[1]->AddTorque(Torque);
	}
}

void ABaseFrame::Swivel(float Value){
}

void ABaseFrame::OnBeginOverlap(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(APersonCharacter::StaticClass()))
	{
		APersonCharacter* PersonCharacter = CastChecked<APersonCharacter>(OtherActor);
		if (PersonCharacter != nullptr && !PersonCharacter->IsPendingKill()){
			this->PersonCaught(this, PersonCharacter);
		}
	}
}

/** Returns true if the caught person is destroyed. Else, return false.*/
bool ABaseFrame::PersonCaught(ABaseFrame* CatchingPlayer, APersonCharacter* CaughtPerson){

	//TODO Check which # the CatchingPlayer is
	AGameMode* GameMode = UGameplayStatics::GetGameMode(this->GetWorld());
	AGetDebtGameMode* OurGameMode = CastChecked<AGetDebtGameMode>(GameMode);
	if (OurGameMode != nullptr || OurGameMode != NULL){
		OurGameMode->PlayerOneScore++;
		AActor* Owner = CaughtPerson->GetOwner();
		if (Owner != nullptr){
			APersonAIController* AIController = CastChecked<APersonAIController>(CaughtPerson->GetOwner());
			CaughtPerson->SetOwner(AIController); //Why is this here?
		}
       
        UGameplayStatics::PlaySoundAtLocation(this->GetWorld(), CaptureSound, CaughtPerson->GetActorLocation());
		CaughtPerson->Destroy();

		return true;
	}
	return false;
}

void ABaseFrame::AddCameraZoom(float Val)
{
    if(Val != 0.f && Controller && Controller->IsLocalPlayerController()) {
        Val *= 10.0f;
        float NewArmLength = this->SpringArm->TargetArmLength + Val;
        NewArmLength = FMath::Clamp(NewArmLength, 800.0f, 1800.0f);
        this->SpringArm->TargetArmLength = NewArmLength;
    }
}