// Fill out your copyright notice in the Description page of Project Settings.


#include "Networkmanagers/Request/GetCompaniesRequest.h"
#include "UI/LOGIN.h"

void AGetCompaniesRequest::BeginPlay()
{
	Super::BeginPlay();
	// This Object will instantiate when we start game in the client application 

}


void AGetCompaniesRequest::StartRequets(const FString& RequestURL, const TSharedPtr<FJsonObject> JsonObject, const TEnumAsByte<ERequestType> RequestType , UObject* RequestingObject)
{
	if (auto PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (APawn* Pawn = PlayerController->GetPawn())
		{
			if (Pawn->IsLocallyControlled()) // Check for client
			{
				UE_LOG(LogTemp, Warning, TEXT("Started request for getting companies from url"));
				OnRequestCompleteCallback.AddDynamic(this, &AGetCompaniesRequest::GetCompaniesFromNodeServer);
				Super::StartRequets(RequestURL, JsonObject, RequestType,RequestingObject);
				
			}
		}
	}
}

const TArray<FString>& AGetCompaniesRequest::GetCompanies() const
{
	return Companies;
}


void AGetCompaniesRequest::GetCompaniesFromNodeServer(const FString& ResponseString)
{
	if (!ResponseString.IsEmpty())
	{
		Companies.Empty();
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
					//CompaniesComboBox->AddOption(JSONObject->AsObject().Get()->TryGetField("CompanyID").Get()->AsString());
					Companies.Add(JSONObject->AsObject().Get()->TryGetField("CompanyID").Get()->AsString());
				}
			}
		}
	}
	if (!Companies.IsEmpty())
	{
		if (M_RequestingObject)
		{
			ULOGIN* LogInUI = Cast<ULOGIN>(M_RequestingObject);
			LogInUI->PopulateCompanyNames(Companies);
		}
	}
}