// Copyright (c) 2024, Studio Eusebi Jucgla. All rights reserved 
// MD5: 2245b93ed1bae2e4aff277e6f3ecb8ff
#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "Tickable.h"

#include "OSCServer.h"
#include "OSCClient.h"
#include "OSCToolsetLog.h"

#include "OSCT_Settings.h"
#include "OSCT_ETypes.h"
#include "OSCT_Modules.h"
#include "OSCT_Proxy.h"
#include "Functions/OSCT_Functions.h"
#include "Interfaces/OSCT_Router.h"
#include "UI/SOSCT_Menu.h"

#include "OSCT_Master.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnOSCToolsetEvent);

UCLASS()                  
class OSCTOOLSET_API UOSCT_Master : public UGameInstanceSubsystem, public FTickableGameObject
{
	GENERATED_BODY()

protected:
	virtual void Tick(float DeltaTime) override;
	virtual bool IsTickable() const override { return !IsTemplate(); }
	virtual TStatId GetStatId() const override { RETURN_QUICK_DECLARE_CYCLE_STAT(UOSCT_Master, STATGROUP_Tickables); }

public:
	
	UPROPERTY(BlueprintReadOnly, Category="OSCToolset")
	UOSCServer* OSCT_Server;

	UPROPERTY(BlueprintReadOnly, Category = "OSCToolset")
	TArray<UOSCClient*> OSCT_Clients;

	////// RECEIVERS
	
	UFUNCTION()
	void AddReceiver(FOSCT_Receiver Receiver, UObject* Owner);

	UFUNCTION()
	void AddManyReceivers(TArray<FOSCT_Receiver> Receivers, UObject* Owner);
	
	UFUNCTION()
	void AddReceiversFromDataTable(UDataTable* InTable, UObject* Owner);
	
	UFUNCTION()
	void RemoveReceiver(FOSCT_Receiver Receiver, UObject* Owner);
	
	UFUNCTION()
	void RemoveAllReceivers();

	UFUNCTION()
	UOSCT_Proxy* GetReceiverProxy(const FOSCT_Receiver& Receiver, UObject* Owner);
	
	UFUNCTION()
	TArray<UOSCT_Proxy*> GetManyReceiverProxies(const TArray<FOSCT_Receiver>& Receivers, UObject* Owner);
	
	UFUNCTION()
	TArray<UOSCT_Proxy*> GetReceiverProxiesFromDataTable(UDataTable* InTable, UObject* Owner);

	
	
	////// SENDERS
	UFUNCTION()
	void SetupSender(FOSCT_Sender& Sender, const EOSCT_ModuleType ModuleType, UObject* Owner);
	UFUNCTION()
	void Send_Event(UPARAM(ref) FOSCT_Sender& Sender, UObject* Owner );
	UFUNCTION()
	void Send_Bool(UPARAM(ref) FOSCT_Sender& Sender, const bool Value, UObject* Owner );
	UFUNCTION()
	void Send_Float(UPARAM(ref) FOSCT_Sender& Sender,  const float Value, UObject* Owner);
	UFUNCTION()
	void Send_Integer(UPARAM(ref) FOSCT_Sender& Sender,  const int32 Value, UObject* Owner);
	UFUNCTION()
	void Send_Vector2(UPARAM(ref) FOSCT_Sender& Sender,  const FVector2D& Value, UObject* Owner);
	UFUNCTION()
	void Send_Vector3(UPARAM(ref) FOSCT_Sender& Sender,  const FVector& Value, UObject* Owner);
	UFUNCTION()
	void Send_Rotation(UPARAM(ref) FOSCT_Sender& Sender,  const FRotator& Value, UObject* Owner);
	UFUNCTION()
	void Send_Color(UPARAM(ref) FOSCT_Sender& Sender,  const FLinearColor& Value, UObject* Owner);
	UFUNCTION()
	void Send_Transform(UPARAM(ref) FOSCT_Sender& Sender,  const FTransform& Value, UObject* Owner);
	UFUNCTION()
	void Send_String(UPARAM(ref) FOSCT_Sender& Sender, const FString& Value, UObject* Owner);
	
	UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset")
	FOnOSCToolsetEvent OnOSCToolsetInitialized;

	UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset")
	FOnOSCToolsetEvent OnOSCToolsetShutdown;

	UPROPERTY()
	const UOSCT_Settings* Settings;

	// Initialize the subsystem
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	// Deinitialize or clean up the subsystem
	virtual void Deinitialize() override;
	
	void ToggleOSCTMenu();
	
protected:
	// Event handler for the level change
	void OnLevelChanged(const FString& LevelName);

private:
	// Maps an OSC address string to its unified proxy container
	UPROPERTY()
	TMap<FName, UOSCT_Proxy*> ReceiverProxies;
	
	/// Modules
	TMap<FName, TArray<FOSCT_EventLink>> EventLinks;
	TMap<FName, TArray<FOSCT_EventPackLink>> EventPackLinks;
	
	TMap<FName, TArray<FOSCT_BoolLink>> BoolLinks;
	TMap<FName, TArray<FOSCT_BoolPackLink>> BoolPackLinks;
	
	TMap<FName, TArray<FOSCT_FloatLink>> FloatLinks;
	TMap<FName, TArray<FOSCT_FloatPackLink>> FloatPackLinks;
	
	TMap<FName, TArray<FOSCT_IntegerLink>> IntegerLinks;
	TMap<FName, TArray<FOSCT_IntegerPackLink>> IntegerPackLinks;

	TMap<FName, TArray<FOSCT_Vector2Link>> Vec2Links;
	TMap<FName, TArray<FOSCT_Vector2PackLink>> Vec2PackLinks;
	
	TMap<FName, TArray<FOSCT_Vector3Link>> Vec3Links;
	TMap<FName, TArray<FOSCT_Vector3PackLink>> Vec3PackLinks;
	
	TMap<FName, TArray<FOSCT_RotationLink>> RotationLinks;
	TMap<FName, TArray<FOSCT_RotationPackLink>> RotationPackLinks;
	
	TMap<FName, TArray<FOSCT_ColorLink>> ColorLinks;
	TMap<FName, TArray<FOSCT_ColorPackLink>> ColorPackLinks;
	
	TMap<FName, TArray<FOSCT_TransformLink>> TransformLinks;
	TMap<FName, TArray<FOSCT_TransformPackLink>> TransformPackLinks;
	
	TMap<FName, TArray<FOSCT_StringLink>> StringLinks;
	TMap<FName, TArray<FOSCT_StringPackLink>> StringPackLinks;
	
	TMap<FName, TArray<FOSCT_NoteLink>> NoteLinks;
	
	TMap<FName, EOSCT_RouteType> AddressToType;
	TSet<FName> TickableAddresses; //For checking during tick event.
	
	void CleanupLinks();
	
	// Factory helper to grab an existing proxy or create a new one on the fly
	UOSCT_Proxy* GetOrCreateProxy(const FOSCT_Receiver& Receiver);
	
