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

		oldpos.X += pos.X;
		oldpos.Y += pos.Y;
		oldpos.Z += pos.Z;

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
		scale += FVector(1.0f, 1.0f, 1.0f);
		SelectedActor->SetActorScale3D(scale);
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
	FVector actorForwardVectorMulDistance = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraRotation().Vector() * 1000;
	FVector actorLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() + actorForwardVectorMulDistance;

	return actorLocation;
}

#pragma endregion Private Methods