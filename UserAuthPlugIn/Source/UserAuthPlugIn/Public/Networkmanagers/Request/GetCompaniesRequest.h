// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NetworkRequestManagerBase.h"
#include "GetCompaniesRequest.generated.h"
/**
 * This is the first request made by the client application to get the list of companies , which will
 * be displayed in a combobox to choose from .
 * Spawn and cache this actor in the gameinstance class of the client
 */
UCLASS()
class USERAUTHPLUGIN_API AGetCompaniesRequest : public ANetworkRequestManagerBase
{
	GENERATED_BODY()
public:
#pragma region exposedfields
	
	UPROPERTY(EditAnywhere)
	FString URL;//URL of the end point to hit to
	
	void StartRequets(const FString& RequestURL, const TSharedPtr<FJsonObject> JsonObject, const TEnumAsByte<ERequestType> RequestType , UObject* RequestingObject) override;
	const TArray<FString>& GetCompanies() const;
#pragma endregion
protected:
	void BeginPlay() override;
private:
	UFUNCTION()
	void GetCompaniesFromNodeServer(const FString& ResponseString);
	TArray<FString> Companies;
};
