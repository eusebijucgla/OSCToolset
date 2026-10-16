// Fill out your copyright notice in the Description page of Project Settings.
#include "Functions/OSCT_Manager.h"
#include "Engine/GameInstance.h"
#include "OSCT_Master.h"

UOSCT_Master* UOSCT_Manager::GetMaster(const UObject* WorldContextObject)
{
    if (!WorldContextObject) return nullptr;

    UWorld* World = WorldContextObject->GetWorld();
    if (!World) return nullptr;

    UGameInstance* GI = World->GetGameInstance();
    if (!GI) return nullptr;

    return GI->GetSubsystem<UOSCT_Master>();
}

void UOSCT_Manager::AddReceiver(UObject* WorldContextObject, FOSCT_Receiver Receiver, UObject* Owner)
{
	if (UOSCT_Master* Master = GetMaster(WorldContextObject))
	{
		Master->AddReceiver(Receiver, Owner);
	}
}

void UOSCT_Manager::AddManyReceivers(UObject* WorldContextObject, TArray<FOSCT_Receiver> Receivers, UObject* Owner)
{
	if (UOSCT_Master* Master = GetMaster(WorldContextObject))
	{
		Master->AddManyReceivers(Receivers, Owner);
	}
}

void UOSCT_Manager::AddReceiversFromDataTable(UObject* WorldContextObject, UDataTable* InTable, UObject* Owner)
{
	if (UOSCT_Master* Master = GetMaster(WorldContextObject))
	{
		Master->AddReceiversFromDataTable(InTable, Owner);
	}
}

void UOSCT_Manager::RemoveReceiver(UObject* WorldContextObject, FOSCT_Receiver Receiver, UObject* Owner)
{
	if (UOSCT_Master* Master = GetMaster(WorldContextObject))
	{
		Master->RemoveReceiver(Receiver, Owner);
	}

}

void UOSCT_Manager::RemoveAllReceivers(UObject* WorldContextObject)
{
	if (UOSCT_Master* Master = GetMaster(WorldContextObject))
	{
		Master->RemoveAllReceivers();
	}
}

UOSCT_Proxy* UOSCT_Manager::GetReceiverProxy(UObject* WorldContextObject, FOSCT_Receiver Receiver, UObject* Owner)
{
	if (UOSCT_Master* Master = GetMaster(WorldContextObject))
	{
		return Master->GetReceiverProxy(Receiver, Owner);
	}
	return nullptr;
}

TArray<UOSCT_Proxy*> UOSCT_Manager::GetManyReceiverProxies(UObject* WorldContextObject,	TArray<FOSCT_Receiver> Receivers, UObject* Owner)
{
	if (UOSCT_Master* Master = GetMaster(WorldContextObject))
	{
		return Master->GetManyReceiverProxies(Receivers, Owner);
	}
	return TArray<UOSCT_Proxy*>();
}

TArray<UOSCT_Proxy*> UOSCT_Manager::GetReceiverProxiesFromDataTable(UObject* WorldContextObject,
	UDataTable* InTable, UObject* Owner)
{
	if (UOSCT_Master* Master = GetMaster(WorldContextObject))
	{
		return Master->GetReceiverProxiesFromDataTable(InTable, Owner);
	}
	return TArray<UOSCT_Proxy*>();

}

FString UOSCT_Manager::GetReceiverAddress(const FOSCT_Receiver& Receiver)
{
	return Receiver.Address;
}

void UOSCT_Manager::SendEvent(UObject* WorldContextObject, FOSCT_Sender& Sender, UObject* Owner)
{
	if (UOSCT_Master* Master = GetMaster(WorldContextObject))
	{
		Master->Send_Event(Sender, Owner);
	}
	
}

void UOSCT_Manager::SendBool(UObject* WorldContextObject, FOSCT_Sender& Sender, const bool Value, UObject* Owner)
{
	if (UOSCT_Master* Master = GetMaster(WorldContextObject))
	{
		Master->Send_Bool(Sender, Value, Owner);
	}
}

void UOSCT_Manager::SendFloat(UObject* WorldContextObject, FOSCT_Sender& Sender, const float Value, UObject* Owner)
{
	if (UOSCT_Master* Master = GetMaster(WorldContextObject))
	{
		Master->Send_Float(Sender, Value, Owner);
	}
}

void UOSCT_Manager::SendInteger(UObject* WorldContextObject, FOSCT_Sender& Sender, const int32 Value, UObject* Owner)
{
	if (UOSCT_Master* Master = GetMaster(WorldContextObject))
	{
		Master->Send_Integer(Sender, Value, Owner);
	}
}

void UOSCT_Manager::SendVector2(UObject* WorldContextObject, FOSCT_Sender& Sender, const FVector2D Value, UObject* Owner)
{
	if (UOSCT_Master* Master = GetMaster(WorldContextObject))
	{
		Master->Send_Vector2(Sender, Value, Owner);
	}
}

void UOSCT_Manager::SendVector3(UObject* WorldContextObject, FOSCT_Sender& Sender, const FVector Value, UObject* Owner)
{
	if (UOSCT_Master* Master = GetMaster(WorldContextObject))
	{
		Master->Send_Vector3(Sender, Value, Owner);
	}
}

void UOSCT_Manager::SendRotation(UObject* WorldContextObject, FOSCT_Sender& Sender, const FRotator Value,
	UObject* Owner)
{
	if (UOSCT_Master* Master = GetMaster(WorldContextObject))
	{
		Master->Send_Rotation(Sender, Value, Owner);
	}
}

void UOSCT_Manager::SendColor(UObject* WorldContextObject, FOSCT_Sender& Sender, const FLinearColor Value,
	UObject* Owner)
{
	if (UOSCT_Master* Master = GetMaster(WorldContextObject))
	{
		Master->Send_Color(Sender, Value, Owner);
	}
}

void UOSCT_Manager::SendTransform(UObject* WorldContextObject, FOSCT_Sender& Sender, const FTransform Value,
	UObject* Owner)
{
	if (UOSCT_Master* Master = GetMaster(WorldContextObject))
	{
		Master->Send_Transform(Sender, Value, Owner);
	}
}

void UOSCT_Manager::SendString(UObject* WorldContextObject, FOSCT_Sender& Sender, const FString& Value, UObject* Owner)
{
	if (UOSCT_Master* Master = GetMaster(WorldContextObject))
	{
		Master->Send_String(Sender, Value, Owner);
	}
}
