// Fill out your copyright notice in the Description page of Project Settings.


#include "Specimen.h"
#include "../NeuralNetwork/NeuralNetwork.h"
#include "../NeuralNetwork/Manager/NeuralNetworkManager.h"

void USpecimen::InitSpecimen(uint8 Number, ANeuralNetworkManager& NeuralNetworkManager)
{
	
	GenerationBorn = NeuralNetworkManager.CurrentGeneration;
	NumberOfSpecimen = Number;
	NeuralNetwork = NewObject<UNeuralNetwork>(this);
	NeuralNetwork->SetupNeuralNetwork(NeuralNetworkManager);
}

void USpecimen::ResetSpecimen(uint8 Generation, uint8 NewNumberOfSpecimen)
{
	Fitness = 0.f;
	PreviousFitness = 0.f;
	GenerationBorn = Generation;
	NumberOfSpecimen = NewNumberOfSpecimen;
	UnderperformedGenerations = 0;
}


void USpecimen::EndRun(uint8 UnderperformedGenerationsUntilMutation)
{
	//If I have improved set UnderperformedGenerations to 0
	if (Fitness >= PreviousFitness)
	{
		UnderperformedGenerations = 0;
	}
	//If not rise it
	else
	{
		++UnderperformedGenerations;
	}
}


