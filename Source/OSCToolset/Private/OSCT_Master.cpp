#include "OSCT_Master.h"

#include "OSCManager.h"
#include "OSCT_Modules.h"
#include "Interfaces/OSCT_Router.h"
#include "OSCToolsetLog.h"
#include "Functions/OSCT_Functions.h"
#include "Functions/OSCT_Parsing.h"

#include "OSCT_Settings.h"
#include "UI/SOSCT_Menu.h"
#include "Widgets/SWeakWidget.h"

#include "AssetRegistry/AssetData.h"

#include "SocketSubsystem.h"
#include "IPAddress.h"

#include "Engine/World.h" // Required for UWorld
#include "Engine/Engine.h" //Required for GEngine
#include "Engine/GameViewportClient.h" // Required for GameViewport
#include "CoreMinimal.h"

#include "GameFramework/PlayerController.h" // Required for APlayerController


void UOSCT_Master::AddReceiver(FOSCT_Receiver Receiver, UObject* Owner)
{
    if (!Owner) return;
    
    if (UOSCT_Functions::FormatAddress(
        Receiver.Role,
        Receiver.ModuleType,
        Receiver.Pack,
        Receiver.Address,
        Receiver.FormattedAddress))
    {
        FName AddressKey = FName(*Receiver.FormattedAddress);
        if (!Owner->Implements<UOSCT_Router>())
        {
            UE_LOG(OSCToolset, Warning, TEXT("%s needs to implement OSCT_Router interface. Trying to receive address: %s"), 
                *Owner->GetName(), *Receiver.FormattedAddress);
            return;
        }

        switch (Receiver.ModuleType)
        {
        case EOSCT_ModuleType::EVENT:
            if (Receiver.Pack) AddReceiverLink(EventPackLinks, AddressKey, Receiver, Owner);
            else               AddReceiverLink(EventLinks, AddressKey, Receiver, Owner);
            break;
        case EOSCT_ModuleType::BOOL:
            if (Receiver.Pack) AddReceiverLink(BoolPackLinks, AddressKey, Receiver, Owner);
            else               AddReceiverLink(BoolLinks, AddressKey, Receiver, Owner);
            break;
        case EOSCT_ModuleType::FLOAT:
            if (Receiver.Pack) AddReceiverLink(FloatPackLinks, AddressKey, Receiver, Owner);
            else               AddReceiverLink(FloatLinks, AddressKey, Receiver, Owner);
            break;
        case EOSCT_ModuleType::INT:
            if (Receiver.Pack) AddReceiverLink(IntegerPackLinks, AddressKey, Receiver, Owner);
            else               AddReceiverLink(IntegerLinks, AddressKey, Receiver, Owner);
            break;
        case EOSCT_ModuleType::VEC2:
            if (Receiver.Pack) AddReceiverLink(Vec2PackLinks, AddressKey, Receiver, Owner);
            else               AddReceiverLink(Vec2Links, AddressKey, Receiver, Owner);
            break;
        case EOSCT_ModuleType::VEC3:
            if (Receiver.Pack) AddReceiverLink(Vec3PackLinks, AddressKey, Receiver, Owner);
            else               AddReceiverLink(Vec3Links, AddressKey, Receiver, Owner);
            break;
        case EOSCT_ModuleType::ROTATION:
            if (Receiver.Pack) AddReceiverLink(RotationPackLinks, AddressKey, Receiver, Owner);
            else               AddReceiverLink(RotationLinks, AddressKey, Receiver, Owner);
            break;
        case EOSCT_ModuleType::COLOR:
            if (Receiver.Pack) AddReceiverLink(ColorPackLinks, AddressKey, Receiver, Owner);
            else               AddReceiverLink(ColorLinks, AddressKey, Receiver, Owner);
            break;
        case EOSCT_ModuleType::TRANSFORM:
            if (Receiver.Pack) AddReceiverLink(TransformPackLinks, AddressKey, Receiver, Owner);
            else               AddReceiverLink(TransformLinks, AddressKey, Receiver, Owner);
            break;
        case EOSCT_ModuleType::STRING:
            if (Receiver.Pack) AddReceiverLink(StringPackLinks, AddressKey, Receiver, Owner);
            else               AddReceiverLink(StringLinks, AddressKey, Receiver, Owner);
            break;
        case EOSCT_ModuleType::NOTE:
            AddReceiverLink(NoteLinks, AddressKey, Receiver, Owner);
            break;
        default:
            break;
        }
        AddReceiverLink(ReceiverMap, AddressKey, Receiver, Owner); //The global receiver map
        IOSCT_Router::Execute_OnReceiverAdded(Owner, Receiver);
        UOSCT_Functions::SendReceiverStateUpdate(OSCT_Clients, Receiver, Owner, true);
        
        UE_LOG(OSCToolset, Log, TEXT("Added OSCT %s: %s to Owner: %s"), 
            *UOSCT_Functions::GetEnumString(Receiver.Role), *Receiver.FormattedAddress, *Owner->GetName());    
    }
    else
    {
        // ERROR: This is where you log it!
        UE_LOG(OSCToolset, Error, TEXT("OSCT_Master: Failed to add module from %s. Address or Prefix is missing."), 
            *Owner->GetName());
    }
}

void UOSCT_Master::AddManyReceivers(TArray<FOSCT_Receiver> Receivers, UObject* Owner)
{
    for (const FOSCT_Receiver Module : Receivers)
    {
        AddReceiver(Module, Owner);
    }
}

void UOSCT_Master::AddReceiversFromDataTable(UDataTable* InTable, UObject* Owner)
{
    if (!InTable)
    {
        UE_LOG(OSCToolset, Warning, TEXT("OSCT_Master: Register attempted with null table."));
        return;
    }

    // Ensure the table uses our struct
    if (InTable->RowStruct->IsChildOf(FOSCT_ReceiverRow::StaticStruct()))
    {
        static const FString ContextString(TEXT("OSCT Receiver Rows"));
        TArray<FOSCT_ReceiverRow*> Rows;
        InTable->GetAllRows<FOSCT_ReceiverRow>(ContextString, Rows);
        
        for (FOSCT_ReceiverRow* RowData: Rows)
        {
            if (RowData)
            {
                FOSCT_Receiver NewReceiver(*RowData);
                AddReceiver(NewReceiver, Owner);
            }
        }
    }
}

