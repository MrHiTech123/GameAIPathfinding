// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Pathfinding_Demo_CPPGameMode.generated.h"

/**
 *  Simple GameMode for a third person game
 */
UCLASS(abstract)
class APathfinding_Demo_CPPGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	
	/** Constructor */
	APathfinding_Demo_CPPGameMode();
};



