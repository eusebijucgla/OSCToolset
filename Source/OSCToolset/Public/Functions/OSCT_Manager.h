// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "OSCT_ETypes.h"
#include "OSCT_Modules.h"

#include "OSCT_Manager.generated.h"

/**
 * 
 */
UCLASS()
class OSCTOOLSET_API UOSCT_Manager : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
private:
	UFUNCTION()
	static UOSCT_Master* GetMaster(const UObject* WorldContextObject);
	
public:
	
	//Receivers
	UFUNCTION(BlueprintCallable, Category = "OSCToolset|Receivers", meta = (WorldContext = "WorldContextObject", DefaultToSelf = "Owner", HidePin = "Owner"))
	static void AddReceiver(UObject* WorldContextObject, FOSCT_Receiver Receiver, UObject* Owner);
	
	UFUNCTION(BlueprintCallable, Category = "OSCToolset", meta = (WorldContext = "WorldContextObject", DefaultToSelf = "Owner", HidePin = "Owner"))
	static void AddManyReceivers(UObject* WorldContextObject, TArray<FOSCT_Receiver> Receivers, UObject* Owner);

	UFUNCTION(BlueprintCallable, Category = "OSCToolset", meta = (WorldContext = "WorldContextObject", DefaultToSelf = "Owner", HidePin = "Owner"))
	static void AddReceiversFromDataTable(UObject* WorldContextObject, UDataTable* InTable, UObject* Owner);
	
	UFUNCTION(BlueprintCallable, Category = "OSCToolset", meta = (WorldContext = "WorldContextObject", DefaultToSelf = "Owner", HidePin = "Owner"))
	static void RemoveReceiver(UObject* WorldContextObject, FOSCT_Receiver Receiver, UObject* Owner);
	
	UFUNCTION(BlueprintCallable, Category = "OSCToolset", meta = (WorldContext = "WorldContextObject"))
	static void RemoveAllReceivers(UObject* WorldContextObject);
	
	UFUNCTION(BlueprintPure, Category = "OSCToolset|Receivers", meta = (DisplayName = "Get Receiver Address", CompactNodeTitle = "Address"))
	static FString GetReceiverAddress(const FOSCT_Receiver& Receiver);

	//Proxies
	UFUNCTION(BlueprintCallable, Category = "OSCToolset|Receivers|Proxy", meta = (WorldContext = "WorldContextObject", DefaultToSelf = "Owner", HidePin = "Owner"))
	static UOSCT_Proxy* GetReceiverProxy(UObject* WorldContextObject, FOSCT_Receiver Receiver, UObject* Owner);
	
	UFUNCTION(BlueprintCallable, Category = "OSCToolset|Receivers|Proxy", meta = (WorldContext = "WorldContextObject", DefaultToSelf = "Owner", HidePin = "Owner"))
	static TArray<UOSCT_Proxy*> GetManyReceiverProxies(UObject* WorldContextObject, TArray<FOSCT_Receiver> Receivers, UObject* Owner);
	
	UFUNCTION(BlueprintCallable, Category = "OSCToolset|Receivers|Proxy", meta = (WorldContext = "WorldContextObject", DefaultToSelf = "Owner", HidePin = "Owner"))
	static TArray<UOSCT_Proxy*> GetReceiverProxiesFromDataTable(UObject* WorldContextObject, UDataTable* InTable, UObject* Owner);
	
	//Senders
	UFUNCTION(BlueprintCallable, Category = "OSCToolset|Senders", meta = (WorldContext = "WorldContextObject", DefaultToSelf = "Owner", HidePin = "Owner"))
	static void SendEvent(UObject * WorldContextObject, UPARAM(ref) FOSCT_Sender& Sender, UObject* Owner);
	
	UFUNCTION(BlueprintCallable, Category = "OSCToolset|Senders", meta = (WorldContext = "WorldContextObject", DefaultToSelf = "Owner", HidePin = "Owner"))
	static void SendBool(UObject * WorldContextObject, UPARAM(ref) FOSCT_Sender& Sender, const bool Value, UObject* Owner);
	
	UFUNCTION(BlueprintCallable, Category = "OSCToolset|Senders", meta = (WorldContext = "WorldContextObject", DefaultToSelf = "Owner", HidePin = "Owner"))
	static void SendFloat(UObject * WorldContextObject, UPARAM(ref) FOSCT_Sender& Sender, const float Value, UObject* Owner);

	UFUNCTION(BlueprintCallable, Category = "OSCToolset|Senders", meta = (WorldContext = "WorldContextObject", DefaultToSelf = "Owner", HidePin = "Owner"))
	static void SendInteger(UObject * WorldContextObject, UPARAM(ref) FOSCT_Sender& Sender, const int32 Value, UObject* Owner);

	UFUNCTION(BlueprintCallable, Category = "OSCToolset|Senders", meta = (WorldContext = "WorldContextObject", DefaultToSelf = "Owner", HidePin = "Owner"))
	static void SendVector2(UObject * WorldContextObject, UPARAM(ref) FOSCT_Sender& Sender, const FVector2D Value, UObject* Owner);
	
	UFUNCTION(BlueprintCallable, Category = "OSCToolset|Senders", meta = (WorldContext = "WorldContextObject", DefaultToSelf = "Owner", HidePin = "Owner"))
	static void SendVector3(UObject * WorldContextObject, UPARAM(ref) FOSCT_Sender& Sender, const FVector Value, UObject* Owner);
	
	UFUNCTION(BlueprintCallable, Category = "OSCToolset|Senders", meta = (WorldContext = "WorldContextObject", DefaultToSelf = "Owner", HidePin = "Owner"))
	static void SendRotation(UObject * WorldContextObject, UPARAM(ref) FOSCT_Sender& Sender, const FRotator Value, UObject* Owner);
	
	UFUNCTION(BlueprintCallable, Category = "OSCToolset|Senders", meta = (WorldContext = "WorldContextObject", DefaultToSelf = "Owner", HidePin = "Owner"))
	static void SendColor(UObject * WorldContextObject, UPARAM(ref) FOSCT_Sender& Sender, const FLinearColor Value, UObject* Owner);

	UFUNCTION(BlueprintCallable, Category = "OSCToolset|Senders", meta = (WorldContext = "WorldContextObject", DefaultToSelf = "Owner", HidePin = "Owner"))
	static void SendTransform(UObject * WorldContextObject, UPARAM(ref) FOSCT_Sender& Sender, const FTransform Value, UObject* Owner);

	UFUNCTION(BlueprintCallable, Category = "OSCToolset|Senders", meta = (WorldContext = "WorldContextObject", DefaultToSelf = "Owner", HidePin = "Owner"))
	static void SendString(UObject * WorldContextObject, UPARAM(ref) FOSCT_Sender& Sender, const FString& Value, UObject* Owner);

};
