// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GeneticAI.h"
#include "GameFramework/GameModeBase.h"
#include "GeneticAIGameModeBase.generated.h"

class ANeuralNetworkManager;

UCLASS()
class GENETICAI_API AGeneticAIGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AGeneticAIGameModeBase();
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NeuralNetwork")
		ANeuralNetworkManager* Manager;
};
 