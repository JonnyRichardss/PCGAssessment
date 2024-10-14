// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CraterSettings.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct TESTGRAPHICSANDPCG_API FCraterSettings
{
public:
	GENERATED_BODY();
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
};
