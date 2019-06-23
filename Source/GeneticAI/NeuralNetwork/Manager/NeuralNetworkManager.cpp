// Fill out your copyright notice in the Description page of Project Settings.


#include "NeuralNetworkManager.h"
#include "Genetics/Population.h"
#include "Actors/WreckingBall.h"
#include "Actors/DrivingLane.h"
#include "Genetics/Specimen.h"
#include "Actors/_Components/NeuralInputComponent.h"
#include "NeuralNetwork/NeuralNetwork.h"
#include "../Objects/Neuron/Neuron.h"
#include "Libraries/GeneticFL.h"
#include "GeneticAIGameModeBase.h"
#include WORLD	
#include PLAYERCONTROLLER
#include STATICMESH_COMPONENT
#include FLOATINGPAWNMOVEMENT_COMPONENT
#include TIMERMANAGER
#include SCENE_COMPONENT
// Sets default values
ANeuralNetworkManager::ANeuralNetworkManager()
{

	Root = CreateDefaultSubobject<USceneComponent>("Scene");
	SetRootComponent(Root);
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Functions to be called
	OutputFunctions.Add(FOutputFunction("MoveForward", -1.f, 1.f));
	OutputFunctions.Add(FOutputFunction("TurnRight", -1.f, 1.f));
	TimeBetweenRuns = 1.f;
	CurrentGeneration = 1;
	bIsDebugActivated = false;
}

// Called when the game starts or when spawned
void ANeuralNetworkManager::BeginPlay()
{
	Super::BeginPlay();
	UGeneticFL::GetGeneticGameMode(this)->Manager = this;
	uint8 NumberOfOutputFunctions = OutputFunctions.Num();
	if(NeuronLayers.Last() != NumberOfOutputFunctions)
	{
		PRINT_ST("Output Neurons and Functions don't match!", 10.f, ERROR);
	}
	else
	{
		
		if (SetSpawningTransform())
		{
			
			if(TimeBetweenRuns <= 0.f)
			{
				TimeBetweenRuns = 0.5f;
			}
			SpawnSpecimenPawn(SpawningTransform);
			Population = NewObject<UPopulation>(this);
			Population->Populate(NumberOfSpecimens, *this);
			StartSimulation();
		}
		else
		{
			PRINT_ST("Can't spawn specimen pawn!", 10.f, ERROR);

		}
	}
}

void ANeuralNetworkManager::SpawnSpecimenPawn(const FTransform& SpawnTransform)
{
	UWorld* World = GetWorld();
	if (World)
	{ 
		if (SpecimenPawnClass)
		{
			// Spawn Pawn
			FActorSpawnParameters Spawn;
			Spawn.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpecimenPawn = World->SpawnActor<APawn>(SpecimenPawnClass, SpawnTransform, Spawn);
			// Spawn desired controller
			if(bIsPossesedByFirstPlayerController)
			{
				SpecimenPawn->PossessedBy(GetWorld()->GetFirstPlayerController());
			}
			else
			{
				SpecimenPawn->SpawnDefaultController();
			}
			// Set components
			StorePawnComponents();
			//We want the specimen to set the inputs before we get them;
			AddTickPrerequisiteActor(SpecimenPawn);
		}
		else
		{
			PRINT_ST("Pawn class not set", 5.f, ERROR);
		}
	
	}
}

void ANeuralNetworkManager::StartSimulation()
{
	OnSimulationStarted.Broadcast();
	// Creates an array of the names of the output functions and another one with the functions index that has parameters 
	TArray<FName> Names;
	TArray<uint8> FunctionIndex;
	uint8 Size = OutputFunctions.Num();
	for (uint8 i = 0; i < Size; ++i)
	{
		Names.Add(OutputFunctions[i].FunctionName);
		// If the function is not threshold function, it will pass a value
		if (!OutputFunctions[i].bIsThresHoldFunction)
		{
			FunctionIndex.Add(i);
		}
	}
	OnOutputFunctionsCreated.Broadcast(Names, FunctionIndex);
	OnNewGeneration.Broadcast(CurrentGeneration);
	//Resets all the fitness properties, just in case
	ResetFitnessProperties();
	// Prepares the Specimen to simulate
	CurrentSpecimenIndex = 1;
	CurrentSpecimen = Population->Specimens[CurrentSpecimenIndex - 1];
	OnNewSpecimen.Broadcast(CurrentSpecimenIndex);
}

void ANeuralNetworkManager::StartNextSpecimenSimulation()
{
	//Check next generation
	if (CurrentSpecimenIndex >= NumberOfSpecimens)
	{
		AverageFitnessByGeneration.Insert(TotalFitness / NumberOfSpecimens, CurrentGeneration - 1);
		++CurrentGeneration;
		Population->EvolveSpecimens(NumberOfSpecimensToKeep, NumberOfCrosses, SynapseMutationChance, BiasMutationChance, MutationStep, CurrentGeneration);
		CurrentSpecimenIndex = 0;
		OnNewGeneration.Broadcast(CurrentGeneration);
	}
	// Reset used properties and set next specimen
	++CurrentSpecimenIndex;
	CurrentSpecimen = Population->Specimens[CurrentSpecimenIndex-1];
	OnNewSpecimen.Broadcast(CurrentSpecimenIndex);
}

bool ANeuralNetworkManager::SetSpawningTransform() 
{
	return false;
}

