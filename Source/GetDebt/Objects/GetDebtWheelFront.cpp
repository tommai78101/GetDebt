// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#include "GetDebt.h"
#include "GetDebtWheelFront.h"

UGetDebtWheelFront::UGetDebtWheelFront(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	ShapeRadius = 18.f;
	ShapeWidth = 15.0f;
	bAffectedByHandbrake = false;
	SteerAngle = 40.f;
}
