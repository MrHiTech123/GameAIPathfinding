#pragma once

class DistanceSortFunctor
{
private:
	FVector location;
public:
	DistanceSortFunctor(FVector location) {this->location = location;};
	bool operator()(AActor* a, AActor* b);
};