void ANeuralNetworkManager::ResetFitnessProperties()
{
	TimeWithoutImprovement = 0.f;
	CurrentRunTime = 0.f;
}

void ANeuralNetworkManager::ResetPawnProperties()
{
	PRINT_ST("Function not overriden", 5.f, WARNING);
}


void ANeuralNetworkManager::EndSpecimenSimulation()
{ 
	if (IsValid(CurrentSpecimen))
	{
		// Update all the Generation data
		TotalFitness += CurrentSpecimen->Fitness;
		if (!MaxFitnessByGeneration.IsValidIndex(CurrentGeneration - 1))
		{
			MaxFitnessByGeneration.Add(CurrentSpecimen->Fitness);
		}
		else if (CurrentSpecimen->Fitness >= MaxFitnessByGeneration[CurrentGeneration - 1])
		{
			MaxFitnessByGeneration[CurrentGeneration - 1] = CurrentSpecimen->Fitness;
		}


		// Sort the Specimens by Fitness to optimize the Selection function
		for (uint8 i = 0; i < CurrentSpecimen->NumberOfSpecimen; ++i)
		{
			if (CurrentSpecimen->Fitness > Population->Specimens[i]->Fitness)
			{
				Population->Specimens.Remove(CurrentSpecimen);
				Population->Specimens.Insert(CurrentSpecimen, i);
				break;
			}
		}
		
		OnRunEnded.Broadcast(TimeBetweenRuns, CurrentSpecimen->Fitness, CurrentSpecimen->NumberOfSpecimen);
		CurrentSpecimen->EndRun(UnderperformedGenerationsUntilMutation);
		ResetPawnProperties();
		ResetFitnessProperties();
		CurrentSpecimen = nullptr;
		FTimerHandle Handle;
		FTimerDelegate Del;
		Del.BindUFunction(this, "StartNextSpecimenSimulation");
		GetWorld()->GetTimerManager().SetTimer(Handle, Del, TimeBetweenRuns, false);
	}
}


float ANeuralNetworkManager::CalculateFitness_Implementation(float StepTime)
{
	PRINT_ST("Not defined", StepTime, ERROR);
	return -9999.f;
}

void ANeuralNetworkManager::StorePawnComponents()
{
	SpecimenNeuralInput = SpecimenPawn->FindComponentByClass<UNeuralInputComponent>();
	SpecimenNeuralInput->Manager = this;
}

bool ANeuralNetworkManager::CheckEndConditions() const
{
	bool bShouldEnd = false;
	if (CurrentSpecimen->Fitness < MinFitness || TimeWithoutImprovement >= NoImprovementTimeout)
	{
		bShouldEnd = true;
	}
	return bShouldEnd;
}

// Called every frame
void ANeuralNetworkManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsValid(CurrentSpecimen))
	{
		CurrentRunTime += DeltaTime;
		// Get the inputs from our specimen
		for (uint8 i = 0; i < SpecimenNeuralInput->NumberOfInputs; ++i)
		{
			CurrentSpecimen->NeuralNetwork->Neurons[i]->Value = SpecimenNeuralInput->Inputs[i];
		}
		// Run the Neural Network
		CurrentSpecimen->NeuralNetwork->ActivateNeurons();
		// Get the output values and call the output functions
		uint8 LastLayerIndex = CurrentSpecimen->NeuralNetwork->NumberOfNeurons - NeuronLayers.Last();
		uint8 LastIndex = NeuronLayers.Last();
		FOutputFunction* FunctionData = nullptr;
		FunctionValues.Empty();
		for (uint8 i = 0; i < LastIndex; ++i)
		{
			FunctionData = &OutputFunctions[i];
			UFunction* Function = SpecimenPawn->FindFunction(FunctionData->FunctionName);
			if(Function)
			{
				if (IsValid(CurrentSpecimen))
				{
					float Value = CurrentSpecimen->NeuralNetwork->Neurons[i + LastLayerIndex]->Value;
					if (FunctionData->bIsThresHoldFunction)
					{
						if (CurrentSpecimen->NeuralNetwork->Neurons[i + LastLayerIndex]->Value >= FunctionData->ActivableThreshHold)
						{
							SpecimenPawn->ProcessEvent(Function, nullptr);
						}
					}
					else
					{
						float Parameter = FunctionData->ClampMin * (1 - Value) + FunctionData->ClampMax * Value;
						FunctionValues.Add(Parameter);
						SpecimenPawn->ProcessEvent(Function, &Parameter);
					}
				}
			}
			else
			{
				PRINT_ST("Function not valid", 5.f, ERROR);
			}
		}
		if (IsValid(CurrentSpecimen))
		{
			OnOutputFunctionsUpdated.Broadcast(FunctionValues);
			//Update fitness
			CurrentSpecimen->PreviousFitness = CurrentSpecimen->Fitness;
			CurrentSpecimen->Fitness = CalculateFitness(DeltaTime);
			if (CurrentSpecimen->Fitness > 0.f && CurrentSpecimen->Fitness > CurrentSpecimen->PreviousFitness)
			{
				TimeWithoutImprovement = 0.f;
			}
			else
			{
				TimeWithoutImprovement += DeltaTime;
			}
			OnFitnessUpdated.Broadcast(CurrentSpecimen->Fitness);
			//Check fitness integrity
			if (CheckEndConditions())
			{
				EndSpecimenSimulation();
			}
		}
	}
}

