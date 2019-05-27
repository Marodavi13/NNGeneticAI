// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GeneticAI.h"
#include "UObject/NoExportTypes.h"
#include "Neuron.generated.h"

//Forward declaration
class USynapse;

UCLASS()
class GENETICAI_API UNeuron : public UObject
{
	GENERATED_BODY()
public:
	void CalculateValue(EFunction FunctionType);
	
public:
	//Neuron value
	UPROPERTY(BlueprintReadOnly, Category = "Properties")
		float Value = 0.f;
	//Synapses from the previous layer
	UPROPERTY(BlueprintReadOnly, Category = "Properties")
		TArray<USynapse*> Synapses;
	//Neuron bias
	UPROPERTY(BlueprintReadOnly, Category = "Properties")
		float Bias = 0.f;
};
