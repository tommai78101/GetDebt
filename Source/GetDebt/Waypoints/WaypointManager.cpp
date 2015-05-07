// Fill out your copyright notice in the Description page of Project Settings.

#include "GetDebt.h"
#include "Player/MainPlayerController.h"
#include "Player/Machine/MainCore.h"
#include "Waypoint.h"
#include "WaypointManager.h"

AWaypointManager::AWaypointManager(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {

    // Structure to hold one-time initialization
    struct FConstructorStatics {
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
            , NAME_Notes(NSLOCTEXT("SpriteCategory", "Notes", "Notes")) {}
    };
    static FConstructorStatics ConstructorStatics;

#if WITH_EDITORONLY_DATA
    //this->SpriteComponent = ObjectInitializer.CreateEditorOnlyDefaultSubobject<UBillboardComponent>(this, TEXT("Sprite"));
    if(GetSpriteComponent()) {
        GetSpriteComponent()->Sprite = ConstructorStatics.NoteTextureObject.Get();		// Get the sprite texture from helper class object
        GetSpriteComponent()->SpriteInfo.Category = ConstructorStatics.ID_Notes;		// Assign sprite category name
        GetSpriteComponent()->SpriteInfo.DisplayName = ConstructorStatics.NAME_Notes;	// Assign sprite display name
        GetSpriteComponent()->AttachParent = RootComponent;				        // Attach sprite to scene component
        GetSpriteComponent()->Mobility = EComponentMobility::Static;
    }
#endif // WITH_EDITORONLY_DATA

}

void AWaypointManager::BeginPlay(){
	Super::BeginPlay();
}

void AWaypointManager::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    TimeSinceWaypointUpdate += DeltaTime;

    if (TimeSinceWaypointUpdate > 5.0f)
    {
        TArray<AMainPlayerController*> PlayerControllers;
        for(TActorIterator<AMainCore> PlayerItr(this->GetWorld()); PlayerItr; ++PlayerItr)
        {
            AMainPlayerController* PlayerController = CastChecked<AMainPlayerController>((*PlayerItr)->GetController());
            PlayerControllers.Add(PlayerController);
        }

        AWaypoint::UpdateGivenValueForAllWaypoints(PlayerControllers, this->GetWorld());

		TimeSinceWaypointUpdate = 0.0f;
    }
}