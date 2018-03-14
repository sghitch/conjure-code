#include "Assistant.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Asset.h"
#include <iostream>
#include <map>
#include <functional>

using namespace std::placeholders;

//std::map<FString, void(TArray<FString>, std::map<FString, FString>)> functionMap;
std::map<FString, std::function<void(TArray<FString>, std::map<FString, FString>)>> functionMap;

//typedef void (*FnPtr) (TArray<FString>, std::map<FString, FString>);

//std::map<FString, FnPtr> functionMap;

AAssistant::AAssistant()
{
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;

	MyWatson = CreateDefaultSubobject<UWatson>(TEXT("Watson"));
	MySpeaker = CreateDefaultSubobject<USpeaker>(TEXT("Speaker"));
	MyMicrophone = CreateDefaultSubobject<UMicrophone>(TEXT("Microphone"));

	MyConversation = MyWatson->CreateConversation(FAuthentication("8241c345-75d7-4f90-b76f-09514a07b8d0", "gRnKcvq0jTo2"));
	MyTextToSpeech = MyWatson->CreateTextToSpeech(FAuthentication("9481a3c9-f256-4f81-b8d6-8fa10b1d9bfe", "L3URP1vRu1BE"));
	MySpeechToText = MyWatson->CreateSpeechToText(FAuthentication("9c33d075-f79a-43e3-bb61-36064e9b2c75", "Lm0SbXNcAjJV"));

	//TODO: initialize function map here?
	initialize();
}

void AAssistant::initialize() {
	functionMap[FString(TEXT("createObject"))] = std::bind(&AAssistant::createObject, this, _1, _2);
}

void AAssistant::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAction("Microphone", IE_Pressed, this, &AAssistant::OnMicrophoneStart);
	InputComponent->BindAction("Microphone", IE_Released, this, &AAssistant::OnMicrophoneStop);
}

void AAssistant::BeginPlay()
{
	Super::BeginPlay();
}

void AAssistant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAssistant::OnMicrophoneStart()
{
	UE_LOG(LogTemp, Warning, TEXT("Microphone Starting..."));
	std::cout << "Print test" << std::endl;

	MyMicrophone->StartRecording();
	UE_LOG(LogTemp, Warning, TEXT("1"));
}

void AAssistant::OnMicrophoneStop()
{
	UE_LOG(LogTemp, Warning, TEXT("Microphone Stopping..."));
	UE_LOG(LogTemp, Warning, TEXT("2"));
	MyMicrophone->StopRecording();
	UE_LOG(LogTemp, Warning, TEXT("3"));
	// Make Speech To Text Request
	FSpeechToTextRecognizePendingRequest* Request = MySpeechToText->Recognize(MyMicrophone->GetRecording());
	if (sizeof(*Request) > 0) {
		UE_LOG(LogTemp, Warning, TEXT("4"));
		Request->OnSuccess.BindUObject(this, &AAssistant::OnSpeechToTextRecognize);
		Request->OnFailure.BindUObject(this, &AAssistant::OnSpeechToTextFailure);
		UE_LOG(LogTemp, Warning, TEXT("5"));
		Request->Send();
	}
}



void AAssistant::createObject(TArray<FString> intent_arr, std::map<FString, FString> entity_map) {
	
	if (entity_map.find(FString(TEXT("Object"))) == entity_map.end()) {
		//This means that no object was specified
	}
	FString object = entity_map.at(FString(TEXT("Object")));

	float x = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 500));
	float y = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 500));
	float z = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / 500));
	FVector loc = FVector(x, y, z);
	auto spawnedActor = GetWorld()->SpawnActor<AAsset>(AAsset::StaticClass(), loc, FRotator::ZeroRotator);
}




FString parseResponseMethod(FString message) {
	int chopIndex = message.Find(":");
	return message.Left(chopIndex);
}

