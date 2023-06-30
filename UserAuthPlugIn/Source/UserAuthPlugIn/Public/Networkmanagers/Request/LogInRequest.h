// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkRequestManagerBase.h"
#include "LogInRequest.generated.h"
/**
 * This is the first request made by the client application to get the list of companies , which will
 * be displayed in a combobox to choose from .
 * Spawn and cache this actor in the gameinstance class of the client
 */
UCLASS()
class USERAUTHPLUGIN_API ALogInRequest : public ANetworkRequestManagerBase
{
	GENERATED_BODY()
public:

#pragma region exposedfields
	void StartRequets(const FString& RequestURL, const TSharedPtr<FJsonObject> JsonObject, const TEnumAsByte<ERequestType> RequestType , UObject* RequestingObject) override;
	
#pragma endregion
protected:
	void BeginPlay() override;
};
