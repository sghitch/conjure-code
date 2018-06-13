#include "Assistant.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/CoreUObject/Public/UObject/ConstructorHelpers.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Sound/SoundCue.h"
#include "Components/AudioComponent.h"
#include "Asset.h"
#include <string>
#include <iostream>
#include <map>
#include <functional>
#include "EngineUtils.h"

#define VR //Indicates presence of VR equipment for testing

using namespace std::placeholders;

std::map<FString, std::function<void(TArray<FString>, std::map<FString, FString>)>> functionMap;

std::map<FString, TSharedPtr<FTextToSpeechSynthesizeResponse>> responseCache;

std::map<FString, FString> assetMap;

AAssistant::AAssistant()
{

	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

#ifdef VR
	MyWatson = CreateDefaultSubobject<UWatson>(TEXT("Watson"));
	MySpeaker = CreateDefaultSubobject<USpeaker>(TEXT("Speaker"));
	MyMicrophone = CreateDefaultSubobject<UMicrophone>(TEXT("Microphone"));

	MyConversation = MyWatson->CreateConversation(FAuthentication("8241c345-75d7-4f90-b76f-09514a07b8d0", "gRnKcvq0jTo2"));
	MyTextToSpeech = MyWatson->CreateTextToSpeech(FAuthentication("9481a3c9-f256-4f81-b8d6-8fa10b1d9bfe", "L3URP1vRu1BE"));
	MySpeechToText = MyWatson->CreateSpeechToText(FAuthentication("9c33d075-f79a-43e3-bb61-36064e9b2c75", "Lm0SbXNcAjJV"));
#endif

	GC = CreateDefaultSubobject<UGameController>(TEXT("GameController"));

	//TODO: initialize function map here?
	initialize();
}

void AAssistant::initialize() {
	functionMap[FString(TEXT("createObject"))] = std::bind(&AAssistant::createObject, this, _1, _2);
	functionMap[FString(TEXT("createAsset"))] = std::bind(&AAssistant::createAsset, this, _1, _2);
	functionMap[FString(TEXT("enableRotation"))] = std::bind(&AAssistant::enableRotation, this, _1, _2);
	functionMap[FString(TEXT("enableTranslation"))] = std::bind(&AAssistant::enableTranslation, this, _1, _2);
	functionMap[FString(TEXT("enableScaling"))] = std::bind(&AAssistant::enableScaling, this, _1, _2);
	functionMap[FString(TEXT("enableDelete"))] = std::bind(&AAssistant::enableDelete, this, _1, _2);
	functionMap[FString(TEXT("deleteSelected"))] = std::bind(&AAssistant::deleteSelected, this, _1, _2);
	RotationMode = false;
	ScalingMode = false;
	TranslationMode = false;

	DeleteMode = false;
	InitializeSound();
	
	InitializeAssetMap();
}

void AAssistant::InitializeAssetMap()
{
	assetMap[FString(TEXT("Chair"))] = FString(TEXT("StaticMesh '/Game/Assets/Meshes/SM_Zen_Deck_Chair_01.SM_Zen_Deck_Chair_01'"));
	assetMap[FString(TEXT("Bowl"))] = FString(TEXT("StaticMesh '/Game/Assets/Meshes/SM_WaterBowl.SM_WaterBowl'"));
	assetMap[FString(TEXT("Rock"))] = FString(TEXT("StaticMesh '/Game/Props/SM_Rock.SM_Rock'"));

	assetMap[FString(TEXT("Cup"))] = FString(TEXT("StaticMesh '/Game/Assets/Highball_Glass.Highball_Glass'"));
	assetMap[FString(TEXT("Fork"))] = FString(TEXT("StaticMesh '/Game/Assets/Fork.Fork'"));
	assetMap[FString(TEXT("Knife"))] = FString(TEXT("StaticMesh '/Game/Assets/Knife.Knife'"));
	assetMap[FString(TEXT("Napkin"))] = FString(TEXT("StaticMesh '/Game/Assets/Napkin_Cloth.Napkin_Cloth'"));
	assetMap[FString(TEXT("Plate"))] = FString(TEXT("StaticMesh '/Game/Assets/Plate.Plate'"));
	assetMap[FString(TEXT("Spoon"))] = FString(TEXT("StaticMesh '/Game/Assets/Spoon.Spoon'"));
	assetMap[FString(TEXT("WineGlass"))] = FString(TEXT("StaticMesh '/Game/Assets/Wine_Glass.Wine_Glass'"));
}

void AAssistant::SetupPlayerInputComponent(UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);
	InputComponent->BindAction("Microphone", IE_Pressed, this, &AAssistant::OnMicrophoneStart);
	InputComponent->BindAction("Microphone", IE_Released, this, &AAssistant::OnMicrophoneStop);

	//Testing Functions
	InputComponent->BindAction("SpawnTest", IE_Pressed, this, &AAssistant::TestSpawn);
}


