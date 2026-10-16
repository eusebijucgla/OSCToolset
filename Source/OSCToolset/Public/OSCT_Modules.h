// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OSCT_ETypes.h"
#include "OSCAddress.h"
#include "Engine/DataTable.h"
#include "OSCT_Modules.generated.h"

USTRUCT(BlueprintType)
struct FOSCT_ModuleDebug
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OSCToolset|Debug")
	bool PrintOnScreen = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OSCToolset|Debug")
	bool PrintOnLog = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OSCToolset|Debug")
	FColor DebugColor = FColor::White;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OSCToolset|Debug")
	float DebugDuration = 2.0f;
};

USTRUCT(BlueprintType)
struct FOSCT_ReceiverTick
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OSCToolset|Tick")
	bool bEnable = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OSCToolset|Tick")
	float InterpolationSpeed = 5.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OSCToolset|Tick")
	float Tolerance = 0.001f;
};


//Tables don't like condition hides from structs, so this is a mirror to create them.
//There will be a function to convert the type to FOSCT_Receiver
USTRUCT()
struct FOSCT_ReceiverRow: public FTableRowBase
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OSCToolset")
	FString Address;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OSCToolset")
	EOSCT_ModuleType ModuleType = EOSCT_ModuleType::FLOAT;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OSCToolset")
	bool Pack = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OSCToolset|Tick")
	FOSCT_ReceiverTick Tick;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OSCToolset|Debug")
	FOSCT_ModuleDebug Debug;
};


USTRUCT(BlueprintType)
struct FOSCT_ReceiverProxy
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OSCToolset")
	FString Address;
	
	UPROPERTY()
	EOSCT_Role Role = EOSCT_Role::RECEIVER;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OSCToolset")
	EOSCT_ModuleType ModuleType = EOSCT_ModuleType::FLOAT;
	
	UPROPERTY(
		EditAnywhere, 
		BlueprintReadWrite, 
		Category = "OSCToolset",
		meta=(
			EditCondition = "ModuleType != EOSCT_ModuleType::NOTE", 
			EditConditionHides))
	bool Pack = false;
	
};

USTRUCT(BlueprintType)
struct FOSCT_Receiver
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString FormattedAddress;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OSCToolset")
	FString Address;
	
	UPROPERTY()
	EOSCT_Role Role = EOSCT_Role::RECEIVER;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OSCToolset")
	EOSCT_ModuleType ModuleType = EOSCT_ModuleType::FLOAT;
	
	UPROPERTY(
		EditAnywhere, 
		BlueprintReadWrite, 
		Category = "OSCToolset",
		meta=(
			EditCondition = "ModuleType != EOSCT_ModuleType::NOTE", 
			EditConditionHides))
	bool Pack = false;
	
	UPROPERTY(
		EditAnywhere, 
		BlueprintReadWrite, 
		Category = "OSCToolset",
		meta=(
			EditCondition = "ModuleType != EOSCT_ModuleType::EVENT && ModuleType != EOSCT_ModuleType::BOOL && ModuleType != EOSCT_ModuleType::INT && ModuleType != EOSCT_ModuleType::NOTE && ModuleType != EOSCT_ModuleType::STRING", 
			EditConditionHides))
	FOSCT_ReceiverTick Tick;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OSCToolset|Debug")
	FOSCT_ModuleDebug Debug;
	
	// Constructor to convert from Table Row
	FOSCT_Receiver(const FOSCT_ReceiverRow& Row)
	{
		Address = Row.Address;
		ModuleType = Row.ModuleType;
		Pack = Row.Pack;
		Tick = Row.Tick;
		Debug = Row.Debug;
		Role = EOSCT_Role::RECEIVER;
	}

	// Default constructor (required by GENERATED_BODY)
	FOSCT_Receiver() {}
};


USTRUCT()
struct FOSCT_ReceiverLink
{
	GENERATED_BODY()
	TWeakObjectPtr<UObject> Owner;
	
	UPROPERTY()
	FOSCT_Receiver Data;
	
	bool bIsFirstFrame = false;
	bool bNeedsInterpolation = false;
	bool bInitialized = false;
	
	UObject* GetOwner() const { return Owner.Get(); }
	bool HasValidOwner() const { return Owner.IsValid(); }
	// This allows AddUnique and Contains to work
	bool operator==(const FOSCT_ReceiverLink& Other) const
	{
		return GetOwner() == Other.GetOwner() && Data.Address == Other.Data.Address;
	}
};


