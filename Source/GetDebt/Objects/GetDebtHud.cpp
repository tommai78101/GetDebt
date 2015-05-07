// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "GetDebt.h"
#include "GetDebtHud.h"
#include "GetDebtPawn.h"
#include "Engine/Canvas.h"
#include "Engine/Font.h"
#include "CanvasItem.h"
#include "../Player/Machine/MainCore.h"

#ifdef HMD_INTGERATION
// Needed for VR Headset
#include "Engine.h"
#include "IHeadMountedDisplay.h"
#endif // HMD_INTGERATION

#define LOCTEXT_NAMESPACE "VehicleHUD"

AGetDebtHud::AGetDebtHud(const FObjectInitializer& ObjectInitializer) 
	: Super(ObjectInitializer)
{
	static ConstructorHelpers::FObjectFinder<UFont> Font(TEXT("/Engine/EngineFonts/RobotoDistanceField"));
	HUDFont = Font.Object;
}

void AGetDebtHud::DrawHUD()
{
	Super::DrawHUD();

	// Calculate ratio from 720p
	const float HUDXRatio = Canvas->SizeX / 1280.f;
	const float HUDYRatio = Canvas->SizeY / 720.f;

	bool bHMDDeviceActive = false;

#ifdef HMD_INTGERATION
	// We dont want the onscreen hud when using a HMD device	
	if (GEngine->HMDDevice.IsValid() == true )
	{
		bHMDDeviceActive = GEngine->HMDDevice->IsStereoEnabled();
	}
#endif
	if( bHMDDeviceActive == false )
	{
		// Get our vehicle so we can check if we are in car. If we are we don't want onscreen HUD
		AGetDebtPawn* Vehicle = Cast<AGetDebtPawn>(GetOwningPawn());
		if ((Vehicle != nullptr) && (Vehicle->bInCarCameraActive == false))
		{
			FVector2D ScaleVec(HUDYRatio * 1.4f, HUDYRatio * 1.4f);

			// Speed
			FCanvasTextItem SpeedTextItem(FVector2D(HUDXRatio * 805.f, HUDYRatio * 455), Vehicle->SpeedDisplayString, HUDFont, FLinearColor::White);
			SpeedTextItem.Scale = ScaleVec;
			Canvas->DrawItem(SpeedTextItem);

			// Gear
			FCanvasTextItem GearTextItem(FVector2D(HUDXRatio * 805.f, HUDYRatio * 500.f), Vehicle->GearDisplayString, HUDFont, Vehicle->bInReverseGear == false ? Vehicle->GearDisplayColor : Vehicle->GearDisplayReverseColor);
			GearTextItem.Scale = ScaleVec;
			Canvas->DrawItem(GearTextItem);
		}

		//Note(Thompson): Just drawing text, nothing much.
		//AMainCore* MainCorePawn = Cast<AMainCore>(this->GetOwningPawn());
		//if (MainCorePawn != nullptr){
		//	FVector2D ScaleVector(HUDYRatio * 1.4f, HUDYRatio * 1.4f);
		//	FCanvasTextItem DebugMessageItem(FVector2D(HUDXRatio * (this->Canvas->SizeX / 2.0f), HUDXRatio * (this->Canvas->SizeY / 2.0f)), FText::FromString(TEXT("Hello HUD.")), this->HUDFont, FLinearColor::MakeRandomColor());
		//	DebugMessageItem.Scale = ScaleVector;
		//	this->Canvas->DrawItem(DebugMessageItem);
		//}
	}
}

#undef LOCTEXT_NAMESPACE
