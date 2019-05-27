// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GeneticAI.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GeneticFL.generated.h"

/**
 * 
 */
UCLASS()
class GENETICAI_API UGeneticFL : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static class AGeneticAIGameModeBase* GetGeneticGameMode(const UObject* WorldContext);
};
