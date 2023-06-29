// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LOGIN.h"
#include <Components/Button.h>
#include <Components/ComboBoxString.h>
#include <Components/EditableText.h>
#include <Kismet/GameplayStatics.h>
#include <Json.h>
#include <JsonUtilities.h>

bool ULOGIN::Initialize()
{
    //On initialization
    if (Super::Initialize())
    {
		if (LogInbutton)
		{
			LogInbutton->OnClicked.AddDynamic(this, &ULOGIN::OnLogIN);
		}
        return true;
    }
    return false;
}

void ULOGIN::GetCompaniesFromNodeServer(const FString& ResponseString)
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
					CompaniesComboBox->AddOption(JSONObject->AsObject().Get()->TryGetField("CompanyID").Get()->AsString());
				}
			}
		}
	}
}

void ULOGIN::OnLogIN()
{
	UE_LOG(LogTemp, Warning, TEXT("LogInPressed"));
	const FString& UserName = UserID->GetText().ToString();
	const FString& PasswordKey = Password->GetText().ToString();
	const FString& CompanyID = CompaniesComboBox->GetSelectedOption();
	/*const postData = {
	   UserID: "PARTH",
	   Password : "123456",
	   CompanyID : "MICROLEVEL",
	};*/
	TSharedRef<FJsonObject> LogInJsonObject = MakeShareable(new FJsonObject());
	LogInJsonObject->SetStringField("UserID", UserName);
	LogInJsonObject->SetStringField("Password", PasswordKey);
	LogInJsonObject->SetStringField("CompanyID", CompanyID);


	if (M_LoginRequest)
	{
		M_LoginRequest->StartRequets(M_LogInURL, LogInJsonObject, ERequestType::POST);
	}
	
}

void ULOGIN::SetLoginInstance(ALogInRequest* LoginRequest, const FString& LogInURL)
{
	M_LogInURL = LogInURL;
	M_LoginRequest = LoginRequest;
}
