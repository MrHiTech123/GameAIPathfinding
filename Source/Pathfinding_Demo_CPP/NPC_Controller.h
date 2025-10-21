#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "NPC_Controller.generated.h"

UENUM(Blueprintable)
enum class ENPC_State : uint8
{
	WANDER UMETA(DisplayName = "Wander"),
	ORDERED_WAYPOINT UMETA(DisplayName = "OrderedWaypoint"),
	RANDOM_WAYPOINT UMETA(DisplayName = "RandomWaypoint")
};

UCLASS()
class PATHFINDING_DEMO_CPP_API ANPC_Controller : public AAIController
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	ENPC_State NPC_State;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	TArray<AActor*> Waypoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float WaypointRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float WanderMaxDistance;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float WaitTimeMin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float WaitTimeMax;
protected:
	FTimerHandle MoveDelayTimer;
	UNavigationSystemV1* NavigationSystem;
	int NextWaypointIndex;
	
public:
	virtual void BeginPlay() override;

	// This function is called when the AI movement is complete
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
	void SetTimerForNextMove();
	UFUNCTION()
	void NewDestination();

	bool Wander(FNavLocation& Destination);
	bool OrderedWaypoint(FNavLocation& Destination);
	bool RandomWaypoint(FNavLocation& Destination);
};
