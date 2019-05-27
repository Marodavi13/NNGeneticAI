// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GeneticAI.h"
#include "GameFramework/Actor.h"
#include "NeuralNetworkManager.generated.h"

//Forward declaration
class UNeuron;
class UPopulation;
class USpecimen;
class ATargetPoint;
class ADrivingLane;
class UNeuralInputComponent;
class USceneComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSimulationStarted);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewSpecimen, uint8, NewSpecimenIndex);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNewGeneration, uint8, NewGeneration);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnFitnessUpdated, float, Fitness);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOutputFunctionsCreated, const TArray<FName>&, Names, const TArray<uint8>&, Indexes);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOutputFunctionssUpdated, const TArray<float>&, Values);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnRunEnded, float, TimeUntilNextSimulation, float, Fitness, uint8, NumberOfSpecimen);


//Struct that contains the data of the output functions
USTRUCT(BlueprintType)
struct FOutputFunction
{
	GENERATED_USTRUCT_BODY()
	FOutputFunction() {}
	FOutputFunction(const FName& NewFunctionName, float NewClampMin, float NewClampMax) 
	{
		FunctionName = NewFunctionName;
		bIsThresHoldFunction = false;
		ClampMin = NewClampMin;
		ClampMax = NewClampMax;
	};

	FOutputFunction(const FName& NewFunctionName, float NewThreshold)
	{
		FunctionName = NewFunctionName;
		bIsThresHoldFunction = true;
		ActivableThreshHold = NewThreshold;
	};
	// The name of the function to be called
	UPROPERTY(EditAnywhere, Category = "Output Function")
		FName FunctionName;
	// There are functions that always are called with a value, and functions called when a threshold value is reached
	UPROPERTY(EditAnywhere, Category = "Output Function")
		bool bIsThresHoldFunction;
	// Min Value passed to the function
	UPROPERTY(EditAnywhere, Category = "Output Function", meta = (EditCondition = "!bIsThresHoldFunction"))
		float ClampMin;
	// Max value passed to the function
	UPROPERTY(EditAnywhere, Category = "Output Function", meta = (EditCondition = "!bIsThresHoldFunction"))
		float ClampMax;
	// Threshold to call the function
	UPROPERTY(EditAnywhere, Category = "Output Function", meta = (EditCondition = "bIsThresHoldFunction"))
		float ActivableThreshHold;
};


UCLASS()
class GENETICAI_API ANeuralNetworkManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANeuralNetworkManager();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// End the current specimen run
	UFUNCTION(BlueprintCallable)
		virtual void EndSpecimenSimulation();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent)
		void LoadGame();
	//First call to start the simulation
	void StartSimulation();
	// Starts the simulation of the next Specimen
	UFUNCTION()
	void StartNextSpecimenSimulation();
	// Sets the transform where to spawn
	virtual bool SetSpawningTransform();

	UFUNCTION(BlueprintNativeEvent)
	float CalculateFitness(float StepTime);
	virtual float CalculateFitness_Implementation(float StepTime);

	//Stores the needed components
	virtual void StorePawnComponents();
	//Resets the properties used to calculate the fitness 
	virtual void ResetFitnessProperties();
	// Resets porperties of the Specimen pawn
	virtual void ResetPawnProperties();
	//Checks the conditions to end the current run, returns true if it should end
	virtual bool CheckEndConditions() const;
private:
	// Sets the speciment location to the target point location, if the speciment doesn't exist, it spawns a new one -- INTERNAL
	//void InitSpecimenActor();
	void SpawnSpecimenPawn(const FTransform& SpawnTransform);