	template<typename TLink>
	void AddReceiverLink(TMap<FName, TArray<TLink>>& TargetMap, const FName& AddressKey, const FOSCT_Receiver& Receiver, UObject* Owner)
	{
		TLink NewLink;
		NewLink.Data = Receiver;
		NewLink.Owner = Owner;

		// Find the array (or create it) and add the link
		TargetMap.FindOrAdd(AddressKey).AddUnique(NewLink);
		AddressToType.FindOrAdd(AddressKey) = UOSCT_Functions::ConvertModuleTypeToRouteType(Receiver.ModuleType, Receiver.Pack);
	}
	
// Fallback for simple types (float, int, etc)
template<typename T>
static int32 GetMessageSize(const T& Message) { return 2; }

// Overload for TMaps
template<typename K, typename V>
static int32 GetMessageSize(const TMap<K, V>& Message) { return Message.Num(); }

// Overload for TArrays
template<typename InT>
static int32 GetMessageSize(const TArray<InT>& Message) { return Message.Num(); }
	
template<typename TLink, typename TValue>
TArray<TLink>* UpdateAndPrune(TMap<FName, TArray<TLink>>& TargetMap, 
							 TMap<FName, EOSCT_RouteType>& CacheMap, 
							 FName Key, const TValue& NewValue)
	{
		TArray<TLink>* LinkArray = TargetMap.Find(Key);
		if (!LinkArray) return nullptr;

		for (int32 i = LinkArray->Num() - 1; i >= 0; --i)
		{
			TLink& Link = (*LinkArray)[i];
			if (Link.Owner.IsValid())
			{
				Link.TargetValue = NewValue;
				
				// Handle Initialization State
				if (!Link.bInitialized)
				{
					Link.CurrentValue = NewValue;
					Link.bInitialized = true;
					Link.bNeedsInterpolation = false;
					Link.bIsFirstFrame = true;
					
					int32 MessageSize = GetMessageSize(NewValue);
					IOSCT_Router::Execute_OnReceiverInit(Link.GetOwner(), Link.Data, MessageSize);
				}
				else
				{
					// Toggle interpolation flag based on settings
					Link.bNeedsInterpolation = Link.Data.Tick.bEnable;
					//Check if it is a tickable address
					if (Link.Data.Tick.bEnable)
					{
						TickableAddresses.FindOrAdd(Key);
					}	
				}
			}
			else
			{
				LinkArray->RemoveAtSwap(i);
			}
		}

		if (LinkArray->Num() == 0)
		{
			TargetMap.Remove(Key);
			CacheMap.Remove(Key);
			TickableAddresses.Remove(Key);
			return nullptr;
		}
		return LinkArray;
	}
	
	//ExecFunc is the base Interface Event like GET_Float
	//TickFunc is the interpolated version, this is called here for the first frame to receive the initial value on startup.
	//The proxy broadcast is only fired when a proxy receiver is being fetched, see GetOrCreateProxy(const FOSCT_Receiver& Receiver)
	template<typename TLink, typename TValue>
	void RouteOSCMessage(
		const FOSCMessage& Message,
		TMap<FName, TArray<TLink>>& TargetMap,
		TMap<FName, EOSCT_RouteType>& CacheMap,
		FName AddressKey,
		TFunction<bool(const FOSCMessage&, TValue&)> ParseFunc,
		TFunction<void(UObject*, const FOSCT_Receiver&, const TValue&)> ExecFunc,
		TFunction<void(UObject*, const FOSCT_Receiver&, const TValue&)> TickFunc = nullptr,
		TFunction<void(UOSCT_Proxy*, const TValue&)> ProxyBroadcastFunc = nullptr)
		{
			TValue ParsedValue;
			if (ParseFunc(Message, ParsedValue))
			{
				if (auto* Links = UpdateAndPrune(TargetMap, CacheMap, AddressKey, ParsedValue))
				{
					for (TLink& Link : *Links)
					{
						UObject* Target = Link.Owner.Get();
						if (!Target) continue;

						ExecFunc(Target, Link.Data, ParsedValue);
						UOSCT_Functions::DebugReceiverLink(Link, ParsedValue);
						
						if (TickFunc && Link.bIsFirstFrame)
						{
							TickFunc(Target, Link.Data, Link.CurrentValue);
							Link.bIsFirstFrame = false; // Gate closed forever
						}
					}
				}
				// Handle the Proxy Broadcast if a proxy exists!
				if (UOSCT_Proxy** Proxy = ReceiverProxies.Find(AddressKey))
				{
					if (ProxyBroadcastFunc && *Proxy)
					{
						ProxyBroadcastFunc(*Proxy, ParsedValue);
					}
				}
			}
		}
	
