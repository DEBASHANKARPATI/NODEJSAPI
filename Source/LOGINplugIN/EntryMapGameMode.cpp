// Fill out your copyright notice in the Description page of Project Settings.


#include "EntryMapGameMode.h"
#include <Kismet/GameplayStatics.h>
#include "UserAuthPlugIn.h"
#include "UserAuthPlugIn/Public/Networkmanagers/Cache/RequestObjectcache.h"
#include "UserAuthPlugIn/Public/UI/LOGIN.h"
#include "UserAuthPlugIn/Public/Networkmanagers/Request/GetCompaniesRequest.h"
// If you want to start a specific HTTP request craeate a Derived class from the Networkmanager request base
// define your own URL and bind the function on the on Request processed
// You can spawn the Request actor in the game mode once these things are done and call Start request function 
void AEntryMapGameMode::BeginPlay()
{
	Super::BeginPlay();
	if (RequestObjectCacheAsset)
	{
		if (GetWorld())
		{
			RequestObjectsCache =Cast<ARequestObjectCache>(GetWorld()->SpawnActor<ARequestObjectCache>(RequestObjectCacheAsset));
		}
	}
	if (!LoginWidget && EntryLevelUIAsset && RequestObjectsCache)
	{
		//RequestObjectsCache->GetCompaniesRequest->StartRequets(CompaniesNameFetcherURL, NULL, ERequestType::GET);

		if (auto PlayerController = GetWorld()->GetFirstPlayerController())
		{
			UE_LOG(LogTemp, Warning, TEXT("Successfully Created EntryLevel UI"));
			LoginWidget = CreateWidget<ULOGIN>(PlayerController, EntryLevelUIAsset);
			LoginWidget->SetLoginInstance(RequestObjectsCache->LogInRequest,LogInURL);
		}
	}
	if (LoginWidget)
	{
		LoginWidget->AddToViewport();
		if (RequestObjectsCache->GetCompaniesRequest && LoginWidget) {
			RequestObjectsCache->GetCompaniesRequest->OnRequestCompleteCallback.AddDynamic(LoginWidget, &ULOGIN::GetCompaniesFromNodeServer);
			if (RequestObjectsCache->GetCompaniesRequest->OnRequestCompleteCallback.IsBound())
			{
				//start the HTTP REQUEST
				RequestObjectsCache->GetCompaniesRequest->StartRequets(CompaniesNameFetcherURL,NULL,ERequestType::GET);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Could not bind callback function"));
		}
	
	}
	
}




ARequestObjectCache* AEntryMapGameMode::GetRequestObjectCache() const
{
	return RequestObjectsCache;
}

void AEntryMapGameMode::GetCompaniesFromNodeServer(const FString& ResponseString)
{
	if (!ResponseString.IsEmpty())
	{
		//This Response string would be as [{CompanyID:'XYZ'},{CompanyID:'XYZ2'}] which is an array of json objects
		// Companies get fetched in a format of array of JSON Objects
		TArray<TSharedPtr<FJsonValue>> JSONResponseObject;
		TSharedRef<TJsonReader<>> JSONReader = TJsonReaderFactory<>::Create(ResponseString);
		if (FJsonSerializer::Deserialize(JSONReader, JSONResponseObject))
		{
			//now that we have data in our JSON object we can access it using key
			//UE_LOG(LogTemp,Warning,TEXT("%s"),JSONResponseObject.Get().)
			for (TSharedPtr<FJsonValue>& JSONObject : JSONResponseObject)
			{
				//if the element is an object then let's parse the company ID from it
				if (JSONObject.Get()->Type == EJson::Object)
				{
					UE_LOG(LogTemp, Warning, TEXT("%s"), *JSONObject->AsObject().Get()->TryGetField("CompanyID").Get()->AsString());
				}
			}
		}
	}
}
