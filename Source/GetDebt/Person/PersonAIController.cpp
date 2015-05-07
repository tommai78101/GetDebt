// Fill out your copyright notice in the Description page of Project Settings.

#include "GetDebt.h"
#include "PersonCharacter.h"
#include "../Person/States/FiniteStateMachine.h"
#include "../Waypoints/Waypoint.h"
#include "PersonAIController.h"

APersonAIController::APersonAIController(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer) {
	//this->StateMachine = ObjectInitializer.CreateDefaultSubobject<AFiniteStateMachine>(this, TEXT("StateMachine"));
	this->PrimaryActorTick.bCanEverTick = true;
}

void APersonAIController::BeginPlay(){

	//Note(Robert) Since this is inherited from Actor to get a BeginPlay and Tick, this needs to be here, and not in the constructor
	this->StateMachine = this->GetWorld()->SpawnActor<AFiniteStateMachine>();
	//this->StateMachine->BeginPlay();
	this->StateMachine->OwningController = this;

	//Note(Robert) Find all players in the game
	TArray<AActor*> PlayerControllerArray;
	UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), AMainPlayerController::StaticClass(), PlayerControllerArray);

	//Note(Robert) Error check
	if (PlayerControllerArray.Num() > 2){
		ERROR("Found more than 2 players!");
	}

	for (auto Itr = PlayerControllerArray.CreateIterator(); Itr; Itr++)
	{
		//Note(Robert) Check if valid
		if (!*Itr) continue;
		if (!(*Itr)->IsValidLowLevel()) continue;
		//~~~~~~~~~~~~~~~~~~~~~~

		AMainPlayerController* CurPlayerController = Cast<AMainPlayerController>(*Itr);

		//Two scenarios
		//Note(Thompson): Fixed a bug where ClosestPlayerController is null upon initialization.
		if (ClosestPlayerController == NULL || ClosestPlayerController == nullptr || ClosestPlayerController == 0) {
			//1) It's the first player so it's the closest
			ClosestPlayerController = CurPlayerController;
		}
		else {
			//2) We have to compare and figure out which player is closest

			float ClosestPlayerDistance = this->GetPawn()->GetDistanceTo(ClosestPlayerController->GetPawn());
			float CurrentPlayerDistance = this->GetPawn()->GetDistanceTo(CurPlayerController->GetPawn());

			//2a) Previous ClosestPlayer is still the closest
			if (ClosestPlayerDistance <= CurrentPlayerDistance){
				OtherPlayerController = CurPlayerController;
			}
			else { //2b) CurPlayer is actually the closest;
				OtherPlayerController = ClosestPlayerController;
				ClosestPlayerController = CurPlayerController;
			}
		}
	}

	//FIXME(Thompson): No pawns for PersonAIController to possess.
	//check(0 && "No pawns for PersonAIController to possess. We need to fix this in PersonAIController::BeginPlay(), or in the constructor.");
	APersonCharacter* Pawn = Cast<APersonCharacter>(this->GetPawn());
	if (Pawn != nullptr){
		if (Pawn->GetOwner() == nullptr){
			this->Possess(Pawn);
			Pawn->SetOwner(this);

			TArray<AWaypoint*> WaypointsArray = this->CalculatePathToTargetWaypoint(this->FindClosestWaypoint());
			if (WaypointsArray.Num() > 0){
				FVector TempLocation = WaypointsArray.Pop()->GetActorLocation();
				TempLocation.Z = 0.0f;
				Pawn->TargetLocation = TempLocation;
			}
		}
	}
}

void APersonAIController::Tick(float DeltaSeconds){
	this->TimeSinceLastControllerUpdate += DeltaSeconds;
	if (this->TimeSinceLastControllerUpdate > 5.0f){
		this->UpdateClosestPlayerController();
		this->TimeSinceLastControllerUpdate = 0.0f;
	}

	this->StateMachine->Tick(DeltaSeconds);

	//Follow path
	//LOG("(PersonAIController.cpp:105) CurrentPath Length: " + FString::FromInt(CurrentPath.Num()));
	if (this->CurrentPath.Num() > 0)
	{
		APersonCharacter* Pawn = CastChecked<APersonCharacter>(this->GetPawn());
		//LOG("Pawn is: " + Pawn->GetName());
		if (Pawn->IsAtTargetLocation()){
			//LOG("Pawn is at target location. Moving to next waypoint.");
			AWaypoint* CurrentTargetWP = this->CurrentPath.Pop();
			this->TravelTowardsWaypoint(CurrentTargetWP);
		}
	}
}