void AAssistant::InitializeSound()
{
	ConstructorHelpers::FObjectFinder<USoundCue> micStartSound(TEXT("'/Game/mic-start.mic-start'"));
	micStartCue = micStartSound.Object;
	micStartComponent = CreateDefaultSubobject <UAudioComponent>(TEXT("micStartComponent"));
	micStartComponent->bAutoActivate = false;

	ConstructorHelpers::FObjectFinder<USoundCue> micStopSound(TEXT("'/Game/mic-stop.mic-stop'"));
	micStopCue = micStopSound.Object;
	micStopComponent = CreateDefaultSubobject <UAudioComponent>(TEXT("micStopComponent"));
	micStopComponent->bAutoActivate = false;

}

void AAssistant::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (micStartCue->IsValidLowLevelFast()) {
		micStartComponent->SetSound(micStartCue);
	}

	if (micStopCue->IsValidLowLevelFast()) {
		micStopComponent->SetSound(micStopCue);
	}


}

void AAssistant::BeginPlay()
{
	Super::BeginPlay();
	start_time = time(0);
	
	myfile.open("C:/Users/student/Documents/Unreal Projects/Conjure/command_success" + std::to_string(rand()) + ".txt"); //default saves in C:\Program Files\Epic Games\UE_4.18\Engine\Binaries\Win64 folder.
	
	FString cPath = "StaticMesh'/Game/StarterContent/Shapes/Shape_Cube.Shape_Cube'";
	FName cubePath = FName(*cPath);
	GC->CreateObjectAtStart(cubePath, FVector(-1660.0, -3280.0, 110.0), FVector(.25, .25, .25));

	/*FString chPath = "StaticMesh'/Game/Assets/Meshes/SM_Zen_Deck_Chair_01.SM_Zen_Deck_Chair_01'";
	FName chairPath = FName(*chPath);
	GC->CreateObjectAtStart(chairPath, FVector(-1660.0, -3280.0, 110.0), FVector(1, 1, 1));*/
	
	
}

void AAssistant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	double seconds_since_start = difftime(time(0), start_time);
	if (seconds_since_start >= 60.0) {
		//print success and failures to log
		myfile << "During minute " << std::to_string(minute) << ": " << std::to_string(successful_calls) << " successful calls, " << std::to_string(failed_calls) << " failed calls.\n";
		start_time = time(0);
		successful_calls = 0;
		failed_calls = 0;
		minute++;
		
	}
}

/*
* Provides audio feedback to user. Used by OnMicrophoneStart to signal assistant is listening
* and by OnMicrophoneStop to signal assistant is processing.
*/
void AAssistant::LatencyAudioResponse(FString message)
{
	FTextToSpeechSynthesizeRequest SynthesisRequest;
	SynthesisRequest.text = message;
	FTextToSpeechSynthesizePendingRequest* T2sRequest = MyTextToSpeech->Synthesize(SynthesisRequest, "en-US_AllisonVoice");
	
	T2sRequest->OnSuccess.BindUObject(this, &AAssistant::OnTextToSpeechSynthesize, SynthesisRequest.text);
	T2sRequest->OnFailure.BindUObject(this, &AAssistant::OnTextToSpeechFailure);
	T2sRequest->Send();
}

void AAssistant::OnMicrophoneStart()
{
	micStartComponent->Play();
	UE_LOG(LogTemp, Warning, TEXT("Microphone Starting..."));
	std::cout << "Print test" << std::endl;
	//TODO: this would be a good place to play sound and signal that the assistant is listening
	//LatencyAudioResponse("Listening"); //Occurs at the wrong time
	MyMicrophone->StartRecording();
	UE_LOG(LogTemp, Warning, TEXT("1"));
}

