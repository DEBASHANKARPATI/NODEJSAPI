// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <Networkmanagers/Request/LogInRequest.h>
#include "LOGIN.generated.h"

/**
 * UI Used for triggering Sign in and gets an address for connection (IP,PORT) to connect to 
 */
class UEditableText;
class UButton;
class UComboBoxString;
class ALogInRequest;
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
	void GetCompaniesFromNodeServer(const FString& Response);
	UFUNCTION()
	void OnLogIN();

	UFUNCTION()
	void SetLoginInstance(ALogInRequest* LoginRequest,const FString& LogInURL);
private:
	FString M_LogInURL;
	ALogInRequest* M_LoginRequest;
};
