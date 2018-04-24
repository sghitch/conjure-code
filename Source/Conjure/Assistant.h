#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Watson.h"
#include <map>
#include "Common/Speaker.h"
#include "Common/Microphone.h"
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



protected:
	// Speaker
	UPROPERTY() USpeaker* MySpeaker;

	// Microphone
	UPROPERTY() UMicrophone* MyMicrophone;
	void LatencyAudioResponse(FString message);
	void OnMicrophoneStart();
	void OnMicrophoneStop();

	// world modification methods
	void createObject(TArray<FString> intent_arr, std::map<FString, FString> entity_map);

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
};