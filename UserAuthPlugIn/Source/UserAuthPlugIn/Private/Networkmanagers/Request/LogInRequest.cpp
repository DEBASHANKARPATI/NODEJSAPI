// Fill out your copyright notice in the Description page of Project Settings.


#include "Networkmanagers/Request/LogInRequest.h"



void ALogInRequest::BeginPlay()
{
	Super::BeginPlay();
	// This Object will instantiate when we start game in the client application 

}


void ALogInRequest::StartRequets(const FString& RequestURL, const TSharedPtr<FJsonObject> JsonObject, const TEnumAsByte<ERequestType> RequestType , UObject* RequestingObject)
{
	if (auto PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (APawn* Pawn = PlayerController->GetPawn())
		{
			if (Pawn->IsLocallyControlled()) // Check for client
			{
				UE_LOG(LogTemp, Warning, TEXT("Started request for getting companies from url"));
				Super::StartRequets(RequestURL, JsonObject, RequestType,RequestingObject);
			}
		}
	}
}

// Implement Callback method  
