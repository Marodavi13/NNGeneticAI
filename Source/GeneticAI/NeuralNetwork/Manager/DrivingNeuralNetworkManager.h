// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GeneticAI.h"
#include "NeuralNetwork/Manager/NeuralNetworkManager.h"
#include "DrivingNeuralNetworkManager.generated.h"
// forward declaration
class UFloatingPawnMovement;
class UBoxComponent;
UCLASS()
class GENETICAI_API ADrivingNeuralNetworkManager : public ANeuralNetworkManager
{
	GENERATED_BODY()
public:
	// Sets default values for this actor's properties
	ADrivingNeuralNetworkManager();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// End the current specimen run
	virtual void EndSpecimenSimulation() override;
protected:
	UFUNCTION()
		void OnLapValidation(class UPrimitiveComponent* HitComp = nullptr, class AActor* OtherActor = nullptr, class UPrimitiveComponent* OtherComp = nullptr, int32 OtherBodyIndex = 0, bool bFromSweep = false, const FHitResult& SweepResult = FHitResult());

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	// GSts the transform where to spawn
	virtual bool SetSpawningTransform() override;
	//Stores the needed components
	virtual void StorePawnComponents() override;
	//Resets the properties involved to calculate the fitness 
	virtual void ResetFitnessProperties() override;
	// Resets porperties of the Specimen pawn
	virtual void ResetPawnProperties() override;
	// Calculates the CurrentFitness
	virtual float CalculateFitness_Implementation(float StepTime) override;
	virtual bool CheckEndConditions() const override;
	UFUNCTION(BlueprintImplementableEvent)
		void MovePawnToTargetPoint();
protected:
	// Start Point
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Population | Location")
		ATargetPoint* StartingPoint;
	// Total distance traveled by the specimen
	UPROPERTY(BlueprintReadOnly, Category = "Fitness | Distance")
		float DistanceTraveled = 0.f;
	// Driving Line in the center of the track
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Fitness | Distance")
		ADrivingLane* DrivingLane;
	// Constant to multiply the Direction factor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fitness | Distance")
		float DistanceConstant = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fitness | Distance")
		float LapConstant = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Population | Specimen")
		bool bIsIncreaseLapAllowed = false;
	// Sum of all the speeds
	UPROPERTY(BlueprintReadOnly, Category = "Fitness | Speed")
		float SpeedSum = 0.f;
	// Times that we have sum the Speed
	UPROPERTY(BlueprintReadOnly, Category = "Fitness | Speed")
		int32 SpeedCount = 0;
	// Constant to multiply the Speed factor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fitness | Speed")
		float SpeedConstant = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fitness | Time")
		float TimeConstant = 1.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Population | Specimen")
		uint8 MaxNumberOfLaps = 2;
	UPROPERTY(BlueprintReadOnly, Category = "Population | Specimen")
		uint8 CurrentNumberOfLaps = 0;
	UPROPERTY(BlueprintReadOnly, Category = "Population | Pawn")
		UFloatingPawnMovement* SpecimenPawnMovement;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UBoxComponent* LapValidator;

private:
	UPROPERTY()
		FRotator BaseRotation;
};