void buildParams(TArray<FString> &intentArr, std::map<FString, FString> &entityMap, TSharedPtr<FConversationMessageResponse> Response) {
	TArray<FConversationMessageRuntimeIntent> intents = Response->intents;

	for (int i = 0; i < Response->intents.Num(); i++) {
		intentArr.Add(intents[i].intent);
	}

	TArray<FConversationMessageRuntimeEntity> entityArr = Response->entities;
	for (int i = 0; i < entityArr.Num(); i++) {
		entityMap[entityArr[i].entity] = entityArr[i].value;
	}
}

void AAssistant::OnConversationMessage(TSharedPtr<FConversationMessageResponse> Response)
{
	//UE_LOG(LogTemp, Warning, TEXT("Conversation Success: %s"), *Response->output.text.Last());
	UE_LOG(LogTemp, Warning, TEXT("6"));
	LastResponse = Response;

	// Make Text To Speech Request
	FTextToSpeechSynthesizeRequest SynthesisRequest;
	SynthesisRequest.text = Response->output.text.Last();
	FString method = parseResponseMethod(Response->output.text.Last());
	TArray<FString> intentArr;
	std::map<FString, FString> entityMap;
	buildParams(intentArr, entityMap, Response);

	//createObject(intentArr, entityMap);

	if (functionMap.find(method) != functionMap.end()) {
		functionMap[method](intentArr, entityMap);
		//functionMap[method](intentArr, entityMap);
		//FnPtr f = functionMap[method];
		//(*f)(intentArr, entityMap);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Function: %s does not exist"), *method);
	}


	UE_LOG(LogTemp, Warning, TEXT("7"));
	FTextToSpeechSynthesizePendingRequest* T2sRequest = MyTextToSpeech->Synthesize(SynthesisRequest, "en-US_AllisonVoice");
	UE_LOG(LogTemp, Warning, TEXT("8"));
	T2sRequest->OnSuccess.BindUObject(this, &AAssistant::OnTextToSpeechSynthesize);
	T2sRequest->OnFailure.BindUObject(this, &AAssistant::OnTextToSpeechFailure);
	T2sRequest->Send();
}

void AAssistant::OnConversationFailure(FString Error)
{
	UE_LOG(LogTemp, Warning, TEXT("Conversation Error: %s"), *Error);
}

void AAssistant::OnSpeechToTextRecognize(TSharedPtr<FSpeechToTextRecognizeResponse> Response)
{
	//UE_LOG(LogTemp, Warning, TEXT("Speech To Text Success: %s"), *Response->results[0].alternatives[0].transcript);
	UE_LOG(LogTemp, Warning, TEXT("10"));

	// Make Conversation Request
	FConversationMessageRequest ConversationRequest;
	ConversationRequest.input.text = Response->results[0].alternatives[0].transcript;
	UE_LOG(LogTemp, Warning, TEXT("11"));
	if (LastResponse.Get() != nullptr)
	{
		ConversationRequest.context = LastResponse->context;
		ConversationRequest.output = LastResponse->output;
	}
	// Replace string with Workspace ID in IBM Cloud Conversation
	UE_LOG(LogTemp, Warning, TEXT("12"));
	FConversationMessagePendingRequest* ConvRequest = MyConversation->Message("23df3f30-511a-4f0d-a07f-26624f8df73b", ConversationRequest);
	ConvRequest->OnSuccess.BindUObject(this, &AAssistant::OnConversationMessage);
	ConvRequest->OnFailure.BindUObject(this, &AAssistant::OnConversationFailure);
	UE_LOG(LogTemp, Warning, TEXT("13"));
	ConvRequest->Send();
}

void AAssistant::OnSpeechToTextFailure(FString Error)
{
	UE_LOG(LogTemp, Warning, TEXT("Speech To Text Error: %s"), *Error);
}

void AAssistant::OnTextToSpeechSynthesize(TSharedPtr<FTextToSpeechSynthesizeResponse> Response)
{
	MySpeaker->PlayAudio(Response->audioData, Response->audioLength);
}

void AAssistant::OnTextToSpeechFailure(FString Error)
{
	UE_LOG(LogTemp, Warning, TEXT("Text To Speech Error: %s"), *Error);
}