void UOSCT_Master::RemoveReceiver(FOSCT_Receiver Receiver, UObject* Owner)
{
    if (!Owner) return;
    if (UOSCT_Functions::FormatAddress(
        Receiver.Role,
        Receiver.ModuleType,
        Receiver.Pack,
        Receiver.Address,
        Receiver.FormattedAddress))
    {
        FName AddressKey = FName(*Receiver.FormattedAddress);
        if (const EOSCT_RouteType* RouteTypePtr = AddressToType.Find(AddressKey))
        {
            switch (*RouteTypePtr)
            {
            case EOSCT_RouteType::EVENT:
                RemoveReceiverLink(EventLinks, AddressToType, AddressKey, Owner);
                break;
            case EOSCT_RouteType::EVENT_PACK:
                RemoveReceiverLink(EventPackLinks, AddressToType, AddressKey, Owner);
                break;
            case EOSCT_RouteType::BOOL:
                RemoveReceiverLink(BoolLinks, AddressToType, AddressKey, Owner);
                break;
            case EOSCT_RouteType::BOOL_PACK:
                RemoveReceiverLink(BoolPackLinks, AddressToType, AddressKey, Owner);
                break;
            case EOSCT_RouteType::FLOAT:
                RemoveReceiverLink(FloatLinks, AddressToType, AddressKey, Owner);
                break;
            case EOSCT_RouteType::FLOAT_PACK:
                RemoveReceiverLink(FloatPackLinks, AddressToType, AddressKey, Owner);
                break;
            case EOSCT_RouteType::INT:
                RemoveReceiverLink(IntegerLinks, AddressToType, AddressKey, Owner);
                break;
            case EOSCT_RouteType::INT_PACK:
                RemoveReceiverLink(IntegerPackLinks, AddressToType, AddressKey, Owner);
                break;
            case EOSCT_RouteType::VEC2:
                RemoveReceiverLink(Vec2Links, AddressToType, AddressKey, Owner);
                break;;
            case EOSCT_RouteType::VEC2_PACK:
                RemoveReceiverLink(Vec2PackLinks, AddressToType, AddressKey, Owner);
                break;;
            case EOSCT_RouteType::VEC3:
                RemoveReceiverLink(Vec3Links, AddressToType, AddressKey, Owner);
                break;
            case EOSCT_RouteType::VEC3_PACK:
                RemoveReceiverLink(Vec3PackLinks, AddressToType, AddressKey, Owner);
                break;
            case EOSCT_RouteType::ROTATION:
                RemoveReceiverLink(RotationLinks, AddressToType, AddressKey, Owner);
                break;
            case EOSCT_RouteType::ROTATION_PACK:
                RemoveReceiverLink(RotationPackLinks, AddressToType, AddressKey, Owner);
                break;
            case EOSCT_RouteType::COLOR:
                RemoveReceiverLink(ColorLinks, AddressToType, AddressKey, Owner);
                break;
            case EOSCT_RouteType::COLOR_PACK:
                RemoveReceiverLink(ColorPackLinks, AddressToType, AddressKey, Owner);
                break;
            case EOSCT_RouteType::TRANSFORM:
                RemoveReceiverLink(TransformLinks, AddressToType, AddressKey, Owner);
                break;
            case EOSCT_RouteType::TRANSFORM_PACK:
                RemoveReceiverLink(TransformPackLinks, AddressToType, AddressKey, Owner);
                break;
            case EOSCT_RouteType::STRING:
                RemoveReceiverLink(StringLinks, AddressToType, AddressKey, Owner);
                break;
            case EOSCT_RouteType::STRING_PACK:
                RemoveReceiverLink(StringPackLinks, AddressToType, AddressKey, Owner);
                break;
            case EOSCT_RouteType::NOTE:
                RemoveReceiverLink(NoteLinks, AddressToType, AddressKey, Owner);
                break;
            default:
                UE_LOG(OSCToolset, Warning, TEXT("Did not implement removing the receiver for this type: %s"), *Receiver.FormattedAddress);
                break;
            }
            RemoveReceiverLink(ReceiverMap, AddressToType, AddressKey, Owner);
            UOSCT_Functions::SendReceiverStateUpdate(OSCT_Clients, Receiver, Owner, false);
            UE_LOG(OSCToolset, Log, TEXT("Unregistered OSCT Receiver: %s"), *Receiver.FormattedAddress);
        }
    }
}

void UOSCT_Master::RemoveAllReceivers()
{
    CleanupLinks();    
    UE_LOG(OSCToolset, Log, TEXT("Removed all receivers."));
}

void UOSCT_Master::Initialize(FSubsystemCollectionBase& Collection)
{
    Super::Initialize(Collection);
    
    // Logic to initialize OSC communication or other services
    UE_LOG(OSCToolset, Log, TEXT("OSCT_MasterSubsystem Initialized"));
    IPV4 = GetLocalIPAddress();

    init_OSCT_Master();

    FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UOSCT_Master::OnLevelChanged);
}

void UOSCT_Master::BroadcastOSC(FOSCMessage& Message)
{
    for (UOSCClient* Client : OSCT_Clients)
    {
        if (Client)
        {
            Client->SendOSCMessage(Message);
        }
    }
}

void UOSCT_Master::init_OSCT_Master()
{
    Settings = GetMutableDefault<UOSCT_Settings>();

    if (Settings)
    {
        // Successfully loaded settings, now you can use them to initialize your OSC server/client
        UE_LOG(OSCToolset, Log, TEXT("OSCT Settings Loaded Successfully"));
        InitializeOSC();
        LogSettings();
    }
}

void UOSCT_Master::InitializeOSC()
{
    FString Server_addr = SetLocalIPAddress(Settings->ServerConfig.Address, Settings->UseLocalIPV4);
    int32 Server_port= Settings->ServerConfig.Port;
    bool Multicast_loopback = Settings->MulticastLoopback;
    OSCT_Server = UOSCManager::CreateOSCServer(Server_addr, Server_port, Multicast_loopback, true, "OSCT_Server", this);
    if (OSCT_Server != nullptr)
    {
        // SendOSCTBaseMessage(OSCT_Init_addr);
        // UE_LOG(OSCToolset, Log, TEXT("Send OSCT Master Init message"));
        OSCT_Server->OnOscMessageReceived.AddDynamic(this, &UOSCT_Master::RouteMessage);
    }
    
    
    OSCT_Clients.Empty(); //Clear clients
    for (int32 i = 0; i < Settings->RemoteClients.Num(); i++)
    {
        const FOSCT_Network& Client = Settings->RemoteClients[i];
        FString TargetAddress = SetLocalIPAddress(Client.Address, Settings->UseLocalIPV4);
        FString UniqueClientName = FString::Printf(TEXT("OSCT_Client_%d"), i);
        UOSCClient* NewClient = UOSCManager::CreateOSCClient(TargetAddress, Client.Port, UniqueClientName, this);
        if (NewClient)
        {
            OSCT_Clients.Add(NewClient);
        }
    }
    if (OSCT_Clients.Num() > 0)
    {
        OnOSCToolsetInitialized.Broadcast();
        SendOSCTBaseMessage(OSCT_Init_addr);
        UE_LOG(OSCToolset, Log, TEXT("Initialize OSC from OSCT_Master"));
        
        //Call OSCT_Router Initialized event:
        if (UGameInstance* GI = GetWorld()->GetGameInstance())
        {
            if (GI->Implements<UOSCT_Router>())
            {
                IOSCT_Router::Execute_OnOSCToolsetInitialized(GI);
            }
        }
    }
}

