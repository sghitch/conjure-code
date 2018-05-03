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
	if (selectedActor != nullptr) 
	{
		selectedActor->SetActorLocation(pos);
	}
}

#pragma endregion TransformAbsoulte

#pragma region TransformRelative

void UGameController::TranslateSelectedRelative(FVector pos)
{
	if (selectedActor != nullptr)
	{
		auto oldpos = selectedActor->GetActorLocation();

		oldpos.X += pos.X;
		oldpos.Y += pos.Y;
		oldpos.Z += pos.Z;

		selectedActor->SetActorLocation(oldpos);
	}
}

void UGameController::RotateSelectedRelative(FVector rot)
{
	if (selectedActor != nullptr)
	{
		auto oldrot = selectedActor->GetActorRotation();

		oldrot.Pitch += rot.X;
		oldrot.Yaw += rot.Y;
		oldrot.Roll += rot.Z;
		 
		selectedActor->SetActorRotation(oldrot.Quaternion());
	}
}

void UGameController::ScaleSelectedRelative(FVector scale)
{
	if (selectedActor != nullptr)
	{
		selectedActor->SetActorScale3D(scale);
	}
}

#pragma endregion Transform Relative

#pragma region Selection

AAsset* UGameController::GetSelectedActor()
{
	return selectedActor;
}

AAsset* UGameController::SetSelectedActor(FString name)
{
	for (TActorIterator<AAsset> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
		AAsset *asset = *ActorItr;
		if (asset->GetName().Equals(name))
		{
			selectedActor = asset;
		}
	}

	return selectedActor;
}

AAsset * UGameController::SetSelectedActor(AActor actor)
{
	return SetSelectedActor(actor.GetName());
}

AAsset* UGameController::GetActorAlongPath(FVector path, FVector origin)
{
	return nullptr;
}

#pragma endregion Selection

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
	FVector playerPos = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	FVector actorForwardVectorMulDistance = GetWorld()->GetFirstPlayerController()->GetActorForwardVector() * 1000;
	FVector actorLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() + actorForwardVectorMulDistance;

	return actorLocation;
}

#pragma endregion Private Methods