#pragma once
#include "AllowWindowsPlatformTypes.h"
#include <windows.h>
#include "HideWindowsPlatformTypes.h"

#include <map>

#include "Common/Microphone.h"
#include "Common/Speaker.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "CoreMinimal.h"
#include "GameController.h"
#include "GameFramework/Pawn.h"
#include "Watson.h"
#include <iostream>
#include <fstream>
#include <time.h>

#include "Assistant.generated.h"




//Enter Your Project Name Below
UCLASS()
class CONJURE_API AAssistant : public APawn
{
	GENERATED_BODY()

public:
	AAssistant();
	void BeginPlay() override;
	void Tick(float DeltaTime) override;
	void initialize();
	void SetupPlayerInputComponent(class UInputComponent* InputComponent);
	void LatencyAudioResponse(FString message);
	void OnMicrophoneStart();
	void OnMicrophoneStop();

	//Called by MyCharacter
	bool GetTranslateFlag();
	bool GetScaleFlag();
	bool GetRotateFlag();

	//GC
	UPROPERTY() UGameController* GC;

<<<<<<< HEAD
	std::ofstream myfile;
=======
	//Latency Feedback
	UPROPERTY(BlueprintReadOnly, Category = "Audio")
		USoundCue * micStartCue;

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
		USoundCue *micStopCue;

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
		UAudioComponent *micStartComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Audio")
		UAudioComponent *micStopComponent;

	void InitializeSound();
	void PostInitializeComponents();
>>>>>>> master

protected:
	// Speaker
	UPROPERTY() USpeaker* MySpeaker;

	// Microphone
	UPROPERTY() UMicrophone* MyMicrophone;
	
	//Info needed by MyCharacter
	bool TranslationMode; //TODO: eventually make these enums
	bool ScalingMode;
	bool RotationMode;
	
	//MyCharacter
	void SetupMyCharacter();

	// world modification methods
	void enableTranslation(TArray<FString> intent_arr, std::map<FString, FString> entity_map);
	void enableScaling(TArray<FString> intent_arr, std::map<FString, FString> entity_map);
	void clearEditingFlags();
	void createObject(TArray<FString> intent_arr, std::map<FString, FString> entity_map);
	void enableRotation(TArray<FString> intent_arr, std::map<FString, FString> entity_map);
	// Watson
	UPROPERTY() UWatson* MyWatson;

	// Conversation
	UPROPERTY() UConversation* MyConversation;
	TSharedPtr<FConversationMessageResponse> LastResponse;

	//void buildParams(TArray<FString> &intentArr, std::map<FString, FString> &entityMap, TSharedPtr<FConversationMessageResponse> Response);

	void OnConversationMessage(TSharedPtr<FConversationMessageResponse> Response);
	void OnConversationFailure(FString Error);

	// TextToSpeech
	UPROPERTY() UTextToSpeech* MyTextToSpeech;
	void OnTextToSpeechSynthesize(TSharedPtr<FTextToSpeechSynthesizeResponse> Response);
	void OnTextToSpeechFailure(FString Error);

	// SpeechToText
	UPROPERTY() USpeechToText* MySpeechToText;
	void OnSpeechToTextRecognize(TSharedPtr<FSpeechToTextRecognizeResponse> Response);
	void OnSpeechToTextFailure(FString Error);

	//Mesh Component
	UStaticMeshComponent* SphereVisual;

	FORCEINLINE FName GetObjPath(const UObject* Obj)
	{
		if (!Obj)
		{
			return NAME_None;
		}
		//~

		FStringAssetReference ThePath = FStringAssetReference(Obj);

		if (!ThePath.IsValid()) return NAME_None;

		//The Class FString Name For This Object
		FString Str = Obj->GetClass()->GetDescription();

		Str += "'";
		Str += ThePath.ToString();
		Str += "'";

		return FName(*Str);
	}


	template <typename ObjClass>
	static FORCEINLINE ObjClass* LoadObjFromPath(const FName& Path)
	{
		if (Path == NAME_None) return NULL;
		//~

		return Cast<ObjClass>(StaticLoadObject(ObjClass::StaticClass(), NULL, *Path.ToString()));
	}

	//Test Functions
	void TestSpawn();

	time_t start_time = time(0);
	int successful_calls = 0;
	int failed_calls = 0;
	int minute = 0;
};