//Interpolation declarations
// For float
static void Interp(float& Cur, const float& Target, float DeltaTime, float Speed) {
	Cur = FMath::FInterpTo(Cur, Target, DeltaTime, Speed);
}
// For 2D Vector
static void Interp(FVector2D& Cur, const FVector2D& Target, float DeltaTime, float Speed) {
	Cur = FMath::Vector2DInterpTo(Cur, Target, DeltaTime, Speed);
}
// For FVector
static void Interp(FVector& Cur, const FVector& Target, float DeltaTime, float Speed) {
	Cur = FMath::VInterpTo(Cur, Target, DeltaTime, Speed);
}
// For FRotator
static void Interp(FRotator& Cur, const FRotator& Target, float DeltaTime, float Speed) {
	Cur = FMath::RInterpTo(Cur, Target, DeltaTime, Speed);
}
// For FLinearColor
static void Interp(FLinearColor& Cur, const FLinearColor& Target, float DeltaTime, float Speed)
{
	Cur = FMath::CInterpTo(Cur, Target, DeltaTime, Speed);
}
// For FTransform
static void Interp(FTransform& Cur, const FTransform& Target, float DeltaTime, float Speed) {
	FVector NewLoc = FMath::VInterpTo(Cur.GetLocation(), Target.GetLocation(), DeltaTime, Speed);
	FQuat NewRot = FQuat::Slerp(Cur.GetRotation(), Target.GetRotation(), DeltaTime * Speed);
	FVector NewScale = FMath::VInterpTo(Cur.GetScale3D(), Target.GetScale3D(), DeltaTime, Speed);
	Cur.SetLocation(NewLoc);
	Cur.SetRotation(NewRot);
	Cur.SetScale3D(NewScale);
}

//Equality functions
template<typename T>
static auto IsNearlyEqual(const T& A, const T& B, float Tolerance) -> decltype(A.Equals(B), bool()) {
	return A.Equals(B, Tolerance);
}
// Specific override for floats (since they don't have .Equals)
static bool IsNearlyEqual(float A, float B, float Tolerance) {
	return FMath::IsNearlyEqual(A, B, Tolerance);
}
// For Rotators
static bool IsNearlyEqual(const FRotator& A, const FRotator& B, float Tolerance) {
	// This handles the 0-360 wrapping correctly
	return A.Quaternion().Equals(B.Quaternion(), Tolerance);
}
template<typename T>
struct TOSCT_LinkBase
{
	T CurrentValue;
	T TargetValue;
	
	bool bIsCurrentlySettled = true;

	bool IsSettled(float Tolerance) const
	{
		return IsNearlyEqual(TargetValue, CurrentValue, Tolerance);
	}

	// Every child using this template will have this exact logic
	void Interpolate(float DeltaTime, float Speed, float Tolerance)
	{
		Interp(CurrentValue, TargetValue, DeltaTime, Speed);
	}
};

template<typename T>
struct TOSCT_PackLinkBase
{
	TMap<FString, T> CurrentValue;
	TMap<FString, T> TargetValue;
	bool bIsCurrentlySettled = true;

	bool IsSettled(float Tolerance) const { return bIsCurrentlySettled; }

	void Interpolate(float DeltaTime, float Speed, float Tolerance)
	{
		bIsCurrentlySettled = true;
		for (auto& Pair : TargetValue)
		{
			T& Cur = CurrentValue.FindOrAdd(Pair.Key);
			if (!IsNearlyEqual(Cur, Pair.Value, Tolerance))
			{
				Interp(Cur, Pair.Value, DeltaTime, Speed);
				bIsCurrentlySettled = false;
				if (IsNearlyEqual(Cur, Pair.Value, Tolerance))
				{
					Cur = Pair.Value; // Snap to exact target
				}
			}
			else
			{
				Cur = Pair.Value;  // Already within tolerance, ensure it is exactly the target
			}
		}
	}
};


////////////// Receiver Types

/// EVENT
USTRUCT()
struct FOSCT_EventLink: public FOSCT_ReceiverLink
#if CPP
	, public TOSCT_LinkBase<bool>
#endif
{
	GENERATED_BODY()
};

/// BOOL
USTRUCT()
struct FOSCT_BoolLink: public FOSCT_ReceiverLink
#if CPP
	, public TOSCT_LinkBase<bool>
#endif
{
	GENERATED_BODY()
};

/// FLOAT
USTRUCT()
struct FOSCT_FloatLink: public FOSCT_ReceiverLink
#if CPP
	, public TOSCT_LinkBase<float>
#endif
{
	GENERATED_BODY()
	using ValueType = float;

};

/// INT
USTRUCT()
struct FOSCT_IntegerLink: public FOSCT_ReceiverLink
#if CPP
	, public TOSCT_LinkBase<int32>
#endif
{
	GENERATED_BODY()
	using ValueType = float;

};

/// VECTOR2
USTRUCT()
struct FOSCT_Vector2Link: public FOSCT_ReceiverLink
#if CPP
	, public TOSCT_LinkBase<FVector2D>
#endif
{
	GENERATED_BODY()
	using ValueType = FVector2D;
};


/// VECTOR3
USTRUCT()
struct FOSCT_Vector3Link: public FOSCT_ReceiverLink
#if CPP
	, public TOSCT_LinkBase<FVector>
#endif
{
	GENERATED_BODY()
	using ValueType = FVector;
};