void APersonAIController::UpdateClosestPlayerController() {
	//We have to compare and figure out which player is closest
	float ClosestPlayerDistance = 0.0f;
	float OtherPlayerDistance = 0.0f;
	if (ClosestPlayerController != nullptr){
		APawn* ClosestPawn = ClosestPlayerController->GetPawn();
		if (ClosestPawn != nullptr){
			ClosestPlayerDistance = this->GetPawn()->GetDistanceTo(ClosestPawn);
		}
	}
	if (OtherPlayerController != nullptr){
		APawn* ClosestPawn = OtherPlayerController->GetPawn();
		if (ClosestPawn != nullptr){
			OtherPlayerDistance = this->GetPawn()->GetDistanceTo(ClosestPawn);
		}
	}


	if (OtherPlayerController != nullptr && ClosestPlayerController != nullptr){
		//CurPlayer is actually the closest;
		if (OtherPlayerDistance < ClosestPlayerDistance) {
			AMainPlayerController * Temp = ClosestPlayerController;
			ClosestPlayerController = OtherPlayerController;
			OtherPlayerController = Temp;
		}
	}
}


AWaypoint* APersonAIController::FindClosestWaypoint(){
	AWaypoint* ClosestWaypoint = nullptr;
	TArray<AActor*> WaypointArray = TArray<AActor*>();
	UGameplayStatics::GetAllActorsOfClass(this->GetWorld(), AWaypoint::StaticClass(), WaypointArray);

	if (WaypointArray.Num() == 0)
	{
		ERROR("Can't find closest waypoint");
		return ClosestWaypoint;
	}

	for (auto ArrayItr = WaypointArray.CreateIterator(); ArrayItr; ArrayItr++)
	{
		AWaypoint* CurrentWaypoint = CastChecked<AWaypoint>(*ArrayItr);

		if (ClosestWaypoint == nullptr) {
			ClosestWaypoint = CurrentWaypoint;
		}
		else if (this->GetPawn() != nullptr){
			float DistanceToClosestWaypoint = FVector::Dist(ClosestWaypoint->GetActorLocation(), this->GetPawn()->GetActorLocation());
			float DistanceToCurrentWaypoint = FVector::Dist(CurrentWaypoint->GetActorLocation(), this->GetPawn()->GetActorLocation());

			if (DistanceToClosestWaypoint > DistanceToCurrentWaypoint) {
				ClosestWaypoint = CurrentWaypoint;
			}
		}
	}
	return ClosestWaypoint;
}

