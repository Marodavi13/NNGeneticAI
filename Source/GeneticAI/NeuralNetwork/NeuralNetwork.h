// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GeneticAI.h"
#include "UObject/NoExportTypes.h"
#include "NeuralNetwork.generated.h"

//Forward declaration
class UNeuron;
class ANeuralNetworkManager;

UCLASS()
class GENETICAI_API UNeuralNetwork : public UObject
{
	GENERATED_BODY()

public:
	// Initializes the Neural Network
	void SetupNeuralNetwork(ANeuralNetworkManager& NeuralNetworkManager);
	// Activates the neurons of the Neural Network
	void ActivateNeurons();
	// Randomizes Neurons 
	void Randomize();
private:
	// Creates the synapsis between neurones
	void CreateNeuronSynapsis(UNeuron& Neuron, uint8 TotalNumberOfNeurons, uint8 NeuronsInPreviousLayer, uint8 CreatedNeuronsInThisLayer);
public:
	// Neural Network Active Manager
	UPROPERTY(BlueprintReadWrite, Category = "Manager")
		ANeuralNetworkManager* Manager;
	// All the Neurons
	UPROPERTY(BlueprintReadOnly, Category = "Neural Network Structure")
		TArray<UNeuron*> Neurons;
	// Size of Neurons
	UPROPERTY(BlueprintReadOnly, Category = "Neural Network Structure")
		uint8 NumberOfNeurons;
};
