#include "NPC_Controller.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

void ANPC_Controller::BeginPlay()
{
	Super::BeginPlay();
	NPC_State = ENPC_State::WANDER;
	WanderMaxDistance = 1000;
	WaypointRadius = 100; 
	NextWaypointIndex = 0;
	WaitTimeMin = 0.5;
	WaitTimeMax = 3.0;
	
	// find all waypoints
	UGameplayStatics::GetAllActorsWithTag(GetWorld(),FName("Waypoint"),Waypoints);
	
	// Select a new destination immediately
	NewDestination();
}

void ANPC_Controller::NewDestination()
{
	
	FNavLocation NavLocation;
	bool bSuccess;

		
	// get reference to the navigation system
	// note: must add NavigationSystem to the list of module dependencies in .Build.cs
	NavigationSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
	
	switch (NPC_State)
	{
		case ENPC_State::WANDER:
			bSuccess = Wander(NavLocation);
			break;
		case ENPC_State::ORDERED_WAYPOINT:
			bSuccess = OrderedWaypoint(NavLocation);
			break;
		case ENPC_State::RANDOM_WAYPOINT:
			bSuccess=RandomWaypoint(NavLocation);
			break;
	}

	if (bSuccess)
	{
		// initiate movement to new destination
		MoveToLocation(NavLocation.Location);
	}
	else
	{
		SetTimerForNextMove();		
	}
}

bool ANPC_Controller::Wander(FNavLocation& Destination)
{
	bool success = NavigationSystem->GetRandomReachablePointInRadius(GetCharacter()->GetActorLocation(), WanderMaxDistance, Destination);
	return success;
}

bool ANPC_Controller::OrderedWaypoint(FNavLocation& Destination)
{
	if (NextWaypointIndex < Waypoints.Num())
	{
		AActor* Waypoint = Waypoints[NextWaypointIndex];
		
		bool success = NavigationSystem->GetRandomReachablePointInRadius(Waypoint->GetActorLocation(), WaypointRadius, Destination);
		if (success)
		{
			UE_LOG(LogTemp, Display, TEXT("Curent Destination: %d Location: %f %f"),NextWaypointIndex, Destination.Location.X, Destination.Location.Y);
			NextWaypointIndex = (NextWaypointIndex+1) % Waypoints.Num();
			
			UE_LOG(LogTemp, Display, TEXT("Next waypoint: %d"), NextWaypointIndex);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Get Random Point Failed: %f %f"), Waypoint->GetActorLocation().X, Waypoint->GetActorLocation().Y);
		}
		return success;
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Waypoint Array Size Error"))
		return false;
	}
}

bool ANPC_Controller::RandomWaypoint(FNavLocation& Destination)
{
	bool success = false;
	if (Waypoints.Num() > 0)
	{
		int RandomIndex = FMath::RandRange(0, Waypoints.Num()-1);
		AActor* Waypoint = Waypoints[RandomIndex];
		success = NavigationSystem->GetRandomReachablePointInRadius(Waypoint->GetActorLocation(), WaypointRadius, Destination);
		if (success)
		{
			NextWaypointIndex = (NextWaypointIndex+1) % Waypoints.Num();
		}
	}
	return success;
}

void ANPC_Controller::SetTimerForNextMove()
{
	float Delay = FMath::RandRange(.25f, 3.0f);
	GetWorld()->GetTimerManager().SetTimer(MoveDelayTimer,
		this,&ANPC_Controller::NewDestination,
		Delay,
		false);	
}

void ANPC_Controller::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	SetTimerForNextMove();
}
