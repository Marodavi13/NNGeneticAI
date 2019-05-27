// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GeneticAI.h"
#include "UObject/NoExportTypes.h"
#include "Specimen.generated.h"

//Forward declaration
class UNeuralNetwork;
class ANeuralNetworkManager;

UCLASS()
class GENETICAI_API USpecimen : public UObject
{
	GENERATED_BODY()
public:
	//Called when the run has ended, checks if specimen should mutate
	void EndRun(uint8 UnderperformedGenerationsUntilMutation);
	void InitSpecimen(uint8 Number, ANeuralNetworkManager& NeuralnetworkManager);
	void ResetSpecimen(uint8 Generation, uint8 NewNumberOfSpecimen);
public:
	UPROPERTY(BlueprintReadOnly, Category = "Neural Network")
		UNeuralNetwork* NeuralNetwork;
	// Fitness of this specimen 
	UPROPERTY(BlueprintReadOnly, Category = "Fitness")
		float Fitness		= 0.f;
	// Fitness of this specimen from last generation
	UPROPERTY(BlueprintReadOnly, Category = "Fitness")
		float PreviousFitness	= 0.f;
	// Generations without improvement
	UPROPERTY(BlueprintReadOnly, Category = "Mutation")
		uint8 UnderperformedGenerations = 0;
	// Belonged generation
	UPROPERTY(BlueprintReadOnly, Category = "Generation")
		uint8 GenerationBorn = 0;
	// Number of the specimen in his generation
	UPROPERTY(BlueprintReadOnly, Category = "Generation")
		uint8 NumberOfSpecimen = 0;

};
