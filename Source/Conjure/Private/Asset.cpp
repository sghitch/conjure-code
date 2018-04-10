// Fill out your copyright notice in the Description page of Project Settings.


#include "Asset.h"

FString object;
UStaticMeshComponent* SphereVisual;
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

/*AAsset::AAsset()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	UStaticMeshComponent* SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	SphereVisual->SetupAttachment(RootComponent);
	FString path = FString(TEXT("/Game/StarterContent/Shapes/Shape_"));
	path += object;
	path += FString(TEXT(".Shape_"));
	path += object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(*object);
	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
	}
}*/

// Called when the game starts or when spawned
void AAsset::BeginPlay()
{
	Super::BeginPlay();
	
}

/*void AAsset::setup(FString object) {
	this->object = object;



	//UStaticMeshComponent* SphereVisual = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("VisualRepresentation"));
	//SphereVisual->SetupAttachment(RootComponent);
	FString path = FString(TEXT("/Game/StarterContent/Shapes/Shape_"));
	path += object;
	path += FString(TEXT(".Shape_"));
	path += object;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> SphereVisualAsset(*path);
	if (SphereVisualAsset.Succeeded())
	{
		SphereVisual->SetStaticMesh(SphereVisualAsset.Object);
	}
}*/

// Called every frame
void AAsset::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