public:
	//--------Delegates------
	UPROPERTY(BlueprintAssignable)
		FOnSimulationStarted OnSimulationStarted;
	UPROPERTY(BlueprintAssignable)
		FOnRunEnded OnRunEnded;
	UPROPERTY(BlueprintAssignable)
		FOnNewSpecimen OnNewSpecimen;
	UPROPERTY(BlueprintAssignable)
		FOnNewGeneration OnNewGeneration;
	UPROPERTY(BlueprintAssignable)
		FOnFitnessUpdated OnFitnessUpdated;
	UPROPERTY(BlueprintAssignable)
		FOnOutputFunctionsCreated OnOutputFunctionsCreated;
	UPROPERTY(BlueprintAssignable)
		FOnOutputFunctionssUpdated OnOutputFunctionsUpdated;
	//-------------------------


	// All the layers
	UPROPERTY(EditAnywhere, BlueprintReadOnly,	Category = "Neural Network")
		TArray<uint8> NeuronLayers;
	// Function  to be used in the neural network
	UPROPERTY(EditAnywhere, BlueprintReadOnly,	Category = "Neural Network")
		EFunction NeuralNetworkFunction;
	// Generation running
	UPROPERTY(BlueprintReadOnly,					Category = "Population | Specimen")
		uint8 CurrentGeneration = 1;


	// Spawned Pawn
	UPROPERTY(BlueprintReadOnly,					Category = "Population | Pawn")
		APawn* SpecimenPawn;
	// Neural input of the Spawned pawn
	UPROPERTY(BlueprintReadOnly,					Category = "Population | Pawn")
		UNeuralInputComponent* SpecimenNeuralInput;

	// All the Average fitness until this generation
	UPROPERTY(BlueprintReadOnly,					Category = "Population | Stats")
		TArray<float> AverageFitnessByGeneration;
	// Max fitness until this generation
	UPROPERTY(BlueprintReadOnly,					Category = "Population | Stats")
		TArray<float> MaxFitnessByGeneration;
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly,		Category = "Population | Specimen")
		uint8 UnderperformedGenerationsUntilMutation;
	// Current Population
	UPROPERTY(BlueprintReadOnly,					Category = "Population")
		UPopulation* Population;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,		Category = "Population | Evolution")
		uint8 NumberOfSpecimensToKeep;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,		Category = "Population | Evolution")
		uint8 NumberOfCrosses;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,		Category = "Population | Evolution", meta = (ClampMin = "0", ClampMax = "100"))
		uint8 SynapseMutationChance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,		Category = "Population | Evolution", meta = (ClampMin = "0", ClampMax = "100"))
		uint8 BiasMutationChance;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,		Category = "Population | Evolution", meta = (ClampMin = "0", ClampMax = "1"))
		float MutationStep;

	// Class of the Pawn to spawn
	UPROPERTY(EditAnywhere, BlueprintReadOnly,	Category = "Population | Pawn")
		TSubclassOf<APawn> SpecimenPawnClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly,		Category = "Population | Pawn")
		TArray<FOutputFunction> OutputFunctions;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Population | Pawn")
		bool bIsPossesedByFirstPlayerController;
	//Number of specimens in this population
	UPROPERTY(EditAnywhere, BlueprintReadOnly,		Category = "Population | Specimen")
		uint8 NumberOfSpecimens;
	// Current specimen
	UPROPERTY(BlueprintReadOnly,					Category = "Population | Specimen")
		USpecimen* CurrentSpecimen;
	// Index of the CurrentSpecimen
	UPROPERTY(BlueprintReadOnly,					Category = "Population | Specimen")
		uint8 CurrentSpecimenIndex = 0;
	//Time elapsed between the end of the run and the next one
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Population | Simulation")
		float TimeBetweenRuns = 1.f;
	// If the specimen gets lower fitness than this one it end his run
	UPROPERTY(EditAnywhere, BlueprintReadOnly,		Category = "Fitness")
		float MinFitness;
	// Time that has passed without improving
	UPROPERTY(BlueprintReadOnly,					Category = "Fitness")
		float TimeWithoutImprovement;
	// Time that has to pass without improvement to end its run
	UPROPERTY(EditAnywhere, BlueprintReadOnly,		Category = "Fitness")
		float NoImprovementTimeout;
	UPROPERTY(BlueprintReadOnly, Category = "Fitness")
		float CurrentRunTime = 0.f;
	UPROPERTY(BlueprintReadOnly, Category = "Population | Pawn")
		FTransform SpawningTransform;
#if WITH_EDITOR
	UPROPERTY(EditAnywhere, BlueprintReadWrite,		Category = "Debug")
		bool bIsDebugActivated = false;
#endif

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		USceneComponent* Root;
	private:
		float TotalFitness = 0.f;
		TArray<float> FunctionValues;
};
