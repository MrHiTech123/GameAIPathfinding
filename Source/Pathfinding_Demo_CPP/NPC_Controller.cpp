#include "NPC_Controller.h"

#include "DistanceSortFunctor.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"

#define NEAR_EACH_OTHER_DISTANCE 1000.0

ANPC_Controller::ANPC_Controller()
{
	NPC_State = ENPC_State::WANDER;
	WanderMaxDistance = 1000;
	WaypointRadius = 100; 
	NextWaypointIndex = 0;
	WaitTimeMin = 0.5;
	WaitTimeMax = 1;

	
}

float ANPC_Controller::DistanceFrom(AActor* other) const
{
	return (other->GetActorLocation() - GetPawn()->GetActorLocation()).Size();
}

DistanceSortFunctor ANPC_Controller::DistanceSortPredicate()
{
	return DistanceSortFunctor(GetPawn()->GetActorLocation());
}


void ANPC_Controller::BeginPlay()
{
	Super::BeginPlay();

	
	// find all waypoints
	UGameplayStatics::GetAllActorsWithTag(GetWorld(),FName("Waypoint"),Waypoints);

	
	Waypoints.Sort(DistanceSortPredicate());
	
	UE_LOG(LogTemp, Warning, TEXT("Waypoint amount: %d"), Waypoints.Num());
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
		case ENPC_State::LONELY_WAYPOINT:
			bSuccess = LonelyWaypoint(NavLocation);
			break;
	}

	if (bSuccess)
	{
		// initiate movement to new destination
		//MoveToLocation(NavLocation.Location);
		MoveToLocation(NavLocation.Location,-1,true,true,false,true,
			NavFilter,true);
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
	if (Waypoints.Num())
	{
		NextWaypointIndex = (NextWaypointIndex+1) % Waypoints.Num();
		AActor* Waypoint = Waypoints[NextWaypointIndex];
		Destination.Location = Waypoint->GetActorLocation();
		return true;
	}
	else
	{
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

bool ANPC_Controller::NearEachOther(AActor* a, AActor* b)
{
	return FVector::Dist(a->GetActorLocation(), b->GetActorLocation()) < NEAR_EACH_OTHER_DISTANCE;
}

int ANPC_Controller::PeopleNearAmount(AActor* waypoint)
{
	TArray<AActor*> AllMovementComponents;
	UGameplayStatics::GetAllActorsWithTag(GetPawn()->GetWorld(), TEXT("person"), AllMovementComponents);
	int toReturn = 0;
	for (AActor* person : AllMovementComponents)
	{
		if (NearEachOther(person, waypoint))
		{
			++toReturn;
		}
	}
	return toReturn;
}

bool ANPC_Controller::LonelyWaypoint(FNavLocation& Destination)
{
	if (Waypoints.Num() < 0) return false;
	
	AActor* leastPopular = Waypoints[0];
	for (AActor* waypoint : Waypoints)
	{
		int nearWaypoint = PeopleNearAmount(waypoint);
		int nearLeastPopular = PeopleNearAmount(leastPopular);
		int distWaypoint = FVector::Dist(waypoint->GetActorLocation(), GetPawn()->GetActorLocation());
		int distLeastPopular = FVector::Dist(leastPopular->GetActorLocation(), GetPawn()->GetActorLocation());
		if ((nearWaypoint < nearLeastPopular) ||
			((nearWaypoint == nearLeastPopular) && (distWaypoint < distLeastPopular)))
		{
			leastPopular = waypoint;
		}
	}
	Destination.Location = leastPopular->GetActorLocation();

	return true;
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
