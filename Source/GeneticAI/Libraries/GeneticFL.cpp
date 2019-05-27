// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneticFL.h"
#include "GeneticAIGameModeBase.h"
#include WORLD

AGeneticAIGameModeBase* UGeneticFL::GetGeneticGameMode(const UObject* WorldContext)
{
	UWorld* World = WorldContext->GetWorld();
	if (World)
	{
		return Cast<AGeneticAIGameModeBase>(World->GetAuthGameMode());
	}
	else
	{
		return nullptr;
	}
}