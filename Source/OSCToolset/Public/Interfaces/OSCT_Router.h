// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OSCT_ETypes.h"
#include "OSCT_Router.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UOSCT_Router : public UInterface
{
	GENERATED_BODY()
};

class OSCTOOLSET_API IOSCT_Router
{
	GENERATED_BODY()
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	//Message received main event
	UFUNCTION(BlueprintNativeEvent, Category = "OSCToolset|Base")
	void OnOSCMessageReceived(const FString& Address, const FOSCMessage& Message);
	
	//OSCTModules lifecycle
	UFUNCTION(BlueprintNativeEvent, Category = "OSCToolset|Base")
	void OnReceiverAdded(const FOSCT_Receiver& Receiver);
	
	UFUNCTION(BlueprintNativeEvent, Category = "OSCToolset|Base")
	void OnReceiverInit(const FOSCT_Receiver& Receiver, const int32& MessageSize);
	
	UFUNCTION(BlueprintNativeEvent, Category = "OSCToolset|Base")
	void OnReceiverRemoved(const FOSCT_Receiver& Receiver);
	
	UFUNCTION(BlueprintNativeEvent, Category = "OSCToolset|Base")
	void OnOSCToolsetInitialized();
	
	
	//Receivers
	
	// Event
	UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Receivers|Event")
	void GET_Event(const FOSCT_Receiver& Receiver);
	UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Receivers|Event|Pack")
	void GET_Event_Pack(const FOSCT_Receiver& Receiver, const TMap<FString, bool>& Map);
	
	// Bool
	UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Receivers|Boolean")
	void GET_Boolean(const FOSCT_Receiver& Receiver, const bool Value);
	UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Receivers|Boolean|Pack")
	void GET_Boolean_Pack(const FOSCT_Receiver& Receiver, const TMap<FString, bool>& Map);

	// Integer
	UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Receivers|Integer")
	void GET_Integer(const FOSCT_Receiver& Receiver, const int32& Value);
	UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Receivers|Integer|Pack")
	void GET_Integer_Pack(const FOSCT_Receiver& Receiver, const TMap<FString, int32>& Map);

	// Float
	UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Receivers|Float")
	void GET_Float(const FOSCT_Receiver& Receiver, const float& Value);
	UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Receivers|Float")
	void GET_Float_Tick(const FOSCT_Receiver& Receiver, const float& Value);
	
	UFUNCTION(BlueprintNativeEvent, Category = "OSCToolset|Receivers|Float|Pack")
	void GET_Float_Pack(const FOSCT_Receiver& Receiver, const TMap<FString, float>& Map);
	UFUNCTION(BlueprintNativeEvent, Category = "OSCToolset|Receivers|Float|Pack")
	void GET_Float_Pack_Tick(const FOSCT_Receiver& Receiver, const TMap<FString, float>& Map);

	//Vector 2
	UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Receivers|Vector2")
	void GET_Vector2(const FOSCT_Receiver& Receiver, const FVector2D& Value);
	UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Receivers|Vector2")
	void GET_Vector2_Tick(const FOSCT_Receiver& Receiver, const FVector2D& Value);
	
	UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Receivers|Vector2|Pack")
	void GET_Vector2_Pack(const FOSCT_Receiver& Receiver, const TMap<FString, FVector2D>& Map);
	UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Receivers|Vector2|Pack")
	void GET_Vector2_Pack_Tick(const FOSCT_Receiver& Receiver, const TMap<FString, FVector2D>& Map);

	//Vector 3
	UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Receivers|Vector3")
	void GET_Vector3(const FOSCT_Receiver& Receiver, const FVector& Value);
	UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Receivers|Vector3")
	void GET_Vector3_Tick(const FOSCT_Receiver& Receiver, const FVector& Value);
	
	UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Receivers|Vector3|Pack")
	void GET_Vector3_Pack(const FOSCT_Receiver& Receiver, const TMap<FString, FVector>& Map);
	UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Receivers|Vector3|Pack")
	void GET_Vector3_Pack_Tick(const FOSCT_Receiver& Receiver, const TMap<FString, FVector>& Map);

	//Rotation
	UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Receivers|Rotation")
	void GET_Rotation(const FOSCT_Receiver& Receiver, const FRotator& Value);
	UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Receivers|Rotation")
	void GET_Rotation_Tick(const FOSCT_Receiver& Receiver, const FRotator& Value);
	
	UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Receivers|Rotation|Pack")
	void GET_Rotation_Pack(const FOSCT_Receiver& Receiver, const TMap<FString, FRotator>& Map);
	UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Receivers|Rotation|Pack")
	void GET_Rotation_Pack_Tick(const FOSCT_Receiver& Receiver, const TMap<FString, FRotator>& Map);

	//Color
	UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Receivers|Color")
	void GET_Color(const FOSCT_Receiver& Receiver, const FLinearColor& Value);
	UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Receivers|Color")
	void GET_Color_Tick(const FOSCT_Receiver& Receiver, const FLinearColor& Value);

	UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Receivers|Color|Pack")
	void GET_Color_Pack(const FOSCT_Receiver& Receiver, const TMap<FString, FLinearColor>& Map);
	UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Receivers|Color|Pack")
	void GET_Color_Pack_Tick(const FOSCT_Receiver& Receiver, const TMap<FString, FLinearColor>& Map);

	//Transform
	UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Receivers|Transform")
	void GET_Transform(const FOSCT_Receiver& Receiver, const FTransform& Value);
	UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Receivers|Transform")
	void GET_Transform_Tick(const FOSCT_Receiver& Receiver, const FTransform& Value);

	
	UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Receivers|Transform|Pack")
	void GET_Transform_Pack(const FOSCT_Receiver& Receiver, const TMap<FString, FTransform>& Map);
	UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Receivers|Transform|Pack")
	void GET_Transform_Pack_Tick(const FOSCT_Receiver& Receiver, const TMap<FString, FTransform>& Map);

	//String
	UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Receivers|String")
	void GET_String(const FOSCT_Receiver& Receiver, const FString& Value);
	UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Receivers|String|Pack")
	void GET_String_Pack(const FOSCT_Receiver& Receiver, const TArray<FString>& Map);
	
	//Notes
	UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Receivers|Note")
	void GET_Notes(const FOSCT_Receiver& Receiver, const FOSCT_Note& Value);
	
	
	// //Senders
	// //In case you want to combine values and connecting actors
	// UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Senders")
	// void SENT_Event(const FOSCT_Receiver& Module);
	//
	// UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Senders")
	// void SENT_Float(const FOSCT_Receiver& Module, const float Value);
	//
	// UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Senders")
	// void SENT_Vector2(const FOSCT_Receiver& Module, const FVector2D& Value);
	//
	// UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Senders")
	// void SENT_Vector3(const FOSCT_Receiver& Module, const FVector& Value);
	//
	// UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Senders")
	// void SENT_Rotator(const FOSCT_Receiver& Module, const FRotator& Value);
	//
	// UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Senders")
	// void SENT_Color(const FOSCT_Receiver& Module, const FLinearColor& Value);
	//
	// UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Senders")
	// void SENT_Transform(const FOSCT_Receiver& Module, const FTransform& Value);
	//
	// UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Senders")
	// void SENT_String(const FOSCT_Receiver& Module, const FString& Value);
	//
	// UFUNCTION(BlueprintNativeEvent, Category="OSCToolset|Senders")
	// void SENT_Notes(const FOSCT_Receiver& Module, const FOSCT_Note& Value);

};
