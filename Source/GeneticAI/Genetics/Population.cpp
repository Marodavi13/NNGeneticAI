// Fill out your copyright notice in the Description page of Project Settings.


#include "Population.h"
#include "Specimen.h"
#include "../NeuralNetwork/Manager/NeuralNetworkManager.h"
#include "../NeuralNetwork/NeuralNetwork.h"
#include "../NeuralNetwork/Objects/Neuron/Neuron.h"
#include "../NeuralNetwork/Objects/Synapse/Synapse.h"


void UPopulation::Populate(uint8 NumberOfSpecies, ANeuralNetworkManager& NeuralnetworkManager)
{
	
	for (uint8 i = 0; i < NumberOfSpecies; ++i)
	{
		USpecimen* Specimen = NewObject<USpecimen>(this);
		Specimen->InitSpecimen(i + 1, NeuralnetworkManager);
		Specimens.Add(Specimen);
	}
	
}
 
void UPopulation::EvolveSpecimens(uint8 NumberOfSpecimensToKeep, uint8 NumberOfSpecimensToCross, uint8 SynapseMutationChance, uint8 BiasMutationChance,float MutationStep, uint8 CurrentGeneration)
{
	//Array that contains the next generation of specimens
	TArray<USpecimen*>NewSpecimens;
	//Select bthe best specimens
	Selection(NewSpecimens, NumberOfSpecimensToKeep);
	// Make the have children
	Crossover(NewSpecimens, NumberOfSpecimensToKeep, NumberOfSpecimensToCross, CurrentGeneration);
	// Mutate all the specimens
	Mutation(NewSpecimens, SynapseMutationChance, BiasMutationChance, MutationStep);
	uint8 NumberOfSpecimens = Specimens.Num();
	// Randomize the untouched specimens
	for (uint8 i = NumberOfSpecimensToKeep + NumberOfSpecimensToCross; i < NumberOfSpecimens; ++i)
	{
		Specimens[i]->NeuralNetwork->Randomize();
		NewSpecimens.Add(Specimens[i]);
		NewSpecimens[i]->NumberOfSpecimen = i + 1;
		NewSpecimens[i]->GenerationBorn = CurrentGeneration;
	}
	Specimens = NewSpecimens;
	PRINT_ST("Size: " + FString::FromInt(Specimens.Num()), 5.f, INFO);
}


void UPopulation::Selection(TArray<USpecimen*>& NewSpecimens, uint8 NumberOfSpecimensToKeep)
{
	// As they are sorted by fitness, we'll keep the best ones.
	for (uint8 i = 0; i < NumberOfSpecimensToKeep; ++i)
	{
		NewSpecimens.Add(Specimens[i]);
		NewSpecimens[i]->NumberOfSpecimen = i + 1;
	}
}

void UPopulation::Crossover(TArray<USpecimen*>& NewSpecimens, uint8 NumberOfParents, uint8 NumberOfSpecimensToMix, uint8 CurrentGeneration)
{
	uint8 FirstParentIndex, SecondParentIndex;
	// For every new child specimen
	for (uint8 i = 0; i < NumberOfSpecimensToMix; ++i)
	{
		// Reset generation and index
		Specimens[NumberOfParents + i]->ResetSpecimen(CurrentGeneration, NumberOfParents + i + 1);
		// Meet his new random parents
		FirstParentIndex = FMath::RandRange(0, NumberOfParents - 1);
		SecondParentIndex = FMath::RandRange(0, NumberOfParents - 1);
		// If they are the same, choose another one
		if(FirstParentIndex == SecondParentIndex)
		{
			SecondParentIndex = (SecondParentIndex + 1) % NumberOfParents;
		}
		USpecimen* Parent = nullptr;
		// For every neuron in the specimen
		for (int8 k = 0; k < Specimens[NumberOfParents + i]->NeuralNetwork->NumberOfNeurons; ++k)
		{
			// Select which parent will set this neuron
			if (FMath::RandBool())
			{
				Parent = Specimens[FirstParentIndex];
			}
			else
			{
				Parent = Specimens[SecondParentIndex];
			}
			//Set new Bias
			Specimens[NumberOfParents + i]->NeuralNetwork->Neurons[k]->Bias = Parent->NeuralNetwork->Neurons[k]->Bias;
			uint8 NumberOfSynapses = Specimens[NumberOfParents + i]->NeuralNetwork->Neurons[k]->Synapses.Num();
			//Set new Synapses Weights
			for (uint8 j = 0; j < NumberOfSynapses; ++j)
			{
				Specimens[NumberOfParents + i]->NeuralNetwork->Neurons[k]->Synapses[j]->Weight = Parent->NeuralNetwork->Neurons[k]->Synapses[j]->Weight;
			}
		}
		NewSpecimens.Add(Specimens[NumberOfParents + i]);
		NewSpecimens[i]->NumberOfSpecimen = i + 1;
		//This algorithm is the real proof that having children is expensive
	}
}


void UPopulation::Mutation(TArray<USpecimen*>& NewSpecimens, uint8 SynapseMutationChance, uint8 BiasMutationChance, float MutationStep)
{
	// for every new specimen 
	for(USpecimen* Specimen: NewSpecimens)
	{
		//for every neuron
		for(UNeuron* Neuron: Specimen->NeuralNetwork->Neurons)
		{
			//mutate all the synapses
			for(USynapse* Synapse: Neuron->Synapses)
			{
				if (FMath::RandRange(1, 100) < SynapseMutationChance)
				{
					Synapse->Weight += FMath::FRandRange(-1.f, 1.f) * MutationStep;
				}
			}
			// mutate the bias
			if(FMath::RandRange(0,100)< BiasMutationChance)
			{
				Neuron->Bias += FMath::FRandRange(-1.f, 1.f) * MutationStep;
			}
		}
	}
}



