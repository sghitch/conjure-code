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
	//LeftHandComponent->AttachTo(VROriginComp);

	RightHandComponent = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("RightHand"));
	RightHandComponent->Hand = EControllerHand::Right;
	RightHandComponent->SetupAttachment(VROriginComp);
	//RightHandComponent->AttachTo(VROriginComp);
	
	/*for (TObjectIterator<AStaticMeshActor> act; act; ++act)
	{
		FString actorName = act->GetName();
		if (actorName.Equals("Assistant"))
		{
			assistant = Cast<AAssistant>(act);
		}

	}*/

	
	

	//Initialize Watson Assistant and listening capabilities
	//MyMicrophone = CreateDefaultSubobject<UMicrophone>(TEXT("Microphone"));
	
	//FVector actorForwardVectorMulDistance = GetWorld()->GetFirstPlayerController()->GetActorForwardVector() * 1000;
	//FVector actorLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation() + actorForwardVectorMulDistance;
	FVector default = { 0, 0, 0 };
	
	
	//auto WatsonAssistant = GetWorld()->SpawnActor<AAssistant>(); //(AAssistant::StaticClass(), default, FRotator::ZeroRotator);
	//WatsonAssistant->LatencyAudioResponse("I have been spawned into this world");
	
	//AAssistant *WatsonAssistant = GetWorld()->SpawnActor<AAssistant>;
	//AAssistant WatsonAssistant = GetWorld()- //WORKING ON SPAWNING INSTANCE OF AASSISTANT
	//AAssistant WatsonAssistant = AAssistant::AAssistant();
	//AAssistant();
	// watsonAssistant = AAssistant(); //TODO: not quite sure if this is how we do it but :D

}

// Called when the game starts or when spawned
void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	assistant = nullptr;
	UWorld* world = GetWorld();
	if (world != NULL) {
		for (TActorIterator<AAssistant> Iter(world); Iter; ++Iter) {
			assistant = *Iter;
		}

		if (assistant != nullptr) {
			assistant->LatencyAudioResponse("I have been spawned into this world");
		}

	}
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

// Called to bind functionality to input
void AMyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAction("Microphone", IE_Pressed, this, &AMyCharacter::CallMicrophone);
	PlayerInputComponent->BindAction("Microphone", IE_Released, this, &AMyCharacter::StopMicrophone);
}



/*void AMyCharacter::OnMicrophoneStart()
{
	//AAssistant::OnMicrophoneStart();
}

void AMyCharacter::OnMicrophoneStop()
{
	
	
}*/