void AAssistant::OnMicrophoneStop()
{
	micStopComponent->Play();
	UE_LOG(LogTemp, Warning, TEXT("Microphone Stopping..."));
	UE_LOG(LogTemp, Warning, TEXT("2"));
	MyMicrophone->StopRecording();
	//LatencyAudioResponse("Alright"); //Occurs at the wrong time
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

bool AAssistant::GetRotateFlag()
{
	return RotationMode;
}

bool AAssistant::GetScaleFlag()
{
	return ScalingMode;
}

bool AAssistant::GetTranslateFlag()
{
	return TranslationMode;
}

bool AAssistant::GetDeleteFlag()
{
	return DeleteMode;
}

void AAssistant::SetDeleteFlag(bool deleteFlag)
{
	DeleteMode = deleteFlag;
}

void AAssistant::enableRotation(TArray<FString> intent_arr, std::map<FString, FString> entity_map) {

	successful_calls++;
	RotationMode = true;
	ScalingMode = false;
	TranslationMode = false;
	DeleteMode = false;
	

}

void AAssistant::enableTranslation(TArray<FString> intent_arr, std::map<FString, FString> entity_map) {
	successful_calls++;
	RotationMode = false;
	ScalingMode = false;
	TranslationMode = true;
	DeleteMode = false;
	

}

void AAssistant::enableScaling(TArray<FString> intent_arr, std::map<FString, FString> entity_map) {
	successful_calls++;
	LatencyAudioResponse("object selected");
	RotationMode = false;
	ScalingMode = true;
	TranslationMode = false;
	DeleteMode = false;


}

void AAssistant::enableDelete(TArray<FString> intent_arr, std::map<FString, FString> entity_map) {
/*	if (GC->SelectedActor == nullptr) {
		LatencyAudioResponse("No object selected");
		failed_calls++;
	}
	else {
		successful_calls++;
		LatencyAudioResponse("object selected");
		RotationMode = false;
		ScalingMode = false;
		TranslationMode = false;
		DeleteMode = true;
	}*/

	successful_calls++;
	//LatencyAudioResponse("object selected");
	RotationMode = false;
	ScalingMode = false;
	TranslationMode = false;
	DeleteMode = true;
}

void AAssistant::deleteSelected(TArray<FString> intent_arr, std::map<FString, FString> entity_map) {
	if (GC->SelectedActor == nullptr) {
		LatencyAudioResponse("No object selected");
		failed_calls++;
	}
	else {
		successful_calls++;
		//RotationMode = false;
		//ScalingMode = false;
		//TranslationMode = false;
		DeleteMode = true;
	}
}

void AAssistant::clearEditingFlags()
{
	RotationMode = false;
	ScalingMode = false;
	TranslationMode = false;
	DeleteMode = false;
}

void AAssistant::createObject(TArray<FString> intent_arr, std::map<FString, FString> entity_map) {
	
	clearEditingFlags();

	if (entity_map.find(FString(TEXT("Object"))) == entity_map.end()) {
		//This means that no object was specified
	}
	FString object = entity_map.at(FString(TEXT("Object")));
	successful_calls++;
	FString path = "StaticMesh'";
	path += FString(TEXT("/Game/StarterContent/Shapes/Shape_"));
	path += object;
	path += FString(TEXT(".Shape_"));
	path += object;
	path += "'"; 
	FName pathName = FName(*path);

	GC->CreateObject(pathName);

	
}

void AAssistant::createAsset(TArray<FString> intent_arr, std::map<FString, FString> entity_map) {

	clearEditingFlags();

	if (entity_map.find(FString(TEXT("Asset"))) == entity_map.end()) {
		//This means that no object was specified
	} else {
		FString asset = entity_map.at(FString(TEXT("Asset")));
		successful_calls++;
		
		if (assetMap.find(asset) != assetMap.end()) {
			FString path = assetMap.at(asset);
			//FString path = "StaticMesh'";
			//path += FString(TEXT("/Game/Assets/Meshes/SM_Zen_Deck_Chair_01.SM_Zen_Deck_Chair_01'"));
			FName pathName = FName(*path);

			GC->CreateObject(pathName);
		}
	} 
}



FString parseResponseMethod(FString message) {
	int chopIndex = message.Find(":");
	return message.Left(chopIndex);
}

FString parseResponseMessage(FString message) {
	int chopIndex = message.Find(":");
	if (chopIndex == -1) { //This response didn't have a method attached to it.
		return message;
	}
	chopIndex++;
	FString speechMessage = message.RightChop(chopIndex);
	return speechMessage;
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
	FString method = parseResponseMethod(Response->output.text.Last());
	SynthesisRequest.text = parseResponseMessage(Response->output.text.Last());

	
	
	TArray<FString> intentArr;
	std::map<FString, FString> entityMap;
	buildParams(intentArr, entityMap, Response);

	if (functionMap.find(method) != functionMap.end()) {
		functionMap[method](intentArr, entityMap);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Function: %s does not exist"), *method);
		failed_calls++;
	}

	//check if text is in the map
	//if its in the map than call play audio on that response
	//don't do the rest


	if (responseCache.find(SynthesisRequest.text) != responseCache.end()) {
		MySpeaker->PlayAudio(responseCache[SynthesisRequest.text]->audioData, responseCache[SynthesisRequest.text]->audioLength);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("7"));
		FTextToSpeechSynthesizePendingRequest* T2sRequest = MyTextToSpeech->Synthesize(SynthesisRequest, "en-US_AllisonVoice");
		UE_LOG(LogTemp, Warning, TEXT("8"));

		T2sRequest->OnSuccess.BindUObject(this, &AAssistant::OnTextToSpeechSynthesize, SynthesisRequest.text);
		T2sRequest->OnFailure.BindUObject(this, &AAssistant::OnTextToSpeechFailure);
		T2sRequest->Send();
	}


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
	if (!Response->results.IsValidIndex(0)) {
		return;
	}
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

void AAssistant::OnTextToSpeechSynthesize(TSharedPtr<FTextToSpeechSynthesizeResponse> Response, FString text)
{
	//init a text to Response map somewhere else.
	                                                                          
	//try to pass in the text here as well (maybe have last text as global if cant also could cause problems maybe), put text and response into map
	responseCache[text] = Response;
	MySpeaker->PlayAudio(Response->audioData, Response->audioLength);
}

void AAssistant::OnTextToSpeechFailure(FString Error)
{
	UE_LOG(LogTemp, Warning, TEXT("Text To Speech Error: %s"), *Error);
	failed_calls++;
}

void AAssistant::TestSpawn()
{

}