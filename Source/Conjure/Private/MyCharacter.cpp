// Fill out your copyright notice in the Description page of Project Settings.

#include "MyCharacter.h"
#include "HeadMountedDisplay.h"
#include "MotionControllerComponent.h"
#include "Assistant.h"
#include "EngineUtils.h"
#include "UObjectIterator.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "Engine.h"
//static AAssistant watsonAssistant;

// Sets default values
AMyCharacter::AMyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	VROriginComp = CreateDefaultSubobject<USceneComponent>(TEXT("VRCameraOrigin"));
	VROriginComp->AttachTo(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComp->AttachTo(VROriginComp);

	LeftHandComponent = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("LeftHand"));
	LeftHandComponent->Hand = EControllerHand::Left;
	LeftHandComponent->SetupAttachment(VROriginComp);

	RightHandComponent = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightHand"));
	RightHandComponent->Hand = EControllerHand::Right;
	RightHandComponent->SetupAttachment(VROriginComp);


}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	assistant = nullptr;
}

FVector AMyCharacter::GetLeftHandPos()
{
	return LeftHandComponent->GetComponentLocation();
}

FVector AMyCharacter::GetRightHandPos()
{
	return RightHandComponent->GetComponentLocation();
}

/* Calculates the difference between old and new position
* Finds the primary axis that has the most difference*/
FVector AMyCharacter::GetNormalizedDifference(FVector oldPos, FVector newPos) {
	auto delta = newPos - oldPos;
	FVector normalizedVec;

	//Since X has the biggest difference, normalize along x axis
	if ((FMath::Abs(delta.X) >= FMath::Abs(delta.Y)) && (FMath::Abs(delta.X) >= FMath::Abs(delta.Z))) {
		normalizedVec = FVector(delta.X, 0.0f, 0.0f);
	}

	//Since Y has the biggest difference, normalize along y axis
	else if ((FMath::Abs(delta.Y) > FMath::Abs(delta.X)) && (FMath::Abs(delta.Y) > FMath::Abs(delta.Z))) {
		normalizedVec = FVector(0.0f, delta.Y, 0.0f);
	}

	//Since Z has the biggest difference, normalize along z axis
	else {
		normalizedVec = FVector(0.0f, 0.0f, delta.Z);
	}

	return normalizedVec;
}

// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector curPos = RightHandComponent->GetComponentLocation();
	FRotator curRot = RightHandComponent->GetComponentRotation();

	assistant->GC->SetRHLocation(curPos, curRot);
	
	if (TriggerPressed && assistant->GetRotateFlag()) {
		FVector oldRotVec = FVector(oldRot.Pitch, oldRot.Yaw, oldRot.Roll);
		FVector curRotVec = FVector(curRot.Pitch, curRot.Yaw, curRot.Roll);
		assistant->GC->RotateSelectedRelative(
			GetNormalizedDifference(oldRotVec, curRotVec));
	}

	if (TriggerPressed && assistant->GetScaleFlag()) {
		assistant->GC->ScaleSelectedRelative(
			GetNormalizedDifference(oldScale, curPos) * SCALING_FACTOR);
	}

	if (TriggerPressed && assistant->GetTranslateFlag()) {
		assistant->GC->TranslateSelectedRelative(
			GetNormalizedDifference(oldPos, curPos) * TRANSLATION_FACTOR);
	}

	//This allows for enable deletion mode. would have a problem of being stuck on deletion mode because you cant select anything without deleting it
	/*if (TriggerPressed && assistant->GetDeleteFlag()) {
		assistant->GC->DeleteSelected();
	}*/

	//deletes only one object at a time
	if (assistant->GetDeleteFlag()) {
		assistant->GC->DeleteSelected();
		assistant->SetDeleteFlag(false);
	}
	

	oldPos = curPos;
	oldRot = curRot;
}

void AMyCharacter::CallMicrophone()
{
	if (assistant != nullptr) {
		assistant->OnMicrophoneStart();
	}
	
}

void AMyCharacter::StopMicrophone()
{
	if (assistant != nullptr) {
		assistant->OnMicrophoneStop();
	}

}

void AMyCharacter::SetTriggerFlagTrue() //TODO: make this into a lambda
{
	TriggerPressed = true;
	oldScale = RightHandComponent->GetComponentLocation();
}

void AMyCharacter::SetTriggerFlagFalse()
{
	TriggerPressed = false;
}


// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Microphone", IE_Pressed, this, &AMyCharacter::CallMicrophone);
	PlayerInputComponent->BindAction("Microphone", IE_Released, this, &AMyCharacter::StopMicrophone);
	PlayerInputComponent->BindAction("Trigger", IE_Pressed, this, &AMyCharacter::SetTriggerFlagTrue);
	PlayerInputComponent->BindAction("Trigger", IE_Released, this, &AMyCharacter::SetTriggerFlagFalse);
}

void AMyCharacter::SetupAssistant()
{
	UWorld* world = GetWorld();
	if (world != NULL) 
	{
		for (TActorIterator<AAssistant> Iter(world); Iter; ++Iter)
			assistant = *Iter;

		if (assistant != nullptr)
			assistant->LatencyAudioResponse("I have been spawned into this world");
	}
}

void AMyCharacter::SetSelectedActor(AActor* selectedActor)
{
	if (assistant != nullptr)
	{
		assistant->GC->SelectedActor = selectedActor;
	}
}


