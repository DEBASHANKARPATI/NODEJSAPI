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
			RequestObjectsCache = Cast<ARequestObjectCache>(GetWorld()->SpawnActor<ARequestObjectCache>(RequestObjectCacheAsset));
		}
	}
	if (!LoginWidget && EntryLevelUIAsset && RequestObjectsCache)
	{
		//RequestObjectsCache->GetCompaniesRequest->StartRequets(CompaniesNameFetcherURL, NULL, ERequestType::GET);

		if (auto PlayerController = GetWorld()->GetFirstPlayerController())
		{
			UE_LOG(LogTemp, Warning, TEXT("Successfully Created EntryLevel UI"));
			LoginWidget = CreateWidget<ULOGIN>(PlayerController, EntryLevelUIAsset);
			if (LoginWidget)
			{
				LoginWidget->SetGetCompaniesInstance(RequestObjectsCache->GetCompaniesRequest, CompaniesNameFetcherURL);
				LoginWidget->SetLoginInstance(RequestObjectsCache->LogInRequest, LogInURL);
			}
		}
	}

	if (LoginWidget)
	{
		LoginWidget->AddToViewport();
	}
}
ARequestObjectCache* AEntryMapGameMode::GetRequestObjectCache() const
{
	return RequestObjectsCache;
}