void UOSCT_Master::OnLevelChanged(const FString& LevelName)
{
    // RemoveAllReceivers(); //Make sure the listeners are gone before a new level is loaded.
    UE_LOG(OSCToolset, Log, TEXT("OnLevelChanged: %s"), *LevelName);
    SendOSCTBaseMessage(OSCT_OnLevelChanged_addr);
}

void UOSCT_Master::CleanupLinks()
{
    EventLinks.Empty();
    EventPackLinks.Empty();
    BoolLinks.Empty();
    BoolPackLinks.Empty();
    FloatLinks.Empty();
    FloatPackLinks.Empty();
    IntegerLinks.Empty();
    IntegerPackLinks.Empty();
    Vec2Links.Empty();
    Vec2PackLinks.Empty();
    Vec3Links.Empty();
    Vec3PackLinks.Empty();
    RotationLinks.Empty();
    RotationPackLinks.Empty();
    ColorLinks.Empty();
    ColorPackLinks.Empty();
    TransformLinks.Empty();
    TransformPackLinks.Empty();
    StringLinks.Empty();
    StringPackLinks.Empty();
    NoteLinks.Empty();
    
    ReceiverMap.Empty();
    AddressToType.Empty();
    TickableAddresses.Empty();
    ReceiverProxies.Empty();
}


void UOSCT_Master::SendOSCTBaseMessage(const FString& Message)
{   
    FOSCAddress addr = UOSCManager::ConvertStringToOSCAddress(Message);
    FOSCMessage msg;
    msg = UOSCManager::SetOSCMessageAddress(msg, addr);
    BroadcastOSC(msg);
}

FString UOSCT_Master::GetLocalIPAddress()
{
    bool bCanBind = false;
    TSharedRef<FInternetAddr> Addr = ISocketSubsystem::Get(PLATFORM_SOCKETSUBSYSTEM)->GetLocalHostAddr(*GLog, bCanBind);
    if (Addr->IsValid())
    {
        return Addr->ToString(false); // Returns the IP address without the port
    }
    return FString();
}

FString UOSCT_Master::SetLocalIPAddress(FString InAddress, const bool UseLocalIPV4, bool Log)
{
    if (UseLocalIPV4)
    {
        if (InAddress == "localhost" || InAddress == "127.0.0.1")
        {
            if (IPV4.IsEmpty())
            {
                // In case IPV4 was not found we use localhost
                if (Log) 
                {
                    UE_LOG(OSCToolset, Error, TEXT("Address-> %s OSCT Master could not find an IPV4, using localhost (127.0.0.1)"), *InAddress);
                }
                return "127.0.0.1";
            }
            // If the address that is set is localhost, use the local IPV4. A fix for 5.5
            if (Log)
            {
                UE_LOG(OSCToolset, Log, TEXT("Address-> %s OSCT Master using IPV4: %s as localhost (127.0.0.1)"), *InAddress, *IPV4);
            }
            return IPV4;
        }
    }
    // Otherwise use the selected address :^)
    return InAddress;
}

void UOSCT_Master::LogSettings()
{
    UE_LOG(OSCToolset, Log, TEXT("Server Address: %s | Server Port: %d"), 
        *SetLocalIPAddress(Settings->ServerConfig.Address, Settings->UseLocalIPV4, true), 
        Settings->ServerConfig.Port); 
    
    for (int32 i = 0; i < Settings->RemoteClients.Num(); i++)
    {
        const FOSCT_Network& Client = Settings->RemoteClients[i];
        UE_LOG(OSCToolset, Log, TEXT("Client Address: %s | Client Port: %d"),
                *SetLocalIPAddress(Client.Address, Settings->UseLocalIPV4, true),
                Client.Port);
    }
    // for (UOSCClient* Client : OSCT_Clients)
    // {
    //     if (Client)
    //     {
    //         UE_LOG(OSCToolset, Log, TEXT("Client Address: %s | Client Port: %d"),
    //             *SetLocalIPAddress(Settings->RemoteClients, Settings->UseLocalIPV4, true))
    //     }
    // }
        // *SetLocalIPAddress(Settings->ClientAddress, Settings->UseLocalIPV4, true), 
        // Settings->ClientPort);
    
    
}

void UOSCT_Master::ToggleOSCTMenu()
{
    APlayerController* PlayerController = GEngine->GetFirstLocalPlayerController(GWorld);

    if (GEngine) {
        if (!OSCTMenu.IsValid())
        {
            // Create the Slate widget if it doesn’t already exist
            SAssignNew(OSCTMenu, SOSCT_Menu);

            // Display it on screen
            GEngine->GameViewport->AddViewportWidgetContent(
                SNew(SWeakWidget).PossiblyNullContent(OSCTMenu.ToSharedRef()));
            
            // Bind the re-initialize event
            OSCTMenu->OnReInitOSCT.BindLambda([this]()
                {
                    // Handle re-initialization logic here
                    reinit_OSCT_Master();
                });

            if (PlayerController) {
                FInputModeGameAndUI InputMode;
                InputMode.SetWidgetToFocus(OSCTMenu);
                PlayerController->SetInputMode(InputMode);
                PlayerController->bShowMouseCursor = true;
            }
        
        }
        else
        {
            // Remove or toggle visibility by resetting it
            GEngine->GameViewport->RemoveViewportWidgetContent(OSCTMenu.ToSharedRef());
            OSCTMenu.Reset();

            // Revert input mode to game
            if (PlayerController)
            {
                FInputModeGameOnly InputMode;
                PlayerController->SetInputMode(InputMode);
                PlayerController->bShowMouseCursor = false; // Hide the mouse cursor
            }
        }
    }
}

void UOSCT_Master::Deinitialize()
{
    shutdown_OSCT_Master();

    Super::Deinitialize();
    // Clean up any resources here
    CleanupLinks();
    FCoreUObjectDelegates::PreLoadMap.RemoveAll(this); //Cleanup On Level Loaded Static Delegate.
}

void UOSCT_Master::shutdown_OSCT_Master()
{
    // RemoveAllReceivers();
    
    OnOSCToolsetShutdown.Broadcast();
    
	for (UOSCClient* Client : OSCT_Clients)
	{
	    if (Client)
	    {
	        FOSCAddress Addr = UOSCManager::ConvertStringToOSCAddress(OSCT_Shutdown_addr);
        
	        // Always check if the address is valid before sending
	        if (!Addr.GetFullPath().IsEmpty())
	        {
	            FOSCMessage Msg;
	            UOSCManager::SetOSCMessageAddress(Msg, Addr);
            
	            UOSCManager::AddString(Msg, TEXT("OSCT Shutdown"));

	            Client->SendOSCMessage(Msg);
	        }
	    }
	}
    
    OSCT_Clients.Empty();
    
    if (IsValid(OSCT_Server))
    {
        // Stop listening immediately
        OSCT_Server->OnOscMessageReceived.RemoveAll(this);
        OSCT_Server = nullptr;
    }

    UE_LOG(OSCToolset, Log, TEXT("OSCT_MasterSubsystem Deinitialized"));
}

