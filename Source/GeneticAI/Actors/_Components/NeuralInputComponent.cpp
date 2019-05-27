// Fill out your copyright notice in the Description page of Project Settings.


#include "NeuralInputComponent.h"

// Sets default values for this component's properties
UNeuralInputComponent::UNeuralInputComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UNeuralInputComponent::BeginPlay()
{
	Super::BeginPlay();
	Inputs.SetNumZeroed(NumberOfInputs, true);
	// ...
	
}


// Called every frame
void UNeuralInputComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UNeuralInputComponent::SetInputs(const TArray<float> NewInputs)
{
	for (uint8 i = 0; i < NumberOfInputs; ++i)
	{
		if (NewInputs.IsValidIndex(i))
		{
			Inputs[i] = NewInputs[i];
		}
	}
}



