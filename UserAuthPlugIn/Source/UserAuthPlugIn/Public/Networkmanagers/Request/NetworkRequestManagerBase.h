// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HttpModule.h"
#include "http.h"
#include <HTTP/Public/Interfaces/IHttpRequest.h>
#include <HTTP/Public/Interfaces/IHttpResponse.h>
#include "JsonUtilities.h"
#include <Json/Public/Dom/JsonObject.h>
#include "NetworkRequestManagerBase.generated.h"
/*
*  Forms a base for all requests 
*/

//creates an delegate to bind call back from other classes 
UENUM()
enum class ERequestType: int32
{

	GET,
	POST,
	PUT,
	DELETE
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FONRequestCompleteDelegate, const FString& , JsonString);

UCLASS()
class USERAUTHPLUGIN_API ANetworkRequestManagerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANetworkRequestManagerBase();

#pragma region MemberVariables
	FONRequestCompleteDelegate OnRequestCompleteCallback;
#pragma endregion


	

protected:
	// Called when the game starts or when spawned
#pragma region MemberFunctions
	virtual void BeginPlay() override;
	virtual void StartRequets(const FString& RequestURL, const TSharedPtr<FJsonObject> JsonObject ,const TEnumAsByte<ERequestType> RequestType);
	virtual void OnRequestProcessed(const FHttpResponsePtr& Response);
#pragma endregion
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
private:
	void OnRequestComplete(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);
};
