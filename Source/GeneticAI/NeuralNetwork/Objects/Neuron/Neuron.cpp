// Fill out your copyright notice in the Description page of Project Settings.


#include "Neuron.h"
#include "../Synapse/Synapse.h"
void UNeuron::CalculateValue(EFunction FunctionType)
{
	//If it has synapses (It isn't in the inputs Layer)
	if (Synapses.Num())
	{
		Value = Bias;
		//For each synapse add the weight asociated
		for(const USynapse* Synapse: Synapses)
		{
			Value += Synapse->GetWeightedValue();
		}

		//calculate the value
		switch (FunctionType)
		{
		case EFunction::SIGMOID:
			Sigmoid(Value);
			break;
		case EFunction::RELU:
			ReLU(Value);
			break;
		default:
			break;
		}
	}
}