// Fill out your copyright notice in the Description page of Project Settings.
#pragma once
#include "CoreMinimal.h"
#include "../Request/GetCompaniesRequest.h"
#include "../Request/LogInRequest.h"
#include "RequestObjectCache.generated.h"
/**
 * This class will contain all the objects which will create a URL request and all the response classes 
 * Creates and caches them at begin play itself 
 */
UCLASS()
class USERAUTHPLUGIN_API ARequestObjectCache : public AActor
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AGetCompaniesRequest> GetCompaniesRequestAsset;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ALogInRequest> LogInRequestAsset;
public:
	AGetCompaniesRequest* GetCompaniesRequest;
	ALogInRequest* LogInRequest;

protected:
	void BeginPlay() override;
};
