// Fill out your copyright notice in the Description page of Project Settings.


#include "Asset.h"


// Sets default values
AAsset::AAsset()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UStaticMeshComponent* SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(TEXT("/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube"));
	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
	}
}

// Called when the game starts or when spawned
void AAsset::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AAsset::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

