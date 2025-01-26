// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GeneratorSettings.generated.h"

constexpr float JR_BIG_G = 6.6743e-11f;

/**
 *
 */
USTRUCT(BlueprintType)
struct TESTGRAPHICSANDPCG_API FNoiseSettings {
public:
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Octaves;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Persistence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Scale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Strength;
};
USTRUCT(BlueprintType)
struct TESTGRAPHICSANDPCG_API FNoiseMetaSettings {
public:
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f Octaves;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f Persistence;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f Scale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f Strength;
};

USTRUCT(BlueprintType)
struct TESTGRAPHICSANDPCG_API FPlanetSettings
{
public:
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int icoSphereIterations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FNoiseSettings BaseNoise;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FNoiseSettings ContinentNoise;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SeaLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float oceanDepth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float oceanDepthMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float oceanSmoothing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float landBias;
};
USTRUCT(BlueprintType)
struct TESTGRAPHICSANDPCG_API FPlanetMetaSettings
{
public:
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int icoSphereIterations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FNoiseMetaSettings BaseNoise;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FNoiseMetaSettings ContinentNoise;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f SeaLevel;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f oceanDepth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f oceanDepthMultiplier;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f oceanSmoothing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f landBias;
};
USTRUCT(BlueprintType)
struct TESTGRAPHICSANDPCG_API FAsteroidSettings
{
public:
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int icoSphereIterations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int numCraters;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float sizeBias;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f sizeMinMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f floorHeightMinMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f smoothnessMinMax;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float rimSteepness;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float rimWidth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float craterMult;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FNoiseSettings BaseNoise;
};
USTRUCT(BlueprintType)
struct TESTGRAPHICSANDPCG_API FIceBallSettings
{
public:
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int icoSphereIterations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FNoiseSettings BaseNoise;
};
USTRUCT(BlueprintType)
struct TESTGRAPHICSANDPCG_API FIceBallMetaSettings
{
public:
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int icoSphereIterations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FNoiseMetaSettings BaseNoise;
};
USTRUCT(BlueprintType)
struct TESTGRAPHICSANDPCG_API FStarSettings
{
public:
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float size;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float brightness;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float mass;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FLinearColor col1;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FLinearColor col2;

};
USTRUCT(BlueprintType)
struct TESTGRAPHICSANDPCG_API FStarMetaSettings
{
public:
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f size;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f brightness;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f mass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f hue1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f sat1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f val1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f hue2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f sat2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f val2;
};

