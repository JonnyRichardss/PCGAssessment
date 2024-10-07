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
extern class UDynamicMesh;
UCLASS()
class TESTGRAPHICSANDPCG_API UJR_SphereGenerator : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable,Category="$JRCUSTOM")
	static FGeometryScriptVectorList Sphere();
	UFUNCTION(BlueprintPure, Category = "$JRCUSTOM")
	static double Number();
	UFUNCTION(BlueprintCallable, Category = "$JRCUSTOM")
	static UDynamicMesh* NormalisePoints(UDynamicMesh* meshIn);
	UFUNCTION(BlueprintCallable, Category = "$JRCUSTOM")
	static void Unused(FGeometryScriptVectorList& VectorIn);
	UFUNCTION(BlueprintCallable, Category = "$JRCUSTOM")
	static UPARAM(DisplayName = "Target Mesh") UDynamicMesh* AppendCubeSphere(UDynamicMesh* TargetMesh,
		FGeometryScriptPrimitiveOptions PrimitiveOptions,
		FTransform Transform,
		int32 Steps,
		UGeometryScriptDebug* Debug);
	UFUNCTION(BlueprintCallable, Category = "$JRCUSTOM")
	static UDynamicMesh* PlaneToSphere(UDynamicMesh* meshIn,double sphereRadius);
};
