// Fill out your copyright notice in the Description page of Project Settings.


#include "GeneticAIGameModeBase.h"



AGeneticAIGameModeBase::AGeneticAIGameModeBase(){}

void AGeneticAIGameModeBase::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);
}