void UOSCT_Master::reinit_OSCT_Master()
{
    UE_LOG(OSCToolset, Log, TEXT("Start Re-Init the OSCT Master."));
    UE_LOG(OSCToolset, Log, TEXT("Total unique receivers in Map: %d"), ReceiverMap.Num());
    shutdown_OSCT_Master(); //First shutdown and clear
    init_OSCT_Master(); //Then init the OSCT Master again
    
    ReSendAllReceiversStateUpdate(); //For manual reset, we need to call the update.
}

void UOSCT_Master::ReSendAllReceiversStateUpdate()
{
    //Resend the module connected for each added receiver:
    for (auto& Pair : ReceiverMap)
    {
        // Pair.Key is the FName
        // Pair.Value is the TArray<FOSCT_ReceiverLink>
        for (FOSCT_ReceiverLink& Link : Pair.Value)
        {
            UOSCT_Functions::SendReceiverStateUpdate(OSCT_Clients, Link.Data, Link.GetOwner(), true);
        }
    }

}

void UOSCT_Master::SetupSender(FOSCT_Sender& Sender, const EOSCT_ModuleType ModuleType, UObject* Owner)
{
    // Only run this logic if we haven't initialized yet
    if (Sender.FormattedAddress.IsEmpty())
    {
        // 1. Generate the base string
        UOSCT_Functions::FormatAddress(Sender.Role, ModuleType, false, Sender.Address, Sender.FormattedAddress);
        
        // 2. The "/" Fix: Prepend if missing
        if (!Sender.FormattedAddress.StartsWith(TEXT("/")))
        {
            Sender.FormattedAddress = TEXT("/") + Sender.FormattedAddress;
        }

        // 3. Bake the cache immediately
        Sender.CachedFullAddress = UOSCManager::ConvertStringToOSCAddress(Sender.FormattedAddress);
        Sender.ModuleType = ModuleType;

        UE_LOG(OSCToolset, Log, TEXT("Initialized Sender: %s"), *Sender.FormattedAddress);
        UOSCT_Functions::SendSenderStateUpdate(OSCT_Clients, Sender, Owner, true);
    }
}

void UOSCT_Master::Send_Event(FOSCT_Sender& Sender, UObject* Owner)
{
    SetupSender(Sender, EOSCT_ModuleType::EVENT, Owner);
    if (OSCT_Clients.Num() > 0 && !Sender.CachedFullAddress.GetFullPath().IsEmpty())
    {
        float GameTime = GetWorld()->GetRealTimeSeconds();
        FOSCMessage Msg;
        UOSCManager::AddFloat(Msg, GameTime);
        UOSCManager::SetOSCMessageAddress(Msg, Sender.CachedFullAddress);
        BroadcastOSC(Msg);
        UOSCT_Functions::DebugSender(Sender, true);
    }
}

void UOSCT_Master::Send_Bool(FOSCT_Sender& Sender, const bool Value, UObject* Owner)
{
    SetupSender(Sender, EOSCT_ModuleType::BOOL, Owner);
    if (OSCT_Clients.Num() > 0 && !Sender.CachedFullAddress.GetFullPath().IsEmpty())
    {
        FOSCMessage Msg;
        UOSCManager::AddBool(Msg, Value);
        UOSCManager::SetOSCMessageAddress(Msg, Sender.CachedFullAddress);
        BroadcastOSC(Msg);
        UOSCT_Functions::DebugSender(Sender, Value);
    }
}

void UOSCT_Master::Send_Float(FOSCT_Sender& Sender, const float Value, UObject* Owner)
{
    SetupSender(Sender, EOSCT_ModuleType::FLOAT, Owner);
    if (OSCT_Clients.Num() > 0 && !Sender.CachedFullAddress.GetFullPath().IsEmpty())
    {
        FOSCMessage Msg;
        UOSCManager::AddFloat(Msg, Value);
        UOSCManager::SetOSCMessageAddress(Msg, Sender.CachedFullAddress);
        BroadcastOSC(Msg);
        UOSCT_Functions::DebugSender(Sender, Value);
    }
}

void UOSCT_Master::Send_Integer(FOSCT_Sender& Sender, const int32 Value, UObject* Owner)
{
    SetupSender(Sender, EOSCT_ModuleType::INT, Owner);
    if (OSCT_Clients.Num() > 0 && !Sender.CachedFullAddress.GetFullPath().IsEmpty())
    {
        FOSCMessage Msg;
        UOSCManager::AddInt32(Msg, Value);
        UOSCManager::SetOSCMessageAddress(Msg, Sender.CachedFullAddress);
        BroadcastOSC(Msg);
        UOSCT_Functions::DebugSender(Sender, Value);
    }
}

void UOSCT_Master::Send_Vector2(FOSCT_Sender& Sender, const FVector2D& Value, UObject* Owner)
{
    SetupSender(Sender, EOSCT_ModuleType::VEC2, Owner);
    if (OSCT_Clients.Num() > 0 && !Sender.CachedFullAddress.GetFullPath().IsEmpty())
    {
        FOSCMessage Msg;
        UOSCManager::AddFloat(Msg, Value.X);
        UOSCManager::AddFloat(Msg, Value.Y);
        UOSCManager::SetOSCMessageAddress(Msg, Sender.CachedFullAddress);
        BroadcastOSC(Msg);
        UOSCT_Functions::DebugSender(Sender, Value);
    }
}

void UOSCT_Master::Send_Vector3(FOSCT_Sender& Sender, const FVector& Value, UObject* Owner)
{
    SetupSender(Sender, EOSCT_ModuleType::VEC3, Owner);
    if (OSCT_Clients.Num() > 0 && !Sender.CachedFullAddress.GetFullPath().IsEmpty())
    {
        FOSCMessage Msg;
        UOSCManager::AddFloat(Msg, Value.X);
        UOSCManager::AddFloat(Msg, Value.Y);
        UOSCManager::AddFloat(Msg, Value.Z);
        UOSCManager::SetOSCMessageAddress(Msg, Sender.CachedFullAddress);
        BroadcastOSC(Msg);
        UOSCT_Functions::DebugSender(Sender, Value);
    }
}

void UOSCT_Master::Send_Rotation(FOSCT_Sender& Sender, const FRotator& Value, UObject* Owner)
{
    SetupSender(Sender, EOSCT_ModuleType::ROTATION, Owner);
    if (OSCT_Clients.Num() > 0 && !Sender.CachedFullAddress.GetFullPath().IsEmpty())
    {
        FOSCMessage Msg;
        UOSCManager::AddFloat(Msg, Value.Pitch);
        UOSCManager::AddFloat(Msg, Value.Roll);
        UOSCManager::AddFloat(Msg, Value.Yaw);
        UOSCManager::SetOSCMessageAddress(Msg, Sender.CachedFullAddress);
        BroadcastOSC(Msg);
    }
}

