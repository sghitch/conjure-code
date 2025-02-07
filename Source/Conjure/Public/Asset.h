// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Asset.generated.h"

UCLASS()
class CONJURE_API AAsset : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAsset();
	//void setup(FString object);
	UStaticMeshComponent* SphereVisual;
	//AAsset(FString object);
protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	FString object;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	
	
};
