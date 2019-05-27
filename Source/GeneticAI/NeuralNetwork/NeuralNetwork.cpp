// Fill out your copyright notice in the Description page of Project Settings.


#include "NeuralNetwork.h"
#include "Manager/NeuralNetworkManager.h"
#include "Objects/Neuron/Neuron.h"
#include "Objects/Synapse/Synapse.h"
#include "Actors/WreckingBall.h"
#include "Actors/_Components/NeuralInputComponent.h"



void UNeuralNetwork::SetupNeuralNetwork(ANeuralNetworkManager& NeuralNetworkManager)
{
	// Set manager
	if (!Manager)
	{
		Manager = &NeuralNetworkManager; 
		// Total Layers -1
		uint8 NumberOfLayers = Manager->NeuronLayers.Num();
		uint8 NumberOfNeuronsInThisLayer = Manager->SpecimenNeuralInput->NumberOfInputs;
		uint8 NumberOfNeuronsInPreviousLayer = 0;
		NumberOfNeurons = 0;
		// Initialize first layer(input layer)
		for (int i = 0; i < NumberOfNeuronsInThisLayer; ++i)
		{
			UNeuron* Neuron = NewObject<UNeuron>(this);
			Neurons.Add(Neuron);
			++NumberOfNeurons;
		}
		NumberOfNeuronsInPreviousLayer = NumberOfNeuronsInThisLayer;
		// Initialize every other layer
		for (uint8 i = 0; i < NumberOfLayers; ++i)
		{
			// Create neurons
			NumberOfNeuronsInThisLayer = Manager->NeuronLayers[i];
			for (uint8 j = 0; j < NumberOfNeuronsInThisLayer; ++j)
			{
				UNeuron* Neuron = NewObject<UNeuron>(this);
				//Create all the synapsis based in last layer neurons
				CreateNeuronSynapsis(*Neuron, NumberOfNeurons, NumberOfNeuronsInPreviousLayer, j);
				Neurons.Add(Neuron);
				++NumberOfNeurons;
			}
			NumberOfNeuronsInPreviousLayer = NumberOfNeuronsInThisLayer;
		}
	}
}
 

void UNeuralNetwork::ActivateNeurons()
{
	if (IsValid(Manager))
	{
		if (Manager->NeuronLayers.IsValidIndex(0))
		{
			//uint8 NumberOfNeurons = Neurons.Num();
			//for each intern neuron calculate their value based on previous neurons
			for (uint8 i = 0; i < NumberOfNeurons; ++i)
			{
				Neurons[i]->CalculateValue(Manager->NeuralNetworkFunction);
			}
		}
		else
		{
			PRINT_ST("NeuronLayers empty", 5.f, ERROR);
		}
	}
	else
	{
		PRINT_ST("Manager not valid", 5.f, ERROR);
	}
}

void UNeuralNetwork::Randomize()
{
	for(UNeuron* Neuron: Neurons)
	{
		Neuron->Bias = FMath::FRandRange(-5.f, 5.f);
		for(USynapse* Synapse: Neuron->Synapses)
		{
			Synapse->Weight = FMath::FRandRange(-5.f, 5.f);
		}
	}
}

void UNeuralNetwork::CreateNeuronSynapsis(UNeuron& Neuron, uint8 TotalNumberOfNeurons, uint8 NeuronsInPreviousLayer, uint8 CreatedNeuronsInThisLayer)
{
	//If is not a input neuron
	if (NeuronsInPreviousLayer)
	{
		// Create and initialize the synapsis conected from the last layer
		for (uint8 i = 0; i < NeuronsInPreviousLayer; ++i)
		{
			USynapse* Synapse = NewObject<USynapse>(&Neuron);
			Synapse->InitSynapse(*Neurons[i + TotalNumberOfNeurons - NeuronsInPreviousLayer - CreatedNeuronsInThisLayer], Neuron);
			Neuron.Synapses.AddUnique(Synapse);
		}
	}
}
