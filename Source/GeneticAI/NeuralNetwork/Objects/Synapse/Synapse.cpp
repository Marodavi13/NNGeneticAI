// Fill out your copyright notice in the Description page of Project Settings.


#include "Synapse.h"
#include "../Neuron/Neuron.h"

void USynapse::InitSynapse(UNeuron& Source, UNeuron& Target)
{
	NeuronFrom = &Source;
	NeuronTo = &Target;
	Weight = FMath::RandRange(-5.f, 5.f);
	NeuronTo->Synapses.Add(this);
}
 
float USynapse::GetWeightedValue() const
{
	float WeightedValue = 0.f;
	if(IsValid(NeuronFrom))
	{
		WeightedValue = NeuronFrom->Value * Weight;
	}
	else
	{
		PRINT_ST(GetFName().ToString() + " has no NeuronFrom", 5.f, WARNING);
	}
	return WeightedValue;
}
