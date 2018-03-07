#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Watson.h"
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

	void CreateMyObject(TArray<FConversationMessageRuntimeEntity> entity_array);

	// Watson
	UPROPERTY() UWatson* MyWatson;

	// Conversation
	UPROPERTY() UConversation* MyConversation;
	TSharedPtr<FConversationMessageResponse> LastResponse;
	void ProcessCommand(TArray<FConversationMessageRuntimeIntent> intent_array, TArray<FConversationMessageRuntimeEntity> entity_array);
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
};