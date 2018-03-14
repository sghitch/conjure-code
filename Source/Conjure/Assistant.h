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
	void SetupPlayerInputComponent(class UInputComponent* InputComponent);

protected:
	// Speaker
	UPROPERTY() USpeaker* MySpeaker;

	// Microphone
	UPROPERTY() UMicrophone* MyMicrophone;
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
};