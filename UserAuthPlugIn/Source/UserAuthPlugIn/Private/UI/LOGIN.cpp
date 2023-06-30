// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/LOGIN.h"
#include <Components/Button.h>
#include <Components/ComboBoxString.h>
#include <Components/EditableText.h>
#include <Kismet/GameplayStatics.h>
#include <Json.h>
#include <JsonUtilities.h>
#include <Networkmanagers/Request/LogInRequest.h>
#include <Networkmanagers/Request/GetCompaniesRequest.h>

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

void ULOGIN::PopulateCompanyNames(const TArray<FString>& Companies)
{
	for (FString Company : Companies)
	{
		CompaniesComboBox->AddOption(Company);
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
		M_LoginRequest->StartRequets(M_LogInURL, LogInJsonObject, ERequestType::POST, this);
	}
	
}

void ULOGIN::SetLoginInstance(ALogInRequest* LoginRequest, const FString& LogInURL)
{
	M_LogInURL = LogInURL;
	M_LoginRequest = LoginRequest;
}

void ULOGIN::SetGetCompaniesInstance(AGetCompaniesRequest* GetCompaniesRequest, const FString& GetCompaniesURL)
{
	M_GetCompaniesURL = GetCompaniesURL;
	M_GetCompaniesRequest = GetCompaniesRequest;
	M_GetCompaniesRequest->StartRequets(M_GetCompaniesURL, NULL, ERequestType::GET, this);
}
