// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Assistant.h"
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
	AAssistant* assistant = nullptr;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION(BlueprintCallable, Category = "Watson")
	void SetupAssistant();

	
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
};
