// Fill out your copyright notice in the Description page of Project Settings.


#include "NeuralNetworkWidget.h"
#include "NeuralNetwork/Manager/NeuralNetworkManager.h"
#include "Libraries/GeneticFL.h"
#include "GeneticAIGameModeBase.h"

void UNeuralNetworkWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Manager = UGeneticFL::GetGeneticGameMode(this)->Manager;
	if(Manager)
	{
		Manager->OnNewSpecimen.AddDynamic(this, &UNeuralNetworkWidget::UpdateSpecimen);
		Manager->OnNewGeneration.AddDynamic(this, &UNeuralNetworkWidget::UpdateGeneration);
		Manager->OnFitnessUpdated.AddDynamic(this, &UNeuralNetworkWidget::UpdateFitness);
		Manager->OnOutputFunctionsCreated.AddDynamic(this, &UNeuralNetworkWidget::CreateOutputFunctions);
		Manager->OnOutputFunctionsUpdated.AddDynamic(this, &UNeuralNetworkWidget::UpdateOutputFunctions);
		Manager->OnRunEnded.AddDynamic(this, &UNeuralNetworkWidget::EndRun);
	}
}

void UNeuralNetworkWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UNeuralNetworkWidget::UpdateGeneration_Implementation(uint8 NumberOfGeneration)
{
}

void UNeuralNetworkWidget::UpdateSpecimen_Implementation(uint8 NumberOfSpecimen)
{
}


void UNeuralNetworkWidget::InitWidget_Implementation()
{
}

void UNeuralNetworkWidget::UpdateFitness_Implementation(float Fitness)
{
}

void UNeuralNetworkWidget::CreateOutputFunctions_Implementation(const TArray<FName>& Names, const TArray<uint8>& Indexes)
{
}

void UNeuralNetworkWidget::UpdateOutputFunctions_Implementation(const TArray<float>& Values)
{
}

void UNeuralNetworkWidget::EndRun_Implementation(float TimeToNextSpecimen, float Fitness, uint8 SpecimenNumber)
{
}
