// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Assistant.h"
#include "Asset.h"

//#include "Common/Microphone.h"
#include "MyCharacter.generated.h"


UCLASS()
class CONJURE_API AMyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AMyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Wrapper to invoke Watson microphone interface
	//UPROPERTY() UMicrophone* MyMicrophone;
	//void OnMicrophoneStart();
	//void OnMicrophoneStop();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void CallMicrophone();
	void StopMicrophone();
	void SetTriggerFlagTrue();
	void SetTriggerFlagFalse();
	bool TriggerPressed;

	FVector GetNormalizedDifference(FVector oldPos, FVector newPos);

	FVector GetTwoHandDifference(FVector curRH, FVector curLH, FVector oldRH, FVector oldLH);
	
	AAssistant* assistant = nullptr;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Watson")
	void SetupAssistant();

	UFUNCTION(BlueprintCallable, Category = "GameController")
	void SetSelectedActor(AActor* selectedActor);

	FVector GetLeftHandPos();
	FVector GetRightHandPos();

	//Scaling factors
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformFactors")
	FVector TRANSLATION_FACTOR = FVector(10.0f, 10.0f, 10.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformFactors")
	FVector SCALING_FACTOR = FVector(10.0f, 10.0f, 10.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TransformFactors")
	FVector ROTATION_FACTOR = FVector(10.0f, 10.0f, 10.0f);

	UFUNCTION(BlueprintCallable, Category = "GameController")
		void SetRHLocation(FVector location);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "LaserBoolean")
	bool TranslateOn;
	
private:
	UPROPERTY(VisibleAnywhere, Category = "Components")
		UCameraComponent* CameraComp;

	/* Component to specify origin for the HMD */
	UPROPERTY(VisibleAnywhere, Category = "Components")
		USceneComponent* VROriginComp;

	UPROPERTY(EditDefaultsOnly, Category = "VR")
		bool bPositionalHeadTracking;

	/* Motion Controllers */
	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UMotionControllerComponent* LeftHandComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		class UMotionControllerComponent* RightHandComponent;

	FVector oldScale;
	FVector oldRHPos;
	FVector oldLHPos;
	FRotator oldRot;
};