TArray<AWaypoint*> APersonAIController::CalculatePathToTargetWaypoint(AWaypoint* Goal){
	//TODO(Robert) Use A* Star here
	////Note(Thompson): Use the following link as a reference for A*. Find the method, "createPath()".
	///*
	//https://github.com/tommai78101/Pixavive/blob/master/src/pathfinding/Area.java
	//*/
	//(Robert): I also used this as reference: http://www.redblobgames.com/pathfinding/a-star/implementation.html#sec-2-4

	//LOG("Calculating new path using A* Star");

	AWaypoint* Start = this->FindClosestWaypoint(); //Start
	//AWaypoint* TargetWaypoint = ClosestPlayerController->FarthestWaypoint; //Goal
	TArray<AWaypoint*> PreviouslyVisitedWaypoints; //Opened list
	TMap<AWaypoint*, AWaypoint*> BestRouteToWaypoint; //Key = Destination, Value = Previous Location
	TArray<AWaypoint*> EdgeNodes; //Nearest neighbors/Frontier

	TMap<AWaypoint*, float> CostSoFarMap; //Total cost

	EdgeNodes.HeapPush(Start, WaypointPriorityPredicate()); //Starting node
	CostSoFarMap.Add(Start, 0.0f); //It costs use nothing to stand still
	BestRouteToWaypoint.Add(Start, Start);

	AWaypoint* CurrentWaypoint;

	while (EdgeNodes.Num() > 0)
	{
		//Check to make sure we aren't already at our destination
		EdgeNodes.HeapPop(CurrentWaypoint, WaypointPriorityPredicate()); //Pop into CurrentWaypoint

		if (CurrentWaypoint == Goal)
		{
			break;
		}

		for (auto NeighborItr = CurrentWaypoint->AccessibleWaypoints.CreateConstIterator(); NeighborItr; NeighborItr++)
		{
			AWaypoint* Neighbor = *NeighborItr;
			float GivenCost = FVector::Dist(CurrentWaypoint->GetActorLocation(), Neighbor->GetActorLocation());
			float Cost = CostSoFarMap.FindChecked(CurrentWaypoint) + GivenCost;

			if (!CostSoFarMap.Contains(Neighbor) || Cost < CostSoFarMap.FindChecked(Neighbor))
			{
				CostSoFarMap.Add(Neighbor, Cost);
				float Priority = Neighbor->HeuristicCost + Cost;
				Neighbor->Priority = Priority;
				EdgeNodes.HeapPush(Neighbor, WaypointPriorityPredicate());
				BestRouteToWaypoint.Add(Neighbor, CurrentWaypoint);
			}
		}
	}

	//Compose Path from BestRoute Map
	TArray<AWaypoint*> PathToFollow;
	CurrentWaypoint = Goal;
	PathToFollow.Add(CurrentWaypoint); //Add inserts the item at the end

	while (CurrentWaypoint != Start)
	{
		//PathToFollow.Add(CurrentWaypoint);
		CurrentWaypoint = BestRouteToWaypoint.FindChecked(CurrentWaypoint);
		PathToFollow.Add(CurrentWaypoint);
	}


	// #####  PATH SMOOTHING  #######

	TArray<AWaypoint*> WaypointsToRemove;

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams(FName(TEXT("Line Trace")), true, this);
	CollisionParams.bTraceComplex = true;
	CollisionParams.bTraceAsyncScene = true;
	CollisionParams.bReturnPhysicalMaterial = false;
	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECollisionChannel::ECC_WorldDynamic);

	if (PathToFollow.Num() > 1){
		for (auto Iterator = PathToFollow.CreateIterator(); Iterator; ++Iterator){
			AWaypoint* Waypoint = *Iterator;
			if (Waypoint == nullptr){
				continue;
			}

			for (auto Iterator2 = Iterator + 1; Iterator2; ++Iterator2){
				AWaypoint* Second = *Iterator2;
				if (Second == nullptr){
					continue;
				}
				
				this->GetWorld()->LineTraceSingle(HitResult, Waypoint->GetActorLocation(), (Second)->GetActorLocation(), CollisionParams, ObjectParams);

				if (!HitResult.bBlockingHit){
					WaypointsToRemove.Push(Second);
					continue;
				}
				else {
					if (WaypointsToRemove.Num() > 0){
						WaypointsToRemove.Pop();
					}
					break;
				}
			}
		}
	
		for (auto Iterator = PathToFollow.CreateIterator(); Iterator; ++Iterator){
			AWaypoint* First = *Iterator;
			if (First == nullptr){
				continue;
			}
			for (auto Iterator2 = WaypointsToRemove.CreateIterator(); Iterator2; ++Iterator2){
				AWaypoint* Second = *Iterator2;
				if (Second == nullptr){
					continue;
				}
				if (First == Second){
					PathToFollow.Remove(First);
				}
			}
		}
	}

	// #####  PATH SMOOTHING  <END>   #######

	//PathToFollow.Add(Start);

	return PathToFollow;
	//this->CurrentPath = PathToFollow;

}

void APersonAIController::TravelTowardsWaypoint(AWaypoint* Waypoint){
	FVector TargetLocation = Waypoint->GetActorLocation();
	if (TargetLocation.Z != 0.0f){
		TargetLocation.Z = 0.0f;
	}
	APersonCharacter* Pawn = Cast<APersonCharacter>(GetPawn());
	Pawn->MoveTowardsTarget(TargetLocation);
}