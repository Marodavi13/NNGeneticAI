// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GeneticAI.h"
#include "Components/ActorComponent.h"
#include "NeuralInputComponent.generated.h"

class ANeuralNetworkManager;
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GENETICAI_API UNeuralInputComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UNeuralInputComponent();

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	// Sets the inputs which will be passed to the NN-manager
	UFUNCTION(BlueprintCallable)
	void SetInputs(const TArray<float> NewInputs);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
public:	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inputs")
		uint8 NumberOfInputs = 0;
	// Inputs sent to the Neural Network
	UPROPERTY(BlueprintReadOnly, Category = "Inputs")
		TArray<float> Inputs;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Inputs")
		ANeuralNetworkManager* Manager;
		
};
