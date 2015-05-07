// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.
#pragma once
#include "GameFramework/HUD.h"
#include "GetDebtHud.generated.h"


UCLASS(config = Game)
class AGetDebtHud : public AHUD
{
	GENERATED_BODY()

public:
	AGetDebtHud(const FObjectInitializer& ObjectInitializer);

	/** Font used to render the vehicle info */
	UPROPERTY()
	UFont* HUDFont;

	// Begin AHUD interface
	virtual void DrawHUD() override;
	// End AHUD interface

};
