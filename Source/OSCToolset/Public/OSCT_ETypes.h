// Copyright (c) 2024, Studio Eusebi Jucgla. All rights reserved 
// MD5: 2245b93ed1bae2e4aff277e6f3ecb8ff
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"

#include "OSCT_ETypes.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EOSCT_Role : uint8
{
	RECEIVER	UMETA(DisplayName = "Receiver"),
	SENDER		UMETA(DisplayName = "Sender"),
};

UENUM(BlueprintType)
enum class EOSCT_ModuleType : uint8
{
	EVENT	UMETA(DisplayName = "Event"),
	BOOL     UMETA(DisplayName = "Boolean"),
	FLOAT	UMETA(DisplayName = "Float"),
	INT     UMETA(DisplayName = "Integer"),
	VEC2	UMETA(DisplayName = "Vector 2"),
	VEC3	UMETA(DisplayName = "Vector 3"),
	COLOR	UMETA(DisplayName = "Color"),
	TRANSFORM	UMETA(DisplayName = "Transform"),	
	STRING		UMETA(DisplayName = "String"),
	ROTATION	UMETA(DisplayName = "Rotation"),
	NOTE		UMETA(DisplayName = "Note"),
};

UENUM()
enum class EOSCT_RouteType : uint8
{
	EVENT			UMETA(DisplayName = "Event"),
	EVENT_PACK		UMETA(DisplayName = "Event Pack"),
	BOOL			UMETA(DisplayName = "Boolean"),
	BOOL_PACK		UMETA(DisplayName = "Boolean Pack"),
	FLOAT			UMETA(DisplayName = "Float"),
	FLOAT_PACK		UMETA(DisplayName = "Float Pack"),
	INT				UMETA(DisplayName = "Integer"),
	INT_PACK		UMETA(DisplayName = "Integer Pack"),
	VEC2			UMETA(DisplayName = "Vector 2"),
	VEC2_PACK		UMETA(DisplayName = "Vector 2 Pack"),
	VEC3			UMETA(DisplayName = "Vector 3"),
	VEC3_PACK		UMETA(DisplayName = "Vector 3 Pack"),
	COLOR			UMETA(DisplayName = "Color"),
	COLOR_PACK		UMETA(DisplayName = "Color Pack"),
	TRANSFORM		UMETA(DisplayName = "Transform"),	
	TRANSFORM_PACK	UMETA(DisplayName = "Transform Pack"),	
	STRING			UMETA(DisplayName = "String"),
	STRING_PACK		UMETA(DisplayName = "String Pack"),
	ROTATION		UMETA(DisplayName = "Rotation"),
	ROTATION_PACK	UMETA(DisplayName = "Rotation Pack"),
	NOTE			UMETA(DisplayName = "Note")
};

UENUM(BlueprintType)
enum class EOSCT_SenderType : uint8
{
	EVENT	UMETA(DisplayName = "Event"),
	FLOAT	UMETA(DisplayName = "Float"),
	STRING	UMETA(DisplayName = "String"),
};
	
USTRUCT(BlueprintType)
struct FOSCT_Note
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "OSCToolset")
	int32 Pitch = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "OSCToolset")
	int32 Velocity = 0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "OSCToolset")
	int32 Voices = 1;
	
	FString ToString() const
	{
		return FString::Printf(TEXT("Pitch: %d Velocity: %d  Voices:%d"), Pitch, Velocity, Voices);
	}
};

USTRUCT(BlueprintType)
struct FOSCT_BoolMapWrapper { GENERATED_BODY(); UPROPERTY(BlueprintReadWrite, Category="OSCToolset") TMap<FString, bool> Data; };

USTRUCT(BlueprintType)
struct FOSCT_IntMapWrapper { GENERATED_BODY(); UPROPERTY(BlueprintReadWrite, Category="OSCToolset") TMap<FString, int32> Data; };

USTRUCT(BlueprintType)
struct FOSCT_FloatMapWrapper { GENERATED_BODY(); UPROPERTY(BlueprintReadWrite, Category="OSCToolset") TMap<FString, float> Data; };

USTRUCT(BlueprintType)
struct FOSCT_Vector2MapWrapper { GENERATED_BODY(); UPROPERTY(BlueprintReadWrite, Category="OSCToolset") TMap<FString, FVector2D> Data; };

USTRUCT(BlueprintType)
struct FOSCT_Vector3MapWrapper { GENERATED_BODY(); UPROPERTY(BlueprintReadWrite, Category="OSCToolset") TMap<FString, FVector> Data; };

USTRUCT(BlueprintType)
struct FOSCT_RotationMapWrapper { GENERATED_BODY(); UPROPERTY(BlueprintReadWrite, Category="OSCToolset") TMap<FString, FRotator> Data; };

USTRUCT(BlueprintType)
struct FOSCT_ColorMapWrapper { GENERATED_BODY(); UPROPERTY(BlueprintReadWrite, Category="OSCToolset") TMap<FString, FLinearColor> Data; };

USTRUCT(BlueprintType)
struct FOSCT_TransformMapWrapper { GENERATED_BODY(); UPROPERTY(BlueprintReadWrite, Category="OSCToolset") TMap<FString, FTransform> Data; };


class OSCTOOLSET_API OSCT_ETypes
{
public:
	OSCT_ETypes();
	~OSCT_ETypes();
};