/// ROTATION
USTRUCT()
struct FOSCT_RotationLink: public FOSCT_ReceiverLink
#if CPP
	, public TOSCT_LinkBase<FRotator>
#endif
{
	GENERATED_BODY()
	using ValueType = FRotator;
};


/// COLOR
USTRUCT()
struct FOSCT_ColorLink: public FOSCT_ReceiverLink
#if CPP
	, public TOSCT_LinkBase<FLinearColor>
#endif
{
	GENERATED_BODY()
	using ValueType = FLinearColor;
};


/// TRANSFORM
USTRUCT()
struct FOSCT_TransformLink: public FOSCT_ReceiverLink
#if CPP
	, public TOSCT_LinkBase<FTransform>
#endif
{
	GENERATED_BODY()
	using ValueType = FTransform;
};


/// STRING
USTRUCT()
struct FOSCT_StringLink: public FOSCT_ReceiverLink
{
	GENERATED_BODY()

	UPROPERTY()
	FString CurrentValue;
	UPROPERTY()
	FString TargetValue;

};
USTRUCT()
struct FOSCT_StringPackLink: public FOSCT_ReceiverLink
{
	GENERATED_BODY()

	UPROPERTY()
	TArray < FString > CurrentValue;
	UPROPERTY()
	TArray < FString > TargetValue;

};

/// NOTE
USTRUCT()
struct FOSCT_NoteLink: public FOSCT_ReceiverLink
{
	GENERATED_BODY()

	UPROPERTY()
	FOSCT_Note CurrentValue;
	UPROPERTY()
	FOSCT_Note TargetValue;
};


///Packs

/// EVENT PACK
USTRUCT()
struct FOSCT_EventPackLink: public FOSCT_ReceiverLink
#if CPP
	, public TOSCT_PackLinkBase<bool>
#endif
{
	GENERATED_BODY()
	using ValueType = TMap < FString, bool >;
};

/// BOOL PACK
USTRUCT()
struct FOSCT_BoolPackLink: public FOSCT_ReceiverLink
#if CPP
	, public TOSCT_PackLinkBase<bool>
#endif
{
	GENERATED_BODY()
	using ValueType = TMap < FString, bool >;
};

/// FLOAT PACK
USTRUCT()
struct FOSCT_FloatPackLink: public FOSCT_ReceiverLink
#if CPP
	, public TOSCT_PackLinkBase<float>
#endif
{
	GENERATED_BODY()
	using ValueType = TMap < FString, float >;
};

/// INT PACK
USTRUCT()
struct FOSCT_IntegerPackLink: public FOSCT_ReceiverLink
#if CPP
	, public TOSCT_PackLinkBase<int32>
#endif
{
	GENERATED_BODY()
	using ValueType = TMap < FString, int32 >;
};


/// VEC2 PACK
USTRUCT()
struct FOSCT_Vector2PackLink: public FOSCT_ReceiverLink
#if CPP
	, public TOSCT_PackLinkBase<FVector2D>
#endif
{
	GENERATED_BODY()
	using ValueType = TMap < FString, FVector2D >;
};

/// VEC3 PACK
USTRUCT()
struct FOSCT_Vector3PackLink: public FOSCT_ReceiverLink
#if CPP
	, public TOSCT_PackLinkBase<FVector>
#endif
{
	GENERATED_BODY()
	using ValueType = TMap < FString, FVector >;
};

/// ROTATION PACK
USTRUCT()
struct FOSCT_RotationPackLink: public FOSCT_ReceiverLink
#if CPP
	, public TOSCT_PackLinkBase<FRotator>
#endif
{
	GENERATED_BODY()
	using ValueType = TMap < FString, FRotator >;
};

/// COLOR PACK
USTRUCT()
struct FOSCT_ColorPackLink: public FOSCT_ReceiverLink
#if CPP
	, public TOSCT_PackLinkBase<FLinearColor>
#endif
{
	GENERATED_BODY()
	using ValueType = TMap < FString, FLinearColor > ;
};

/// TRANSFORM PACK
USTRUCT()
struct FOSCT_TransformPackLink: public FOSCT_ReceiverLink
#if CPP
	, public TOSCT_PackLinkBase<FTransform>
#endif
{
	GENERATED_BODY()
	using ValueType = TMap < FString, FTransform >;

};

/// Sender
USTRUCT(BlueprintType)
struct FOSCT_Sender
{
	GENERATED_BODY()
	
	UPROPERTY()
	FString FormattedAddress;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OSCToolset")
	FString Address;
	
	UPROPERTY()
	FOSCAddress CachedFullAddress;
	
	UPROPERTY()
	EOSCT_Role Role = EOSCT_Role::SENDER;

	UPROPERTY()
	EOSCT_ModuleType ModuleType = EOSCT_ModuleType::EVENT;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "OSCToolset|Debug")
	FOSCT_ModuleDebug Debug;
	
};


class OSCTOOLSET_API OSCT_Modules
{
public:
	OSCT_Modules();
	~OSCT_Modules();
};
