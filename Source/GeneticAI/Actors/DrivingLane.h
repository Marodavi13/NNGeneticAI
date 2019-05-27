// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GeneticAI.h"
#include "GameFramework/Actor.h"
#include "DrivingLane.generated.h"

class USplineComponent;
UCLASS()
class GENETICAI_API ADrivingLane : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADrivingLane();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spline")
		USplineComponent* Spline;

};
