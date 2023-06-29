// Fill out your copyright notice in the Description page of Project Settings.
#include "Networkmanagers/Cache/RequestObjectCache.h"
void ARequestObjectCache::BeginPlay()
{
	Super::BeginPlay();
	// This Object will instantiate when we start game in the client application 
	
	if (GetCompaniesRequestAsset)
	{
		GetCompaniesRequest = Cast<AGetCompaniesRequest>(GetWorld()->SpawnActor(GetCompaniesRequestAsset));
	}
	if (LogInRequestAsset)
	{
		LogInRequest = Cast<ALogInRequest>(GetWorld()->SpawnActor(LogInRequestAsset));
	}
}


