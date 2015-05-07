// Fill out your copyright notice in the Description page of Project Settings.

#include "GetDebt.h"
#include "Player/MainPlayerController.h"
#include "EngineUtils.h"
#include "Waypoint.h"

// Init static variables
float AWaypoint::MaxHeuristicCost = 100.0f;
//float AWaypoint::TimeSinceLastUpdateForGivenValue = 0.0f;

// Sets default values
AWaypoint::AWaypoint(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	//Note(Robert): This is needed so the waypoint has a position
	this->RootComponent = ObjectInitializer.CreateDefaultSubobject<USceneComponent>(this, "DefaultSceneComponent");
	RootComponent->Mobility = EComponentMobility::Static;

	// Structure to hold one-time initialization
	struct FConstructorStatics
	{
		// A helper class object we use to find target UTexture2D object in resource package
		ConstructorHelpers::FObjectFinderOptional<UTexture2D> NoteTextureObject;

		// Icon sprite category name
		FName ID_Notes;

		// Icon sprite display name
		FText NAME_Notes;

		FConstructorStatics()
			// Use helper class object to find the texture
			// "/Engine/EditorResources/S_Note" is resource path
			: NoteTextureObject(TEXT("/Engine/EditorResources/S_NavP"))
			, ID_Notes(TEXT("Notes"))
			, NAME_Notes(NSLOCTEXT("SpriteCategory", "Notes", "Notes"))
		{
		}
	};
	static FConstructorStatics ConstructorStatics;

#if WITH_EDITORONLY_DATA
	//this->SpriteComponent = ObjectInitializer.CreateEditorOnlyDefaultSubobject<UBillboardComponent>(this, TEXT("Sprite"));
	if (GetSpriteComponent())
	{
		GetSpriteComponent()->Sprite = ConstructorStatics.NoteTextureObject.Get();		// Get the sprite texture from helper class object
		GetSpriteComponent()->SpriteInfo.Category = ConstructorStatics.ID_Notes;		// Assign sprite category name
		GetSpriteComponent()->SpriteInfo.DisplayName = ConstructorStatics.NAME_Notes;	// Assign sprite display name
		GetSpriteComponent()->AttachParent = RootComponent;				        // Attach sprite to scene component
		GetSpriteComponent()->Mobility = EComponentMobility::Static;
	}
#endif // WITH_EDITORONLY_DATA

#ifdef DEBUG
    static ConstructorHelpers::FObjectFinder<UStaticMesh> BodyMeshObject(TEXT("StaticMesh'/Game/Meshes/Sphere_Mesh.Sphere_Mesh'"));
    if(BodyMeshObject.Succeeded()) {
        this->WaypointSphere = ObjectInitializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("BodyMesh"));
        this->WaypointSphere->SetStaticMesh(BodyMeshObject.Object);
        //this->WaypointSphere->SetRelativeScale3D(Scale);
        this->WaypointSphere->SetRelativeLocation(FVector::ZeroVector);

        //this->WaypointSphere = ObjectInitializer.CreateDefaultSubobject<UMeshComponent>(this, TEXT("WaypointSphere"));
        this->WaypointSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
        this->WaypointSphere->AttachTo(this->RootComponent);

        static ConstructorHelpers::FObjectFinder<UMaterial> Material(TEXT("Material'/Game/StarterContent/Materials/M_Ground_Grass.M_Ground_Grass'"));
        if(Material.Object != NULL) {
            UMaterial* TheMaterial = (UMaterial*)Material.Object;
            UMaterialInstanceDynamic* TheMaterial_Dyn = UMaterialInstanceDynamic::Create(TheMaterial, this);
            this->WaypointSphere->SetMaterial(0, TheMaterial_Dyn);
        }
    }
#endif
}

