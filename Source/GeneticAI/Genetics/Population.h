// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GeneticAI.h"
#include "UObject/NoExportTypes.h"
#include "Population.generated.h"

class USpecimen;
class ANeuralNetworkManager;
/**
 *  
 */
UCLASS()
class GENETICAI_API UPopulation : public UObject
{
	GENERATED_BODY()
public:
	//Creates an initializes the population of the specimens
	void Populate(uint8 NumberOfSpecies, ANeuralNetworkManager& NeuralnetworkManager);
	// Function that selects, crosses and mutates new specimens
	void EvolveSpecimens(uint8 NumberOfSpecimensToKeep, uint8 NumberOfSpecimensToCross, uint8 SynapseMutationChance, uint8 BiasMutationChance, float MutationStep, uint8 CurrentGeneration);
private:
	// Mutates specimens
	void Mutation(TArray<USpecimen*>& NewSpecimens, uint8 SynapseMutationChance, uint8 BiasMutationChance, float MutationStep);
	// Selects a number of specimens, randomized if indicated
	void Selection(TArray<USpecimen*>& NewSpecimens, uint8 NumberOfSpecimensToKeep);
	// Crosses specimens
	void Crossover(TArray<USpecimen*>& NewSpecimens, uint8 NumberOfParents, uint8 NumberOfSpecimensToCross, uint8 CurrentGeneration);
public:
	// Specimens of this Generation
	UPROPERTY(BlueprintReadOnly, Category = "Specimens")
		TArray<USpecimen*> Specimens;
};
