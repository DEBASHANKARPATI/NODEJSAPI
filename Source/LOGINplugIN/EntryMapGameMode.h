// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "EntryMapGameMode.generated.h"
class ULOGIN;
class ARequestObjectCache;
/**
 * Game mode responsible for calling creation of classes which will handle network calls for us
 * make sure to check the plugin and the follow of network calls 
 */
UCLASS()
class LOGINPLUGIN_API AEntryMapGameMode : public AGameModeBase
{
	GENERATED_BODY()
protected:
	void BeginPlay() override;
#pragma region AssetRefferences from network manager
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARequestObjectCache> RequestObjectCacheAsset;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ULOGIN> EntryLevelUIAsset;
#pragma endregion
#pragma region URLS
	UPROPERTY(EditDefaultsOnly)
	FString CompaniesNameFetcherURL;

	UPROPERTY(EditDefaultsOnly)
	FString LogInURL;
#pragma endregion
public:
	ARequestObjectCache* GetRequestObjectCache() const;
private:
	ULOGIN* LoginWidget = nullptr;
	ARequestObjectCache* RequestObjectsCache=nullptr;
	UFUNCTION()
	void GetCompaniesFromNodeServer(const FString& ResponseString);
};
