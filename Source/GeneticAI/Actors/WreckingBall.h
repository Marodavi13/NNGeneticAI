// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GeneticAI.h"
#include "GameFramework/Pawn.h"
#include "WreckingBall.generated.h"

//Forward declaration
class UStaticMeshComponent;
class UCameraComponent;
class UNeuralInputComponent;
class UFloatingPawnMovement;
class USphereComponent;
class ANeuralNetworkManager;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnSpecimenCollision);


UCLASS()
class GENETICAI_API AWreckingBall : public APawn
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWreckingBall();
	// Called for turning
	UFUNCTION(BlueprintCallable)
	void TurnRight(float Value);
	// Called for moving in X axis
	UFUNCTION(BlueprintCallable)
	void MoveForward(float Value);
	// Callback Overlap
	UFUNCTION()
		void OnBeginOverlap(class UPrimitiveComponent* HitComp = nullptr, class AActor* OtherActor = nullptr, class UPrimitiveComponent* OtherComp = nullptr, int32 OtherBodyIndex = 0, bool bFromSweep = false, const FHitResult& SweepResult = FHitResult());

	// Called every frame
	virtual void Tick(float DeltaTime) override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	//Speed turning
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Turn")
		float TurnSpeed;
	// Overlap Sphere
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		USphereComponent* SphereCollision;
	//Mesh 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UStaticMeshComponent* Mesh;
	// Camera
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
		UCameraComponent* Camera;
	//Movement Comp
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UFloatingPawnMovement* MovementComponent;
	// COmponent for the NN
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
		UNeuralInputComponent* NeuralInput;
	// Delegates
	UPROPERTY(BlueprintAssignable)
		FOnSpecimenCollision OnSpecimenCollision;
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Neural Network")
	ANeuralNetworkManager* Manager;

};
