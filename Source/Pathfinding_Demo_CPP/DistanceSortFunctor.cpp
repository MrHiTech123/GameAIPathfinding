#include "DistanceSortFunctor.h"


float distance(FVector a, FVector b)
{
	return (a - b).Length();
}

bool DistanceSortFunctor::operator()(AActor* a, AActor* b)
{
	return distance(a->GetActorLocation(), location) < distance(b->GetActorLocation(), location);
}