void UOSCT_Master::Send_Color(FOSCT_Sender& Sender, const FLinearColor& Value, UObject* Owner)
{
    SetupSender(Sender, EOSCT_ModuleType::COLOR, Owner);
    if (OSCT_Clients.Num() > 0 && !Sender.CachedFullAddress.GetFullPath().IsEmpty())
    {
        FOSCMessage Msg;
        UOSCManager::AddFloat(Msg, Value.R);
        UOSCManager::AddFloat(Msg, Value.G);
        UOSCManager::AddFloat(Msg, Value.B);
        UOSCManager::AddFloat(Msg, Value.A);
        
        UOSCManager::SetOSCMessageAddress(Msg, Sender.CachedFullAddress);
        BroadcastOSC(Msg);
    }
}

void UOSCT_Master::Send_Transform(FOSCT_Sender& Sender, const FTransform& Value, UObject* Owner)
{
    SetupSender(Sender, EOSCT_ModuleType::TRANSFORM, Owner);
    if (OSCT_Clients.Num() > 0 && !Sender.CachedFullAddress.GetFullPath().IsEmpty())
    {
        FOSCMessage Msg;
        UOSCManager::AddFloat(Msg, Value.GetLocation().X);
        UOSCManager::AddFloat(Msg, Value.GetLocation().Y);
        UOSCManager::AddFloat(Msg, Value.GetLocation().Z);
        
        UOSCManager::AddFloat(Msg, Value.GetRotation().X);
        UOSCManager::AddFloat(Msg, Value.GetRotation().Y);
        UOSCManager::AddFloat(Msg, Value.GetRotation().Z);
        
        UOSCManager::AddFloat(Msg, Value.GetScale3D().X);
        UOSCManager::AddFloat(Msg, Value.GetScale3D().Y);
        UOSCManager::AddFloat(Msg, Value.GetScale3D().Z);
        
        UOSCManager::SetOSCMessageAddress(Msg, Sender.CachedFullAddress);
        BroadcastOSC(Msg);
    }

}

void UOSCT_Master::Send_String(FOSCT_Sender& Sender, const FString& Value, UObject* Owner)
{
    SetupSender(Sender, EOSCT_ModuleType::STRING, Owner);
    if (OSCT_Clients.Num() > 0 && !Sender.CachedFullAddress.GetFullPath().IsEmpty())
    {
        FOSCMessage Msg;
        UOSCManager::AddString(Msg, Value);
        UOSCManager::SetOSCMessageAddress(Msg, Sender.CachedFullAddress);
        BroadcastOSC(Msg);
    }
}

UOSCT_Proxy* UOSCT_Master::GetOrCreateProxy(const FOSCT_Receiver& Receiver)
{
    FString FormattedAddress;
    if (!UOSCT_Functions::FormatAddress(Receiver.Role, Receiver.ModuleType, Receiver.Pack, Receiver.Address, FormattedAddress))
    {
        return nullptr;
    }
    FName AddressKey = FName(*FormattedAddress);
    
    if (UOSCT_Proxy** Found = ReceiverProxies.Find(AddressKey))
    {
        return *Found;
    }

    // Create a new lightweight UObject proxy managed by this subsystem
    UOSCT_Proxy* NewProxy = NewObject<UOSCT_Proxy>(this);
    NewProxy->ReceiverConfig = Receiver;
    UE_LOG(OSCToolset, Log, TEXT("Created a new receiver proxy for: %s"), *FormattedAddress);

    ReceiverProxies.Add(AddressKey, NewProxy);
    return NewProxy;
}

UOSCT_Proxy* UOSCT_Master::GetReceiverProxy(const FOSCT_Receiver& Receiver, UObject* Owner)
{
    if (!Owner) return nullptr;
    return GetOrCreateProxy(Receiver);
}

TArray<UOSCT_Proxy*> UOSCT_Master::GetManyReceiverProxies(const TArray<FOSCT_Receiver>& Receivers, UObject* Owner)
{
    TArray<UOSCT_Proxy*> ResultProxies;
    ResultProxies.Reserve(Receivers.Num());

    for (const FOSCT_Receiver& Receiver : Receivers)
    {
        if (UOSCT_Proxy* FoundProxy = GetReceiverProxy(Receiver, Owner))
        {
            ResultProxies.Add(FoundProxy);
        }
    }
    return ResultProxies;
}

TArray<UOSCT_Proxy*> UOSCT_Master::GetReceiverProxiesFromDataTable(UDataTable* InTable, UObject* Owner)
{
    if (!InTable)
    {
        return TArray<UOSCT_Proxy*>();
    }
    TArray<FOSCT_Receiver> ExtractedReceivers;
    // Ensure the table uses our struct (or a row struct containing/convertible to FOSCT_Receiver)
    if (InTable->RowStruct->IsChildOf(FOSCT_ReceiverRow::StaticStruct()))
    {
        static const FString ContextString(TEXT("OSCT Receiver Rows"));
        TArray<FOSCT_ReceiverRow*> Rows;
        InTable->GetAllRows<FOSCT_ReceiverRow>(ContextString, Rows);
        
        ExtractedReceivers.Reserve(Rows.Num());

        for (FOSCT_ReceiverRow* RowData : Rows)
        {
            if (RowData)
            {
                FOSCT_Receiver NewReceiver(*RowData);
                ExtractedReceivers.Add(NewReceiver);
            }
        }
    }
    return GetManyReceiverProxies(ExtractedReceivers, Owner);
}


bool UOSCT_Master::HandleCommands(const FOSCMessage& InMessage)
{
    FOSCAddress Addr = UOSCManager::GetOSCMessageAddress(InMessage);
    FString Path = UOSCManager::GetOSCAddressFullPath(Addr);
    
    // Check if this message is actually a command for us
    FString TargetCmdPath = OSCT_Base_addr + "R/CMD";
    if (!Path.Equals(TargetCmdPath))
    {
        return false; // Not a command message
    }

    // It is a command! Proceed with execution
    APlayerController* PC = GetWorld()->GetFirstPlayerController();
    if (PC)
    {
        FString Cmd = "";
        if (UOSCManager::GetString(InMessage, 0, Cmd))
        {
            UE_LOG(OSCToolset, Log, TEXT("Executing Command: %s"), *Cmd);
            PC->ConsoleCommand(Cmd, true);
        }
    }
    return true; // Message handled
}


