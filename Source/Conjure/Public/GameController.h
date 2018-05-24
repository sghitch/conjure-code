// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Asset.h"
#include "Components/ActorComponent.h"
#include "CoreMinimal.h"
#include "EngineUtils.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"

#include "GameController.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class CONJURE_API UGameController : public UActorComponent
{
	GENERATED_BODY()

public:	
	//UE4 Generated
	UGameController();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, 
		FActorComponentTickFunction* ThisTickFunction) override; //Call every frame

	//TransformAbsolute
	void TranslateSelectedAbsolute(FVector pos);

	//TransformRelative
	void TranslateSelectedRelative(FVector pos);
	void RotateSelectedRelative(FVector rot);
	void ScaleSelectedRelative(FVector scale);
	void DeleteSelected();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GameController)
	AActor* SelectedActor = nullptr;

	AAsset* CreateObject(FName pathName);

	FVector RHPos;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	template <typename ObjClass>
	static FORCEINLINE ObjClass* LoadObjFromPath(const FName& Path)
	{
		if (Path == NAME_None) return NULL;
		//~

		return Cast<ObjClass>(StaticLoadObject(ObjClass::StaticClass(), NULL, *Path.ToString()));
	}

private:
	FVector getControllerBasedLocation();
	FVector getDefaultLocation();
	FQuat calculateRelativeRotation(FVector rot);
};
