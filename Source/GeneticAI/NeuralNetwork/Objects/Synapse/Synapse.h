// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GeneticAI.h"
#include "UObject/NoExportTypes.h"
#include "Synapse.generated.h"

class UNeuron;
/**
 * 
 */
UCLASS()
class GENETICAI_API USynapse : public UObject
{
	GENERATED_BODY()
public:
	//Function that initializes the synapse with a random weight
	void InitSynapse(UNeuron& Source, UNeuron& Target);
	// Get the Weighted value to add to the next neuron
	float GetWeightedValue() const;
public:
	UPROPERTY(BlueprintReadOnly, Category = "Neurons")
		UNeuron* NeuronFrom;
	UPROPERTY(BlueprintReadOnly, Category = "Neurons")
		UNeuron* NeuronTo;
	UPROPERTY(BlueprintReadOnly, Category = "Neurons")
		float Weight;
};
