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



// Called every frame
void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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