	template<typename TLink>
	void ProcessActiveLinksTick(
		TMap<FName, 
		TArray<TLink>>& TargetMap, 
		FName Address, 
		float DeltaTime, 
		bool& bOutStillMoving, 
		TFunction<void(UObject*, const TLink&, const typename TLink::ValueType&)> ExecuteFunc,
		TFunction<void(UOSCT_Proxy*, const typename TLink::ValueType&)> ProxyBroadcastFunc = nullptr)
	{
		if (TArray<TLink>* Links = TargetMap.Find(Address))
		{
			for (TLink& L : *Links)
			{
				if (L.bNeedsInterpolation && L.Owner.IsValid())
				{
					L.Interpolate(DeltaTime, L.Data.Tick.InterpolationSpeed, L.Data.Tick.Tolerance);

					if (L.IsSettled(L.Data.Tick.Tolerance)) 
					{
						L.CurrentValue = L.TargetValue; // Snap to final
						L.bNeedsInterpolation = false; // Stop this specific link
					}
					else 
					{
						bOutStillMoving = true; //modifies the bool bAnyLinkStillMoving = false; from master::Tick func.
					}
					ExecuteFunc(L.Owner.Get(), L, L.CurrentValue);
					// 2. Automatically broadcast the smoothed value to the proxy channel if active
					if (ProxyBroadcastFunc)
					{
						if (UOSCT_Proxy** Proxy = ReceiverProxies.Find(Address))
						{
							if (*Proxy)
							{
								ProxyBroadcastFunc(*Proxy, L.CurrentValue);
							}
						}
					}
				}
			}
		}
	}
	
	
	template<typename TLink>
	bool RemoveReceiverLink(TMap<FName, TArray<TLink>>& TargetMap, 
							TMap<FName, EOSCT_RouteType>& CacheMap, 
							const FName& AddressKey, 
							UObject* Owner)
	{
		TArray<TLink>* Links = TargetMap.Find(AddressKey);
		if (!Links) return false;

		// 1. Optional: Call the "About to be removed" logic if needed
		// 2. Perform the Search & Destroy
		Links->RemoveAll([Owner](const TLink& L) {
			return !L.Owner.IsValid() || L.Owner.Get() == Owner;
		});

		// 3. Cleanup: If the array is now empty, wipe the key from both maps
		if (Links->Num() == 0)
		{
			TargetMap.Remove(AddressKey);
			CacheMap.Remove(AddressKey);
			
			// Clean up the shared proxy so it doesn't linger in memory
			if (UOSCT_Proxy** FoundProxy = ReceiverProxies.Find(AddressKey))
			{
				ReceiverProxies.Remove(AddressKey);
			}
		}
    
		return true;
	}
	
	UFUNCTION()
	void BroadcastOSC(FOSCMessage& Message);
	
	UPROPERTY()
	FString IPV4;

	UFUNCTION()
	void init_OSCT_Master();

	UFUNCTION()
	void shutdown_OSCT_Master();

	void reinit_OSCT_Master();
	
	// Maps an OSC Address to a list of Objects that implement IOSCT_Router
	// FNames faster for lookups compared to FStrings for keys.
    TMap<FName, TArray<FOSCT_ReceiverLink>> ReceiverMap;

	UFUNCTION()
	void ReSendAllReceiversStateUpdate();
	
    // The function bound to OSCT_Server->OnOscMessageReceived
    UFUNCTION()
    void RouteMessage(const FOSCMessage& InMessage, const FString& InAddress, int32 InPort);
	
	// UFUNCTION()
	// static bool CheckIfSettled(const FOSCT_ReceiverLink& Link);
	
	FString OSCT_Base_addr = "/OSCT/";

	FString OSCT_Init_addr = OSCT_Base_addr + "init";

	FString OSCT_Shutdown_addr = OSCT_Base_addr + "shutdown";

	FString OSCT_OnLevelChanged_addr = OSCT_Base_addr + "level_changed";

	void InitializeOSC();

	void SendOSCTBaseMessage(const FString& Message);

	FString GetLocalIPAddress();

	FString SetLocalIPAddress(FString InAddress, const bool UseLocalIPV4, bool Log = false);

	UFUNCTION()
	bool HandleCommands(const FOSCMessage& InMessage);

	void LogSettings();

	TSharedPtr<SOSCT_Menu> OSCTMenu;
};