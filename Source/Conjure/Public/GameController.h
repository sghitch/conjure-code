// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Asset.h"
#include "Components/ActorComponent.h"
#include "GameController.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CONJURE_API UGameController : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGameController();

	void TranslateSelectedAbsolute(FVector pos);
	void RotateSelectedAbsolute(FVector rot);
	void ScaleSelectedAbsolute(FVector scale);

	void TranslateSelectedRelative(FVector pos);
	void RotateSelectedRelative(FVector rot);
	void ScaleSelectedRelative(FVector scale);

	AAsset* GetSelectedActor();
	AAsset* SetSelectedActor(FString name);
	AAsset* GetActorAlongPath(FVector path, FVector origin);

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
};
