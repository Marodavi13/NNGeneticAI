// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/Engine/Classes/Components/SplineComponent.h"

#define E_NUMBER 2.71828f
//Defines for easier UWYU
#define STATICMESH_COMPONENT 			"Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#define CAMERA_COMPONENT				"Runtime/Engine/Classes/Camera/CameraComponent.h"
#define WORLD							"Runtime/Engine/Classes/Engine/World.h"
#define TARGETPOINT						"Runtime/Engine/Classes/Engine/TargetPoint.h"
#define DRAWDEBUG						"Runtime/Engine/Public/DrawDebugHelpers.h"
#define	PLAYERCONTROLLER				"Runtime/Engine/Classes/GameFramework/PlayerController.h"
#define HORIZONTALBOX					"Runtime/UMG/Public/Components/HorizontalBox.h"
#define VERTICALBOX						"Runtime/UMG/Public/Components/VerticalalBox.h"
#define SAFEZONE						"Runtime/UMG/Public/Components/SafeZone.h"
#define FLOATINGPAWNMOVEMENT_COMPONENT	"Runtime/Engine/Classes/GameFramework/FloatingPawnMovement.h"
#define CONTROLLER						"Runtime/Engine/Classes/GameFramework/Controller.h"
#define TIMERMANAGER					"Runtime/Engine/Public/TimerManager.h"
#define SPHERE_COMPONENT				"Runtime/Engine/Classes/Components/SphereComponent.h"
#define BOX_COMPONENT					"Runtime/Engine/Classes/Components/BoxComponent.h"
#define SCENE_COMPONENT					"Runtime/Engine/Classes/Components/SceneComponent.h"
//----PRINT MACROS-------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Colors
#define ERROR	FColor::Red
#define WARNING FColor::Yellow
#define SUCCESS FColor::Green
#define INFO	FColor::Cyan

//Current Class where this is called
#define CLASS (FString(__FUNCTION__).Left(FString(__FUNCTION__).Find(TEXT(":"))) )

//Current Line Number in the code where this is called
#define LINE  (FString::FromInt(__LINE__))

//Current Class and Line Number where this is called
#define CLASS_LINE (CLASS + "(" + LINE + ")")

// 	Gives you the Class name and exact line number where you print a message to yourself!
#define PRINT_ST(Param1,Time,Color)				if(GEngine) (GEngine->AddOnScreenDebugMessage(-1, Time, Color, *(CLASS_LINE + ": " + Param1)) )

#define PRINT_ST2(Param1,Param2,Time,Color)		if(GEngine) (GEngine->AddOnScreenDebugMessage(-1, Time, Color, *(CLASS_LINE + ": " + Param1 + ", " + Param2)) )

#define PRINT_ST_F(Param1,Param2,Time,Color)	if(GEngine) (GEngine->AddOnScreenDebugMessage(-1, Time, Color, *(CLASS_LINE + ": " + Param1 + ": " + FString::SanitizeFloat(Param2))) )

//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



UENUM(BlueprintType)
enum class EFunction : uint8
{
	NONE,
	SIGMOID,
	RELU
};


static void Sigmoid(float& x)
{
	x = 1 / (1 + powf(E_NUMBER, -x));
}

static void ReLU(float& x)
{
	x = FMath::Max(0.f, x);
}
class USplineComponent;
static float FindDistanceAlongSplineClosestToWorldLocation(const USplineComponent& Spline, const FVector& WorldLocation)
{
	float InputKey = Spline.FindInputKeyClosestToWorldLocation(WorldLocation);
	int32 InputKetTruncated = FMath::TruncToInt(InputKey);
	// Distance to previous spline point
	float BaseDistance = Spline.GetDistanceAlongSplineAtSplinePoint(InputKetTruncated);
	// Distance to next spline point
	float NextPointDistance = Spline.GetDistanceAlongSplineAtSplinePoint(FMath::TruncToInt(InputKetTruncated + 1));
	// Distance inside the range of both points
	float DeltaDistance = (NextPointDistance - BaseDistance) * (InputKey - static_cast<int32>(InputKetTruncated));
	// Total distance
	return BaseDistance + DeltaDistance;

}


