#pragma once

#include "CoreMinimal.h"
#include "OSCT_Modules.h"
#include "OSCT_Proxy.generated.h"

// Base Signatures
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnOSCEvent, const FOSCT_Receiver&, Receiver);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOSCBoolEvent, const FOSCT_Receiver&, Receiver, bool, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOSCIntEvent, const FOSCT_Receiver&, Receiver, int32, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOSCFloatEvent, const FOSCT_Receiver&, Receiver, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOSCVector2Event, const FOSCT_Receiver&, Receiver, FVector2D, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOSCVector3Event, const FOSCT_Receiver&, Receiver, FVector, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOSCRotationEvent, const FOSCT_Receiver&, Receiver, FRotator, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOSCColorEvent, const FOSCT_Receiver&, Receiver, FLinearColor, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOSCTransformEvent, const FOSCT_Receiver&, Receiver, FTransform, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOSCStringEvent, const FOSCT_Receiver&, Receiver, const FString&, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOSCNoteEvent, const FOSCT_Receiver&, Receiver, const FOSCT_Note&, Value);

// Map/Pack Signatures
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOSCBoolMapEvent, const FOSCT_Receiver&, Receiver, const FOSCT_BoolMapWrapper&, Pack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOSCIntMapEvent, const FOSCT_Receiver&, Receiver, const FOSCT_IntMapWrapper&, Pack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOSCFloatMapEvent, const FOSCT_Receiver&, Receiver, const FOSCT_FloatMapWrapper&, Pack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOSCVector2MapEvent, const FOSCT_Receiver&, Receiver, const FOSCT_Vector2MapWrapper&, Pack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOSCVector3MapEvent, const FOSCT_Receiver&, Receiver, const FOSCT_Vector3MapWrapper&, Pack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOSCRotationMapEvent, const FOSCT_Receiver&, Receiver, const FOSCT_RotationMapWrapper&, Pack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOSCColorMapEvent, const FOSCT_Receiver&, Receiver, const FOSCT_ColorMapWrapper&, Pack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOSCTransformMapEvent, const FOSCT_Receiver&, Receiver, const FOSCT_TransformMapWrapper&, Pack);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnOSCStringArrayEvent, const FOSCT_Receiver&, Receiver, const TArray<FString>&, Pack);

UCLASS(BlueprintType)
class OSCTOOLSET_API UOSCT_Proxy : public UObject
{
	GENERATED_BODY()

public:
	FOSCT_Receiver ReceiverConfig;
	
	// --- Events ---
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset|Receivers|Event")
    FOnOSCEvent OnGET_Event;

    // --- Boolean ---
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset|Receivers|Boolean")
    FOnOSCBoolEvent OnGET_Boolean;
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset|Receivers|Boolean|Pack")
    FOnOSCBoolMapEvent OnGET_Boolean_Pack;

    // --- Integer ---
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset|Receivers|Integer")
    FOnOSCIntEvent OnGET_Integer;
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset|Receivers|Integer|Pack")
    FOnOSCIntMapEvent OnGET_Integer_Pack;

    // --- Floats ---
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset|Receivers|Float")
    FOnOSCFloatEvent OnGET_Float;
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset|Receivers|Float")
    FOnOSCFloatEvent OnGET_Float_Tick;
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset|Receivers|Float|Pack")
    FOnOSCFloatMapEvent OnGET_Float_Pack;
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset|Receivers|Float|Pack")
    FOnOSCFloatMapEvent OnGET_Float_Pack_Tick;

    // --- Vector 2D ---
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset|Receivers|Vector2")
    FOnOSCVector2Event OnGET_Vector2;
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset|Receivers|Vector2")
    FOnOSCVector2Event OnGET_Vector2_Tick;
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset|Receivers|Vector2|Pack")
    FOnOSCVector2MapEvent OnGET_Vector2_Pack;
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset|Receivers|Vector2|Pack")
    FOnOSCVector2MapEvent OnGET_Vector2_Pack_Tick;

    // --- Vector 3D ---
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset|Receivers|Vector3")
    FOnOSCVector3Event OnGET_Vector3;
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset|Receivers|Vector3")
    FOnOSCVector3Event OnGET_Vector3_Tick;
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset|Receivers|Vector3|Pack")
    FOnOSCVector3MapEvent OnGET_Vector3_Pack;
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset|Receivers|Vector3|Pack")
    FOnOSCVector3MapEvent OnGET_Vector3_Pack_Tick;

    // --- Rotation ---
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset|Receivers|Rotation")
    FOnOSCRotationEvent OnGET_Rotation;
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset|Receivers|Rotation")
    FOnOSCRotationEvent OnGET_Rotation_Tick;
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset|Receivers|Rotation|Pack")
    FOnOSCRotationMapEvent OnGET_Rotation_Pack;
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset|Receivers|Rotation|Pack")
    FOnOSCRotationMapEvent OnGET_Rotation_Pack_Tick;

    // --- Color ---
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset|Receivers|Color")
    FOnOSCColorEvent OnGET_Color;
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset|Receivers|Color")
    FOnOSCColorEvent OnGET_Color_Tick;
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset|Receivers|Color|Pack")
    FOnOSCColorMapEvent OnGET_Color_Pack;
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset|Receivers|Color|Pack")
    FOnOSCColorMapEvent OnGET_Color_Pack_Tick;

    // --- Transform ---
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset|Receivers|Transform")
    FOnOSCTransformEvent OnGET_Transform;
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset|Receivers|Transform")
    FOnOSCTransformEvent OnGET_Transform_Tick;
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset|Receivers|Transform|Pack")
    FOnOSCTransformMapEvent OnGET_Transform_Pack;
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset|Receivers|Transform|Pack")
    FOnOSCTransformMapEvent OnGET_Transform_Pack_Tick;

    // --- String ---
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset|Receivers|String")
    FOnOSCStringEvent OnGET_String;
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset|Receivers|String|Pack")
    FOnOSCStringArrayEvent OnGET_String_Pack;

    // --- Notes ---
    UPROPERTY(BlueprintAssignable, BlueprintReadOnly, Category = "OSCToolset|Receivers|Note")
    FOnOSCNoteEvent OnGET_Notes;
};