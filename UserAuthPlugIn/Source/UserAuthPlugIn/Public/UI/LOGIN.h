// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LOGIN.generated.h"

/**
 * UI Used for triggering Sign in and gets an address for connection (IP,PORT) to connect to 
 */
class UEditableText;
class UButton;
class UComboBoxString;
class ALogInRequest;
class AGetCompaniesRequest;
UCLASS()
class USERAUTHPLUGIN_API ULOGIN : public UUserWidget
{
	GENERATED_BODY()
protected:
	bool Initialize() override;

	UPROPERTY(meta = (BindWidget))
	UButton* LogInbutton;
	
	UPROPERTY(meta = (BindWidget))
	UEditableText* UserID;

	UPROPERTY(meta = (BindWidget))
	UEditableText* Password;

	UPROPERTY(meta = (BindWidget))
	UComboBoxString* CompaniesComboBox;
public:
	UFUNCTION()
	void PopulateCompanyNames(const TArray<FString>& Companies);
	UFUNCTION()
	void OnLogIN();

	UFUNCTION()
	void SetLoginInstance(ALogInRequest* LoginRequest,const FString& LogInURL);

	UFUNCTION()
	void SetGetCompaniesInstance(AGetCompaniesRequest* GetCompaniesRequest, const FString& GetCompaniesURL);

private:
	FString M_LogInURL;
	FString M_GetCompaniesURL;
	AGetCompaniesRequest* M_GetCompaniesRequest;
	ALogInRequest* M_LoginRequest;
};
