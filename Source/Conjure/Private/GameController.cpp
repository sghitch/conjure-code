// Fill out your copyright notice in the Description page of Project Settings.

#include "GameController.h"
#include "Asset.h"


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

void UGameController::TranslateSelectedAbsolute(FVector pos)
{
}

void UGameController::RotateSelectedAbsolute(FVector rot)
{
}

void UGameController::ScaleSelectedAbsolute(FVector scale)
{
}

void UGameController::TranslateSelectedRelative(FVector pos)
{
}

void UGameController::RotateSelectedRelative(FVector rot)
{
}

void UGameController::ScaleSelectedRelative(FVector scale)
{
}

AAsset* UGameController::GetSelectedActor()
{
	return nullptr;
}

AAsset* UGameController::SetSelectedActor(FString name)
{
	return nullptr;
}

AAsset* UGameController::GetActorAlongPath(FVector path, FVector origin)
{
	return nullptr;
}
