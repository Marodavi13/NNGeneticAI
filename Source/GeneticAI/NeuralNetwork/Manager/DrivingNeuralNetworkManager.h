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
	// Check if it should end the current run
	virtual bool CheckEndConditions() const override;
	// Move the pawn to spawn position -- Currently in BP because bugs
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
		float DistanceConstant;
	// Constant to multiply the Lap factor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fitness | Distance")
		float LapConstant;
	// Can Increase the lap is passes through the finish lane
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Population | Specimen")
		bool bIsIncreaseLapAllowed;
	// Sum of all the speeds
	UPROPERTY(BlueprintReadOnly,Transient, Category = "Fitness | Speed")
		float SpeedSum;
	// Times that we have sum the Speed
	UPROPERTY(BlueprintReadOnly, Transient, Category = "Fitness | Speed")
		int32 SpeedCount;
	// Constant to multiply the Speed factor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fitness | Speed")
		float SpeedConstant;
	// Constant to multiply the Time factor
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Fitness | Time")
		float TimeConstant;
	// Number of laps to end the simulation
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Population | Specimen")
		uint8 MaxNumberOfLaps;
	//Current Number of laps
	UPROPERTY(BlueprintReadOnly, Transient,Category = "Population | Specimen")
		uint8 CurrentNumberOfLaps;
	// Movement component of the pawn
	UPROPERTY(BlueprintReadOnly, Category = "Population | Pawn")
		UFloatingPawnMovement* SpecimenPawnMovement;
	// Trigger that validates the increase oh the current Lap
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UBoxComponent* LapValidator;

private:
	UPROPERTY()
		FRotator BaseRotation;
};