//TODO(Robert): Uncomment when we have a filter inplace
// Called when the game starts or when spawned
void AWaypoint::BeginPlay()
{
	Super::BeginPlay();

	while (this->AccessibleWaypoints.Num() > 0){
		this->AccessibleWaypoints.Pop();
	}

	//Getting all Waypoints in the level.
	TArray<AActor*> WaypointsArray;
	UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), AWaypoint::StaticClass(), WaypointsArray);
	if (WaypointsArray.Num() <= 0){
		//It should be more than 0.
		check(0 && "Waypoint Array could not get all waypoints.");
		return;
	}

	AGameMode* GameMode = this->GetWorld()->GetAuthGameMode();
	AGetDebtGameMode* GetDebtGameMode = CastChecked<AGetDebtGameMode>(GameMode);
	if (GetDebtGameMode != nullptr || GetDebtGameMode != NULL){
		for (auto Iterator = WaypointsArray.CreateIterator(); Iterator; ++Iterator){
			AActor* Actor = *Iterator;
			AWaypoint* Waypoint = CastChecked<AWaypoint>(Actor);
			if (Waypoint == nullptr){
				check(0 && "Waypoint is returning null for some reasons.");
			}

			if (Waypoint != this){
				FHitResult HitResult;
				FCollisionQueryParams CollisionParams(FName(TEXT("Line Trace")), true, this);
				CollisionParams.bTraceComplex = true;
				CollisionParams.bTraceAsyncScene = true;
				CollisionParams.bReturnPhysicalMaterial = false;
				FCollisionObjectQueryParams ObjectParams;
				ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic); 
				this->GetWorld()->LineTraceSingle(HitResult, this->GetActorLocation(), Waypoint->GetActorLocation(), CollisionParams, ObjectParams);
				if (!HitResult.bBlockingHit){
					this->AccessibleWaypoints.Add(Waypoint); //Stack

//#ifdef DEBUG
//					DrawDebugLine(this->GetWorld(), this->GetActorLocation(), Waypoint->GetActorLocation(), FColor::Yellow, true, -1.0f, (uint8)'\000', 15.0f);
//#endif
				}
			}
		}
		FVector FlatLocation = this->GetActorLocation();
		FlatLocation.Z = 0.0f;
		this->SetActorLocation(FlatLocation);
		GetDebtGameMode->WaypointsArray.Push(this);
	}
}

// Called every frame
void AWaypoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWaypoint::UpdateGivenValueForAllWaypoints(TArray<AMainPlayerController*> PlayerControllers, UWorld* World){

    for (TActorIterator<AWaypoint> WaypointItr(World); WaypointItr; ++WaypointItr){
        AWaypoint* CurrentWaypoint = *WaypointItr;
        float DistanceToClosestPlayer = MaxHeuristicCost;
		float DistanceToFarthestPlayer = 0.0f;
        FVector CurrentWaypointLocation = CurrentWaypoint->GetActorLocation();

        for (auto PlayerControllerItr = PlayerControllers.CreateIterator(); PlayerControllerItr; PlayerControllerItr++)
        {
            AMainPlayerController* PlayerController = (*PlayerControllerItr);
            FVector CurrentPlayerLocation = PlayerController->GetPawn()->GetActorLocation();

            float CurrentDistance = FVector::Dist(CurrentWaypointLocation, CurrentPlayerLocation);

            if(DistanceToClosestPlayer > CurrentDistance) {
                DistanceToClosestPlayer = CurrentDistance;
            }

            //Update the player controller with the waypoint info if we are the farthest away
            if (PlayerController->FarthestWaypoint == NULL || PlayerController->FarthestWaypoint == nullptr){
				//FIXME: What do we do here?
            } 
			else if (PlayerController->FarthestWaypoint != CurrentWaypoint && PlayerController->DistanceToFarthestWaypoint < CurrentDistance) {
                PlayerController->FarthestWaypoint = CurrentWaypoint;
                PlayerController->DistanceToFarthestWaypoint = CurrentDistance;
            }
        }

        CurrentWaypoint->HeuristicCost = MaxHeuristicCost - DistanceToClosestPlayer;
    }

    //TimeSinceLastUpdateForGivenValue = 
}
