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
	
		oldrot.Pitch -= rot.X;
		oldrot.Yaw -= rot.Y;
		oldrot.Roll -= rot.Z;
		 
		SelectedActor->SetActorRotation(oldrot.Quaternion());
	}
}

void UGameController::ScaleSelectedRelative(FVector scale)
{
	//Makes the scale uniform along all axes by choosing largest absolute value
	float max = scale.GetMax();
	float min = scale.GetMin();

	FVector newScale;

	if (min*(-1) > max) //probably trying to shrink
		newScale = FVector(min);
	else //probably trying to grow
		newScale = FVector(max);

	newScale *= 0.01;

	if (SelectedActor != nullptr)
	{
		auto oldScale = SelectedActor->GetActorScale3D();
		oldScale += newScale; //scale;
		oldScale = oldScale.GetAbs();

		SelectedActor->SetActorScale3D(oldScale);
	}
}

void UGameController::DeleteSelected()
{
	if (SelectedActor != nullptr)
	{
		SelectedActor->Destroy();
	}
}

#pragma endregion Transform Relative

AAsset * UGameController::CreateObjectAtStart(FName pathName, FVector location, FVector scale) {
	auto spawnedActor = GetWorld()->SpawnActor<AAsset>(AAsset::StaticClass(), location, FRotator::ZeroRotator);
	auto obj = LoadObjFromPath<UStaticMesh>(pathName);
	TArray<UStaticMeshComponent*> Components;
	spawnedActor->GetComponents<UStaticMeshComponent>(Components);
	for (int32 i = 0; i<Components.Num(); i++)
		 {
		UStaticMeshComponent * StaticMeshComponent = Components[i];
		StaticMeshComponent->SetStaticMesh(obj);
		}
	spawnedActor->SetActorScale3D(scale);
	return spawnedActor;
	
}


AAsset * UGameController::CreateObject(FName pathName)
{
	auto spawnedActor = GetWorld()->SpawnActor<AAsset>(AAsset::StaticClass(), getControllerBasedLocation(), FRotator::ZeroRotator);
	auto obj = LoadObjFromPath<UStaticMesh>(pathName);
	TArray<UStaticMeshComponent*> Components;
	spawnedActor->GetComponents<UStaticMeshComponent>(Components);
	for (int32 i = 0; i<Components.Num(); i++)
	{
		UStaticMeshComponent* StaticMeshComponent = Components[i];
		StaticMeshComponent->SetStaticMesh(obj);
	}

	SelectedActor = spawnedActor; // TODO: new code to "force" object selection
	return spawnedActor;
}

#pragma endregion Public Methods

#pragma region Private Methods

FVector UGameController::getClampedLocation(FVector vec, FVector origin, float radius)
{

	if (vec.Equals(FVector(0.0, 0.0, 0.0), 0.0)) {
		vec = laserEnd;
		vec.X = vec.X + 100;
	}
	float objectDistance = FMath::Abs(vec.X - origin.X) + FMath::Abs(vec.Y - origin.Y)
		+ FMath::Abs(vec.Z - origin.Z);

	

	if (objectDistance <= radius) {
		return vec;
	}

	// Calculate where the vector formed from the origin to the vector would intersect with spherical bounds
	FVector recenteredVec = vec - origin;
	recenteredVec.Normalize();
	recenteredVec *= radius;

	return recenteredVec + origin;
}

/*Location Option B for spawning new objects in the world: place them at fixed distance from User,
in the vector direction their laser controller points in. 
TODO: if another object already present, spawn in a slightly different place? Outside other object's bounds?*/
FVector UGameController::getControllerBasedLocation()
{
	float radius = 1000.0; // Maximum distance from user that an actor can be placed
	FVector actorLocation = getClampedLocation(RHPos, GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation(), radius);
	return actorLocation;
}

/* Location Option A for spawning new objects in the world: random location that helps avoid spawning
multiple objects in the same place, but tends to sometimes put them below the ground or in 
unpredictable places. */
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
	FQuat roll = FQuat(-relativeForward, FMath::DegreesToRadians(rot.Z));
	FQuat pitch = FQuat(-relativeRight, FMath::DegreesToRadians(rot.X));
	FQuat yaw = FQuat(-relativeUp, FMath::DegreesToRadians(rot.Y));

	auto rotation = pitch * yaw * roll;
	return rotation;
}


#pragma endregion Private Methods