void UOSCT_Master::RouteMessage(const FOSCMessage& InMessage, const FString& InAddress, int32 InPort)
{

    FString Address = UOSCManager::GetOSCAddressFullPath(UOSCManager::GetOSCMessageAddress(InMessage));
    FName AddressKey = FName(*Address);
    
    if (const EOSCT_RouteType* RouteTypePtr = AddressToType.Find(AddressKey))
    {
        switch (*RouteTypePtr)
        {
        //SIGNLE VALUES
        case EOSCT_RouteType::FLOAT:
            RouteOSCMessage<FOSCT_FloatLink, float>(InMessage, FloatLinks, AddressToType, AddressKey,
            &UOSCT_Parsing::TryGetFloat,
            &IOSCT_Router::Execute_GET_Float,
            &IOSCT_Router::Execute_GET_Float_Tick,
            [](UOSCT_Proxy* Proxy, const float& Val) 
            {Proxy->OnGET_Float.Broadcast(Proxy->ReceiverConfig, Val);}
            );
            break;
        case EOSCT_RouteType::EVENT:
            RouteOSCMessage<FOSCT_EventLink, bool>(InMessage, EventLinks, AddressToType, AddressKey,
            [](const FOSCMessage& M, bool& Out) { Out = true; return true; },
            [](UObject* Obj, const FOSCT_Receiver& D, bool V) { IOSCT_Router::Execute_GET_Event(Obj, D); },
            nullptr,
            [](UOSCT_Proxy* Proxy, bool DummyVal) 
            {Proxy->OnGET_Event.Broadcast(Proxy->ReceiverConfig);}
            );
            break;
        case EOSCT_RouteType::BOOL:
            RouteOSCMessage<FOSCT_BoolLink, bool>(InMessage, BoolLinks, AddressToType, AddressKey,
            &UOSCT_Parsing::TryGetBool,
            &IOSCT_Router::Execute_GET_Boolean,
            nullptr,
            [](UOSCT_Proxy* Proxy, const bool& Val)
            {Proxy->OnGET_Boolean.Broadcast(Proxy->ReceiverConfig, Val);}
            );
            break;
        case EOSCT_RouteType::INT:
            RouteOSCMessage<FOSCT_IntegerLink, int32>(InMessage, IntegerLinks, AddressToType, AddressKey,
            &UOSCT_Parsing::TryGetInteger,
            &IOSCT_Router::Execute_GET_Integer,
            nullptr,
            [](UOSCT_Proxy* Proxy, const int32& Val)
            {Proxy->OnGET_Integer.Broadcast(Proxy->ReceiverConfig, Val);}
            );
            break;
        case EOSCT_RouteType::VEC2:
            RouteOSCMessage<FOSCT_Vector2Link, FVector2D>(InMessage, Vec2Links, AddressToType, AddressKey,
            &UOSCT_Parsing::TryGetVector2,
            &IOSCT_Router::Execute_GET_Vector2,
            &IOSCT_Router::Execute_GET_Vector2_Tick,
            [](UOSCT_Proxy* Proxy, const FVector2D& Val)
            {Proxy->OnGET_Vector2.Broadcast(Proxy->ReceiverConfig, Val);}
            );
            break;
        case EOSCT_RouteType::VEC3:
            RouteOSCMessage<FOSCT_Vector3Link, FVector>(InMessage, Vec3Links, AddressToType, AddressKey,
            &UOSCT_Parsing::TryGetVector3,
            &IOSCT_Router::Execute_GET_Vector3,
            &IOSCT_Router::Execute_GET_Vector3_Tick,
            [](UOSCT_Proxy* Proxy, const FVector& Val)
            {Proxy->OnGET_Vector3.Broadcast(Proxy->ReceiverConfig, Val);}
            );
            break;
        case EOSCT_RouteType::ROTATION:
            RouteOSCMessage<FOSCT_RotationLink, FRotator>(InMessage, RotationLinks, AddressToType, AddressKey,
            &UOSCT_Parsing::TryGetRotation,
            &IOSCT_Router::Execute_GET_Rotation,
            &IOSCT_Router::Execute_GET_Rotation_Tick,
            [](UOSCT_Proxy* Proxy, const FRotator& Val) 
            {Proxy->OnGET_Rotation.Broadcast(Proxy->ReceiverConfig, Val);}
            );
            break;
        case EOSCT_RouteType::COLOR:
            RouteOSCMessage<FOSCT_ColorLink, FLinearColor>(InMessage, ColorLinks, AddressToType, AddressKey,
            &UOSCT_Parsing::TryGetColor,
            &IOSCT_Router::Execute_GET_Color,
            &IOSCT_Router::Execute_GET_Color_Tick,
            [](UOSCT_Proxy* Proxy, const FLinearColor& Val) 
            {Proxy->OnGET_Color.Broadcast(Proxy->ReceiverConfig, Val);}
            );
            break;
        case EOSCT_RouteType::TRANSFORM:
            RouteOSCMessage<FOSCT_TransformLink, FTransform>(InMessage, TransformLinks, AddressToType, AddressKey,
            &UOSCT_Parsing::TryGetTransform,
            &IOSCT_Router::Execute_GET_Transform,
            &IOSCT_Router::Execute_GET_Transform_Tick,
            [](UOSCT_Proxy* Proxy, const FTransform& Val) 
            {Proxy->OnGET_Transform.Broadcast(Proxy->ReceiverConfig, Val);}
            );
            break;
        case EOSCT_RouteType::STRING:
            RouteOSCMessage<FOSCT_StringLink, FString>(InMessage, StringLinks, AddressToType, AddressKey,
            &UOSCT_Parsing::TryGetString,
            &IOSCT_Router::Execute_GET_String,
            nullptr,
            [](UOSCT_Proxy* Proxy, const FString& Val) 
            {Proxy->OnGET_String.Broadcast(Proxy->ReceiverConfig, Val);}
            );
            break;
        case EOSCT_RouteType::NOTE:
            RouteOSCMessage<FOSCT_NoteLink, FOSCT_Note>(InMessage, NoteLinks, AddressToType, AddressKey,
            &UOSCT_Parsing::TryGetNotes,
            &IOSCT_Router::Execute_GET_Notes,
            nullptr,
            [](UOSCT_Proxy* Proxy, const FOSCT_Note& Val) 
            {Proxy->OnGET_Notes.Broadcast(Proxy->ReceiverConfig, Val);}
            );
            break;
        //PACKS
        case EOSCT_RouteType::FLOAT_PACK:
            RouteOSCMessage<FOSCT_FloatPackLink, TMap<FString, float>>(InMessage, FloatPackLinks, AddressToType, AddressKey,
            &UOSCT_Parsing::TryGetFloatPack,
            &IOSCT_Router::Execute_GET_Float_Pack,
            &IOSCT_Router::Execute_GET_Float_Pack_Tick,
            [](UOSCT_Proxy* Proxy, const TMap<FString, float>& Val) {
                FOSCT_FloatMapWrapper Wrapper;
                Wrapper.Data = Val;
                Proxy->OnGET_Float_Pack.Broadcast(Proxy->ReceiverConfig, Wrapper);
            });
            break;
        case EOSCT_RouteType::EVENT_PACK:
            RouteOSCMessage<FOSCT_EventPackLink, TMap<FString, bool>>(InMessage, EventPackLinks, AddressToType, AddressKey,
            &UOSCT_Parsing::TryGetEventPack,
            &IOSCT_Router::Execute_GET_Event_Pack,
            nullptr,
            [](UOSCT_Proxy* Proxy, const TMap<FString, bool>& Val) {
                FOSCT_BoolMapWrapper Wrapper;
                Wrapper.Data = Val;
                // Assuming you have an OnGET_Event_Pack delegate declared on your proxy
                Proxy->OnGET_Boolean_Pack.Broadcast(Proxy->ReceiverConfig, Wrapper);
            });
            break;
        case EOSCT_RouteType::BOOL_PACK:
            RouteOSCMessage<FOSCT_BoolPackLink, TMap<FString, bool>>(InMessage, BoolPackLinks, AddressToType, AddressKey,
            &UOSCT_Parsing::TryGetEventPack,
            &IOSCT_Router::Execute_GET_Boolean_Pack,
            nullptr,
            [](UOSCT_Proxy* Proxy, const TMap<FString, bool>& Val) {
                FOSCT_BoolMapWrapper Wrapper;
                Wrapper.Data = Val;
                Proxy->OnGET_Boolean_Pack.Broadcast(Proxy->ReceiverConfig, Wrapper);
            });
            break;
        case EOSCT_RouteType::INT_PACK:
            RouteOSCMessage<FOSCT_IntegerPackLink, TMap<FString, int32>>(InMessage, IntegerPackLinks, AddressToType, AddressKey,
            &UOSCT_Parsing::TryGetIntegerPack,
            &IOSCT_Router::Execute_GET_Integer_Pack,
            nullptr,
            [](UOSCT_Proxy* Proxy, const TMap<FString, int32>& Val) {
                FOSCT_IntMapWrapper Wrapper;
                Wrapper.Data = Val;
                Proxy->OnGET_Integer_Pack.Broadcast(Proxy->ReceiverConfig, Wrapper);
            });
            break;
        case EOSCT_RouteType::VEC2_PACK:
            RouteOSCMessage<FOSCT_Vector2PackLink, TMap<FString, FVector2D>>(InMessage, Vec2PackLinks, AddressToType, AddressKey,
            &UOSCT_Parsing::TryGetVector2Pack,
            &IOSCT_Router::Execute_GET_Vector2_Pack,
            &IOSCT_Router::Execute_GET_Vector2_Pack_Tick,
            [](UOSCT_Proxy* Proxy, const TMap<FString, FVector2D>& Val) {
                FOSCT_Vector2MapWrapper Wrapper;
                Wrapper.Data = Val;
                Proxy->OnGET_Vector2_Pack.Broadcast(Proxy->ReceiverConfig, Wrapper);
            });
            break;
        case EOSCT_RouteType::VEC3_PACK:
            RouteOSCMessage<FOSCT_Vector3PackLink, TMap<FString, FVector>>(InMessage, Vec3PackLinks, AddressToType, AddressKey,
            &UOSCT_Parsing::TryGetVector3Pack,
            &IOSCT_Router::Execute_GET_Vector3_Pack,
            &IOSCT_Router::Execute_GET_Vector3_Pack_Tick,
            [](UOSCT_Proxy* Proxy, const TMap<FString, FVector>& Val) {
                FOSCT_Vector3MapWrapper Wrapper;
                Wrapper.Data = Val;
                Proxy->OnGET_Vector3_Pack.Broadcast(Proxy->ReceiverConfig, Wrapper);
            });
            break;
        case EOSCT_RouteType::ROTATION_PACK:
            RouteOSCMessage<FOSCT_RotationPackLink, TMap<FString, FRotator>>(InMessage, RotationPackLinks, AddressToType, AddressKey,
            &UOSCT_Parsing::TryGetRotationPack,
            &IOSCT_Router::Execute_GET_Rotation_Pack,
            &IOSCT_Router::Execute_GET_Rotation_Pack_Tick,
            [](UOSCT_Proxy* Proxy, const TMap<FString, FRotator>& Val) {
                FOSCT_RotationMapWrapper Wrapper;
                Wrapper.Data = Val;
                Proxy->OnGET_Rotation_Pack.Broadcast(Proxy->ReceiverConfig, Wrapper);
            });
            break;
        case EOSCT_RouteType::COLOR_PACK:
            RouteOSCMessage<FOSCT_ColorPackLink, TMap<FString, FLinearColor>>(InMessage, ColorPackLinks, AddressToType, AddressKey,
            &UOSCT_Parsing::TryGetColorPack,
            &IOSCT_Router::Execute_GET_Color_Pack,
            &IOSCT_Router::Execute_GET_Color_Pack_Tick,
            [](UOSCT_Proxy* Proxy, const TMap<FString, FLinearColor>& Val) {
                FOSCT_ColorMapWrapper Wrapper;
                Wrapper.Data = Val;
                Proxy->OnGET_Color_Pack.Broadcast(Proxy->ReceiverConfig, Wrapper);
            });
            break;
        case EOSCT_RouteType::TRANSFORM_PACK:
            RouteOSCMessage<FOSCT_TransformPackLink, TMap<FString, FTransform>>(InMessage, TransformPackLinks, AddressToType, AddressKey,
            &UOSCT_Parsing::TryGetTransformPack,
            &IOSCT_Router::Execute_GET_Transform_Pack,
            &IOSCT_Router::Execute_GET_Transform_Pack_Tick,
            [](UOSCT_Proxy* Proxy, const TMap<FString, FTransform>& Val) {
                FOSCT_TransformMapWrapper Wrapper;
                Wrapper.Data = Val;
                Proxy->OnGET_Transform_Pack.Broadcast(Proxy->ReceiverConfig, Wrapper);
            });
            break;          
        case EOSCT_RouteType::STRING_PACK:
            RouteOSCMessage<FOSCT_StringPackLink, TArray<FString>>(InMessage, StringPackLinks, AddressToType, AddressKey,
            &UOSCT_Parsing::TryGetStringPack,
            &IOSCT_Router::Execute_GET_String_Pack,
            nullptr,
            [](UOSCT_Proxy* Proxy, const TArray<FString>& Val) {
                Proxy->OnGET_String_Pack.Broadcast(Proxy->ReceiverConfig, Val);
            });
            break;                    
        default:
            UE_LOG(OSCToolset, Warning, TEXT("Received a message not well formatted from a Receiver: %s"), *Address);
            break;
        }
    }
    // Try to handle as a command last
    HandleCommands(InMessage);
}

