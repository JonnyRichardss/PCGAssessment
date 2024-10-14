// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "JR_SphereGenerator.generated.h"
/**
 * 
 */
//fwd dec 
struct FGeometryScriptVectorList;
struct FCraterSettings;
extern class UDynamicMesh;
UCLASS()
class TESTGRAPHICSANDPCG_API UJR_SphereGenerator : public UBlueprintFunctionLibrary
{
	
	GENERATED_BODY()
	//TODO COPY  DEFAULT ERROR STATES ETC.

	UFUNCTION(BlueprintCallable, Category = "$JRCUSTOM")
	static UPARAM(DisplayName = "Target Mesh") UDynamicMesh* AppendCubeSphere(UDynamicMesh* TargetMesh, FGeometryScriptPrimitiveOptions PrimitiveOptions, FTransform Transform, int32 Steps, UGeometryScriptDebug* Debug);
	UFUNCTION(BlueprintCallable, Category = "$JRCUSTOM")
	static UPARAM(DisplayName = "Target Mesh") UDynamicMesh* AppendIcoSphere(UDynamicMesh* TargetMesh, FTransform Transform, int32 iterations, UGeometryScriptDebug* Debug);
	UFUNCTION(BlueprintCallable, Category = "$JRCUSTOM")
	static UPARAM(DisplayName = "Target Mesh") UDynamicMesh* WrapPlaneToSphere(UDynamicMesh* TargetMesh, double sphereRadius);

	UFUNCTION(BlueprintCallable, Category="$JRCUSTOM")
	static UPARAM(DisplayName = "Target Mesh") UDynamicMesh* AddCratersToSphere(UDynamicMesh* TargetMesh, FCraterSettings settings);



private:
	struct Crater {
		FVector centre;
		float size;
		float floorHeight;
		float smoothness;
	};
	static FVector TranslateAlongWorldCoords(FVector pos, float distance);
	static float CalculateCraterHeight(FVector pos, const TArray<Crater>& craters,FCraterSettings settings);
	static TArray<Crater> MakeCraters(FCraterSettings settings);
	static float HighBiasedRNG(float bias);
	static float LowBiasedRNG(float bias);
	static float smin(float a, float b, float k);
	static float smax(float a, float b, float k);
};
