// Fill out your copyright notice in the Description page of Project Settings.


#include "Networkmanagers/Request/NetworkRequestManagerBase.h"

// Sets default values
ANetworkRequestManagerBase::ANetworkRequestManagerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
  
}

void ANetworkRequestManagerBase::StartRequets(const FString& RequestURL , const TSharedPtr<FJsonObject> JsonObject , const TEnumAsByte<ERequestType> RequestType , UObject* RequestingObject)
{
	if (RequestingObject)
	{
		M_RequestingObject = RequestingObject;
	}

	auto Http = &FHttpModule::Get();
	auto Request = Http->CreateRequest();
	// Get the passed ULR from the frontEnd
	// The incomming request should contain two fields one for URL and second for a json payload

	///FString URL = JsonObject->TryGetField("URL").Get()->AsString();
	Request->SetURL(RequestURL);
	switch (RequestType)
	{
		case ERequestType::POST:
		{
			Request->SetVerb("POST");
			Request->SetHeader("Content-Type", "application/json");
			// If we have a payload then set it as a body of the post request  
			if (JsonObject.IsValid())
			{
				FString JsonString;
				TSharedRef<TJsonWriter<>> JSONWriter = TJsonWriterFactory<>::Create(&JsonString);
				if (FJsonSerializer::Serialize(JsonObject.ToSharedRef(), JSONWriter))
				{

					Request.Get().SetContentAsString(JsonString);
				}
				JSONWriter->Close();
			}
			break;
		}
		case ERequestType::GET:
		{
			Request->SetVerb("GET");
			break;
		}
		case ERequestType::PUT:
		{
			Request->SetVerb("PUT");
			break;
		}
		case ERequestType::DELETE:
		{
			Request->SetVerb("DELETE");
			break;
		}
	}
	// Finally process the request 
	Request->ProcessRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &ANetworkRequestManagerBase::OnRequestComplete);//calls back the function on request complete
}


// Called when the game starts or when spawned
void ANetworkRequestManagerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANetworkRequestManagerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANetworkRequestManagerBase::OnRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	if (bWasSuccessful && Response.Get()->GetResponseCode()== EHttpResponseCodes::Ok)
	{
		OnRequestProcessed(Response);
	}
}

void ANetworkRequestManagerBase::OnRequestProcessed(const FHttpResponsePtr& Response)
{
	FString ResponseString = Response.Get()->GetContentAsString();
	UE_LOG(LogTemp, Warning, TEXT("%s is the response from node"), *ResponseString);
	//if (FJsonSerializer::Deserialize(JsonReader, JsonObject))
	OnRequestCompleteCallback.Broadcast(ResponseString);
}
