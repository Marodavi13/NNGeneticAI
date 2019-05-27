// Fill out your copyright notice in the Description page of Project Settings.


#include "DrivingLane.h"
// Sets default values
ADrivingLane::ADrivingLane()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
	SetRootComponent(Spline);
}

// Called when the game starts or when spawned
void ADrivingLane::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADrivingLane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

