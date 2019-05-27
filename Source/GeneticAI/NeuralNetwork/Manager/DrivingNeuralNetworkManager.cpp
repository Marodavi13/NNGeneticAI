// Fill out your copyright notice in the Description page of Project Settings.


#include "DrivingNeuralNetworkManager.h"
#include "Actors/DrivingLane.h"
#include "Actors/_Components/NeuralInputComponent.h"
#include TARGETPOINT
#include DRAWDEBUG
#include CONTROLLER
#include FLOATINGPAWNMOVEMENT_COMPONENT
#include BOX_COMPONENT
ADrivingNeuralNetworkManager::ADrivingNeuralNetworkManager()
{
	LapValidator = CreateDefaultSubobject<UBoxComponent>("Lap Validator");
	LapValidator->SetupAttachment(Root);
}

void ADrivingNeuralNetworkManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if(IsValid(SpecimenPawn))
	PRINT_ST("Spawned Location: " + SpecimenPawn->GetActorLocation().ToString(), DeltaTime, SUCCESS);

}

void ADrivingNeuralNetworkManager::EndSpecimenSimulation()
{
	SpecimenPawnMovement->ConsumeInputVector();
	SpecimenPawnMovement->StopMovementImmediately();
	SpecimenPawnMovement->StopActiveMovement();
	SpecimenPawnMovement->StopMovementKeepPathing();
	SpecimenPawnMovement->Velocity = FVector(0.f);
	ANeuralNetworkManager::EndSpecimenSimulation();

}

void ADrivingNeuralNetworkManager::OnLapValidation(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (bIsIncreaseLapAllowed && OtherActor->GetComponentByClass(UNeuralInputComponent::StaticClass()))
	{
		++CurrentNumberOfLaps;
		bIsIncreaseLapAllowed = false;
		if(CurrentNumberOfLaps > MaxNumberOfLaps)
		{
			EndSpecimenSimulation();
		}
	}
}

void ADrivingNeuralNetworkManager::BeginPlay()
{
	Super::BeginPlay();
	LapValidator->OnComponentBeginOverlap.AddDynamic(this, &ADrivingNeuralNetworkManager::OnLapValidation);
}

bool ADrivingNeuralNetworkManager::SetSpawningTransform()
{
	bool bHasBeenSet = false;
	if(StartingPoint)
	{
		SpawningTransform = StartingPoint->GetTransform();
		BaseRotation = StartingPoint->GetActorRotation();
		bHasBeenSet = true;
	}
	return bHasBeenSet;
}

void ADrivingNeuralNetworkManager::StorePawnComponents()
{
	ANeuralNetworkManager::StorePawnComponents();
	SpecimenPawnMovement = SpecimenPawn->FindComponentByClass<UFloatingPawnMovement>();
}

void ADrivingNeuralNetworkManager::ResetFitnessProperties()
{
	ANeuralNetworkManager::ResetFitnessProperties();
	DistanceTraveled = 0.f;
	SpeedSum = 0.f;
	SpeedCount = 0;
	CurrentNumberOfLaps = 0;
	bIsIncreaseLapAllowed = false;
}

void ADrivingNeuralNetworkManager::ResetPawnProperties()
{
	SpecimenPawnMovement->Velocity = FVector(0.f);
	SpecimenPawn->GetController()->SetControlRotation(BaseRotation);
	MovePawnToTargetPoint();
	//SpecimenPawn->TeleportTo(SpawningTransform.GetLocation(),SpawningTransform.Rotator(),true,true);
	SpecimenPawnMovement->Velocity = FVector(0.f);

}

float ADrivingNeuralNetworkManager::CalculateFitness_Implementation(float StepTime)
{
	FVector Velocity = SpecimenPawn->GetVelocity();
	Velocity.Z = 0.f;
	float Speed = Velocity.Size2D();

	DistanceTraveled += Speed * StepTime;

	float CurrentSplineLenght = FindDistanceAlongSplineClosestToWorldLocation(*DrivingLane->Spline, SpecimenPawn->GetActorLocation());
	Velocity.Normalize();
	FVector Direction = DrivingLane->Spline->GetDirectionAtDistanceAlongSpline(CurrentSplineLenght, ESplineCoordinateSpace::World);
	Direction.Normalize();
	float dot = FVector::DotProduct(Direction, Velocity);
	SpeedSum += Speed * dot;
	++SpeedCount;
	DrawDebugLine(GetWorld(), SpecimenPawn->GetActorLocation(), SpecimenPawn->GetActorLocation() + (DrivingLane->Spline->GetDirectionAtDistanceAlongSpline(CurrentSplineLenght, ESplineCoordinateSpace::World)), INFO, false, StepTime * 1.1f, 0, 10.f);
	float DistanceFitness = DistanceTraveled * DistanceConstant /* FMath::Sign(SpeedSum)*/ + CurrentNumberOfLaps * DrivingLane->Spline->GetSplineLength() / (DistanceTraveled + 1) * LapConstant;  //(CurrentSplineLenght + CurrentNumberOfLaps * DrivingLane->Spline->GetSplineLength()) * DistanceConstant / (DistanceTraveled + 1) * FMath::Sign(SpeedSum);
	float SpeedFitness = SpeedSum / SpeedCount / (SpecimenPawnMovement->MaxSpeed)  * SpeedConstant;
	float TimeFitness = CurrentRunTime * TimeConstant;
	PRINT_ST("Current Speed: " + DrivingLane->Spline->GetDirectionAtDistanceAlongSpline(CurrentSplineLenght, ESplineCoordinateSpace::World).ToString(), StepTime, INFO);

	return  DistanceFitness * SpeedFitness - TimeFitness;
}

bool ADrivingNeuralNetworkManager::CheckEndConditions() const
{
	bool bShouldEnd = false;
	if(ANeuralNetworkManager::CheckEndConditions() ||  CurrentNumberOfLaps >= MaxNumberOfLaps)
	{
		bShouldEnd = true;
	}
	return bShouldEnd;
}
