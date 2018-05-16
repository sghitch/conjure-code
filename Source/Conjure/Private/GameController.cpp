// Fill out your copyright notice in the Description page of Project Settings.

#include "GameController.h"
#include <ExceptionHandling.h>

#pragma region UE4 Generated

// Sets default values for this component's properties
UGameController::UGameController()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGameController::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UGameController::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

#pragma endregion UE4 Generated

#pragma region Public Methods

#pragma region TransformAbsolute

void UGameController::TranslateSelectedAbsolute(FVector pos)
{
	if (SelectedActor != nullptr) 
	{
		SelectedActor->SetActorLocation(pos);
	}
}

#pragma endregion TransformAbsoulte

#pragma region TransformRelative

void UGameController::TranslateSelectedRelative(FVector pos)
{
	if (SelectedActor != nullptr)
	{
		auto oldpos = SelectedActor->GetActorLocation();

		oldpos += pos;

		SelectedActor->SetActorLocation(oldpos);
	}
}

void UGameController::RotateSelectedRelative(FVector rot)
{
	if (SelectedActor != nullptr)
	{
		auto oldrot = SelectedActor->GetActorRotation();
	
		oldrot.Pitch += rot.X;
		oldrot.Yaw += rot.Y;
		oldrot.Roll += rot.Z;
		 
		SelectedActor->SetActorRotation(oldrot.Quaternion());
	}
}

void UGameController::ScaleSelectedRelative(FVector scale)
{
	if (SelectedActor != nullptr)
	{
		auto oldScale = SelectedActor->GetActorScale3D();
		oldScale += scale;
		oldScale = oldScale.GetAbs();

		SelectedActor->SetActorScale3D(oldScale);
	}
}

#pragma endregion Transform Relative

AAsset * UGameController::CreateObject(FName pathName)
{
	auto spawnedActor = GetWorld()->SpawnActor<AAsset>(AAsset::StaticClass(), getDefaultLocation(), FRotator::ZeroRotator);
	auto obj = LoadObjFromPath<UStaticMesh>(pathName);
	TArray<UStaticMeshComponent*> Components;
	spawnedActor->GetComponents<UStaticMeshComponent>(Components);
	for (int32 i = 0; i<Components.Num(); i++)
	{
		UStaticMeshComponent* StaticMeshComponent = Components[i];
		StaticMeshComponent->SetStaticMesh(obj);
	}

	return spawnedActor;
}

#pragma endregion Public Methods

#pragma region Private Methods

FVector UGameController::getDefaultLocation()
{
	float x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 500));                                                                                                                                                     
	float y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 500));
	float z = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 500));
	FVector actorForwardVectorMulDistance = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraRotation().Vector() * 1000; //1000;
	FVector actorLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() + actorForwardVectorMulDistance;
	
	//Set a default height that is above ground height
	//actorLocation.Y = 50.0f;

	return actorLocation;
}

FQuat UGameController::calculateRelativeRotation(FVector rot)
{
	if (SelectedActor == nullptr)
		return FQuat::Identity;

	auto cameraManager = GetWorld()->GetFirstPlayerController()->PlayerCameraManager;
	auto transformVector = SelectedActor->GetActorLocation() - cameraManager->GetCameraLocation();
	FVector relativeUp = cameraManager->GetActorUpVector();
	FVector relativeRight = cameraManager->GetActorRightVector();
	FVector relativeForward = cameraManager->GetActorForwardVector();

	/*FVector objectRelativeForward = SelectedActor->GetTransform().TransformVectorNoScale(relativeForward);
	FVector objectRelativeRight = SelectedActor->GetTransform().TransformVectorNoScale(relativeRight);
	FVector objectRelativeUp = SelectedActor->GetTransform().TransformVectorNoScale(relativeUp);*/

	//This is janky af, rewrite with sleep
	FQuat roll = FQuat(relativeForward, FMath::DegreesToRadians(rot.Z));
	FQuat pitch = FQuat(relativeRight, FMath::DegreesToRadians(rot.X));
	FQuat yaw = FQuat(relativeUp, FMath::DegreesToRadians(rot.Y));

	auto rotation = pitch * yaw * roll;
	return rotation;
}


#pragma endregion Private Methods