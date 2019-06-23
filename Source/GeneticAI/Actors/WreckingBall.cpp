// Fill out your copyright notice in the Description page of Project Settings.


#include "WreckingBall.h"
#include "_Components/NeuralInputComponent.h"
#include "Libraries/GeneticFL.h"
#include "GeneticAIGameModeBase.h"
#include "NeuralNetwork/Manager/NeuralNetworkManager.h"
#include "_Components/DamageatorComponent.h"
#include STATICMESH_COMPONENT
#include CAMERA_COMPONENT
#include FLOATINGPAWNMOVEMENT_COMPONENT
#include CONTROLLER
#include SPHERE_COMPONENT

// Sets default values
AWreckingBall::AWreckingBall()
{
	PrimaryActorTick.bCanEverTick = false;
	SphereCollision = CreateDefaultSubobject<USphereComponent>("Sphere");
	SetRootComponent(SphereCollision);

	// Create Mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	Mesh->SetupAttachment(SphereCollision);

	//Create Camera
	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(Mesh);
	// Create Neural Input Component
	NeuralInput = CreateDefaultSubobject<UNeuralInputComponent>("Neural Input");
	// Create Movement component
	MovementComponent = CreateDefaultSubobject<UFloatingPawnMovement>("Movement");
	
	bUseControllerRotationYaw = true;
	TurnSpeed = 45.f;
}

// Called when the game starts or when spawned
void AWreckingBall::BeginPlay()
{
	SphereCollision->OnComponentBeginOverlap.AddDynamic(this, &AWreckingBall::OnBeginOverlap);
	 Manager = UGeneticFL::GetGeneticGameMode(this)->Manager;
	if (Manager)
	{
		OnSpecimenCollision.AddDynamic(Manager, &ANeuralNetworkManager::EndSpecimenSimulation);
	}
	Super::BeginPlay();
}

void AWreckingBall::TurnRight(float Value)
{
	GetController()->SetControlRotation(GetControlRotation() + FRotator(0.f, GetWorld()->DeltaTimeSeconds * Value * TurnSpeed,0.f));
}

void AWreckingBall::MoveForward(float Value)
{
	if (IsValid(MovementComponent))
	{
		//SetActorLocation(GetActorLocation() + GetActorForwardVector() * 500.f * Value * GetWorld()->DeltaTimeSeconds, true);
		MovementComponent->AddInputVector(GetActorForwardVector() * Value);
	}
}

void AWreckingBall::OnBeginOverlap(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->GetComponentByClass(UDamageatorComponent::StaticClass()))
	{
		OnSpecimenCollision.Broadcast();
	}
}


// Called every frame
void AWreckingBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//PRINT_ST("Controller Rotation: " + GetControlRotation().ToString(), DeltaTime, INFO);
}

