// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.

#ifndef __GetDebtADV_H__
#define __GetDebtADV_H__

#include "Engine.h"

//Uncomment to turn on debug helpers
//#define DEBUG

#define COLLISION_MACHINE    ECC_GameTraceChannel1

#ifdef DEBUG
#	define ERROR(Message) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, Message);} //TODO can we save this to a file?
#	define LOG(Message) if(GEngine){GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, Message);}

#	include "DrawDebugHelpers.h"
#else
#	define ERROR(Message)
#	define LOG(Message)
#endif

#endif
