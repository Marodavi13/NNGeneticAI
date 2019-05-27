// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GeneticAI.h"
#include "Blueprint/UserWidget.h"
#include "NeuralNetworkWidget.generated.h"

class USafeZone;
class UVerticalBox;
class UHorizontalBox;
class ANeuralNetworkManager;
UCLASS()
class GENETICAI_API UNeuralNetworkWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	
	// Optionally override the Blueprint "Event Construct" event
	virtual void NativeConstruct() override;
	// Optionally override the tick event
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
protected:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void InitWidget();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void UpdateSpecimen(uint8 NumberOfSpecimen);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void UpdateGeneration(uint8 NumberOfGeneration);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void UpdateFitness(float Fitness);
	UFUNCTION(BlueprintNativeEvent)
		void CreateOutputFunctions(const TArray<FName>& Names, const TArray<uint8>& Indexes);
	UFUNCTION(BlueprintNativeEvent)
		void UpdateOutputFunctions(const TArray<float>& Values);
	UFUNCTION(BlueprintNativeEvent)
		void EndRun(float TimeToNextSpecimen, float Fitness, uint8 SpecimenNumber);
public:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Manager")
		ANeuralNetworkManager* Manager;
};