void UOSCT_Master::Tick(float DeltaTime)
{
    // Important: Only tick if the game is actually running/unpaused
    UWorld* World = GetWorld();
    if (!World || World->IsPaused()) return;
    if (TickableAddresses.Num() == 0) return; //If no active addresses.
    for (auto It = TickableAddresses.CreateIterator(); It; ++It)
    {
        FName Address = *It;
        bool bAnyLinkStillMoving = false;
        
        // Float
        ProcessActiveLinksTick<FOSCT_FloatLink>(FloatLinks, Address, DeltaTime, bAnyLinkStillMoving, 
            [](UObject* Obj, const FOSCT_FloatLink& Link, const float& Val) {
                IOSCT_Router::Execute_GET_Float_Tick(Obj, Link.Data, Val);
            },
            [](UOSCT_Proxy* Proxy, float Val) {
                Proxy->OnGET_Float_Tick.Broadcast(Proxy->ReceiverConfig, Val);
            }
        );
        ProcessActiveLinksTick<FOSCT_FloatPackLink>(FloatPackLinks, Address, DeltaTime, bAnyLinkStillMoving, 
            [](UObject* Obj, const FOSCT_FloatPackLink& Link, const TMap < FString, float >& Val) {
                IOSCT_Router::Execute_GET_Float_Pack_Tick(Obj, Link.Data, Val);
            },
            [](UOSCT_Proxy* Proxy, const TMap<FString, float>& Val) {
                FOSCT_FloatMapWrapper Wrapper;
                Wrapper.Data = Val;
                Proxy->OnGET_Float_Pack_Tick.Broadcast(Proxy->ReceiverConfig, Wrapper);
            }
        );
        // Vector 2D
        ProcessActiveLinksTick<FOSCT_Vector2Link>(Vec2Links, Address, DeltaTime, bAnyLinkStillMoving, 
            [](UObject* Obj, const FOSCT_Vector2Link& Link, const FVector2D& Val) {
                IOSCT_Router::Execute_GET_Vector2_Tick(Obj, Link.Data, Val);
            },
            [](UOSCT_Proxy* Proxy, FVector2D Val) {
                Proxy->OnGET_Vector2_Tick.Broadcast(Proxy->ReceiverConfig, Val);
            }
        );

        ProcessActiveLinksTick<FOSCT_Vector2PackLink>(Vec2PackLinks, Address, DeltaTime, bAnyLinkStillMoving, 
            [](UObject* Obj, const FOSCT_Vector2PackLink& Link, const TMap<FString, FVector2D>& Val) {
                IOSCT_Router::Execute_GET_Vector2_Pack_Tick(Obj, Link.Data, Val);
            },
            [](UOSCT_Proxy* Proxy, const TMap<FString, FVector2D>& Val) {
                FOSCT_Vector2MapWrapper Wrapper;
                Wrapper.Data = Val;
                Proxy->OnGET_Vector2_Pack_Tick.Broadcast(Proxy->ReceiverConfig, Wrapper);
            }
        );
        // Vector 3D
        ProcessActiveLinksTick<FOSCT_Vector3Link>(Vec3Links, Address, DeltaTime, bAnyLinkStillMoving, 
            [](UObject* Obj, const FOSCT_Vector3Link& Link, const FVector& Val) {
                IOSCT_Router::Execute_GET_Vector3_Tick(Obj, Link.Data, Val);
            },
            [](UOSCT_Proxy* Proxy, FVector Val) {
                Proxy->OnGET_Vector3_Tick.Broadcast(Proxy->ReceiverConfig, Val);
            }
        );

        ProcessActiveLinksTick<FOSCT_Vector3PackLink>(Vec3PackLinks, Address, DeltaTime, bAnyLinkStillMoving, 
            [](UObject* Obj, const FOSCT_Vector3PackLink& Link, const TMap<FString, FVector>& Val) {
                IOSCT_Router::Execute_GET_Vector3_Pack_Tick(Obj, Link.Data, Val);
            },
            [](UOSCT_Proxy* Proxy, const TMap<FString, FVector>& Val) {
                FOSCT_Vector3MapWrapper Wrapper;
                Wrapper.Data = Val;
                Proxy->OnGET_Vector3_Pack_Tick.Broadcast(Proxy->ReceiverConfig, Wrapper);
            }
        );
        // Rotation
        ProcessActiveLinksTick<FOSCT_RotationLink>(RotationLinks, Address, DeltaTime, bAnyLinkStillMoving, 
            [](UObject* Obj, const FOSCT_RotationLink& Link, const FRotator& Val) {
                IOSCT_Router::Execute_GET_Rotation_Tick(Obj, Link.Data, Val);
            },
            [](UOSCT_Proxy* Proxy, FRotator Val) {
                Proxy->OnGET_Rotation_Tick.Broadcast(Proxy->ReceiverConfig, Val);
            }
        );

        ProcessActiveLinksTick<FOSCT_RotationPackLink>(RotationPackLinks, Address, DeltaTime, bAnyLinkStillMoving, 
            [](UObject* Obj, const FOSCT_RotationPackLink& Link, const TMap<FString, FRotator>& Val) {
                IOSCT_Router::Execute_GET_Rotation_Pack_Tick(Obj, Link.Data, Val);
            },
            [](UOSCT_Proxy* Proxy, const TMap<FString, FRotator>& Val) {
                FOSCT_RotationMapWrapper Wrapper;
                Wrapper.Data = Val;
                Proxy->OnGET_Rotation_Pack_Tick.Broadcast(Proxy->ReceiverConfig, Wrapper);
            }
        );
        // Color
        ProcessActiveLinksTick<FOSCT_ColorLink>(ColorLinks, Address, DeltaTime, bAnyLinkStillMoving, 
            [](UObject* Obj, const FOSCT_ColorLink& Link, const FLinearColor& Val) {
                IOSCT_Router::Execute_GET_Color_Tick(Obj, Link.Data, Val);
            },
            [](UOSCT_Proxy* Proxy, FLinearColor Val) {
                Proxy->OnGET_Color_Tick.Broadcast(Proxy->ReceiverConfig, Val);
            }
        );

        ProcessActiveLinksTick<FOSCT_ColorPackLink>(ColorPackLinks, Address, DeltaTime, bAnyLinkStillMoving, 
            [](UObject* Obj, const FOSCT_ColorPackLink& Link, const TMap<FString, FLinearColor>& Val) {
                IOSCT_Router::Execute_GET_Color_Pack_Tick(Obj, Link.Data, Val);
            },
            [](UOSCT_Proxy* Proxy, const TMap<FString, FLinearColor>& Val) {
                FOSCT_ColorMapWrapper Wrapper;
                Wrapper.Data = Val;
                Proxy->OnGET_Color_Pack_Tick.Broadcast(Proxy->ReceiverConfig, Wrapper);
            }
        );
        // Transform
        ProcessActiveLinksTick<FOSCT_TransformLink>(TransformLinks, Address, DeltaTime, bAnyLinkStillMoving, 
            [](UObject* Obj, const FOSCT_TransformLink& Link, const FTransform& Val) {
                IOSCT_Router::Execute_GET_Transform_Tick(Obj, Link.Data, Val);
            },
            [](UOSCT_Proxy* Proxy, const FTransform& Val) {
                Proxy->OnGET_Transform_Tick.Broadcast(Proxy->ReceiverConfig, Val);
            }
        );

        ProcessActiveLinksTick<FOSCT_TransformPackLink>(TransformPackLinks, Address, DeltaTime, bAnyLinkStillMoving, 
            [](UObject* Obj, const FOSCT_TransformPackLink& Link, const TMap<FString, FTransform>& Val) {
                IOSCT_Router::Execute_GET_Transform_Pack_Tick(Obj, Link.Data, Val);
            },
            [](UOSCT_Proxy* Proxy, const TMap<FString, FTransform>& Val) {
                FOSCT_TransformMapWrapper Wrapper;
                Wrapper.Data = Val;
                Proxy->OnGET_Transform_Pack_Tick.Broadcast(Proxy->ReceiverConfig, Wrapper);
            }
        );
        if (!bAnyLinkStillMoving) It.RemoveCurrent();
    }
}