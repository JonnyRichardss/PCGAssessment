// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MeshFunctions.generated.h"
/**
 * 
 */
//fwd dec 
struct FGeometryScriptVectorList;
struct FAsteroidSettings;
struct FPlanetSettings;
struct FIceBallSettings;
struct FCometSettings;
namespace UE {
	namespace Geometry {
		class FDynamicMesh3;
	}
};

extern class UDynamicMesh;
UCLASS()
class TESTGRAPHICSANDPCG_API U_JR_MeshFunctions : public UBlueprintFunctionLibrary
{
public:
	GENERATED_BODY()
	//TODO COPY  DEFAULT ERROR STATES ETC.

	UFUNCTION(BlueprintCallable, Category = "JR_MeshFunctions")
	static UPARAM(DisplayName = "Target Mesh") UDynamicMesh* AppendCubeSphere(UDynamicMesh* TargetMesh, FGeometryScriptPrimitiveOptions PrimitiveOptions, FTransform Transform, int32 Steps, UGeometryScriptDebug* Debug);
	UFUNCTION(BlueprintCallable, Category = "JR_MeshFunctions")
	static UPARAM(DisplayName = "Target Mesh") UDynamicMesh* AppendIcoSphere(UDynamicMesh* TargetMesh, FTransform Transform, int32 iterations);
	UFUNCTION(BlueprintCallable, Category = "JR_MeshFunctions")
	static UPARAM(DisplayName = "Target Mesh") UDynamicMesh* WrapPlaneToSphere(UDynamicMesh* TargetMesh, double sphereRadius);

	UFUNCTION(BlueprintCallable, Category="JR_MeshFunctions")
	static UPARAM(DisplayName = "Target Mesh") UDynamicMesh* AppendAsteroid(UDynamicMesh* TargetMesh, FAsteroidSettings settings, FTransform Transform);
	UFUNCTION(BlueprintCallable, Category="JR_MeshFunctions")
	static UPARAM(DisplayName = "Target Mesh") UDynamicMesh* AppendIceBall(UDynamicMesh* TargetMesh, FIceBallSettings settings, FTransform Transform);
	UFUNCTION(BlueprintCallable, Category="JR_MeshFunctions")
	static UPARAM(DisplayName = "Target Mesh") UDynamicMesh* AppendEarthPlanet(UDynamicMesh* TargetMesh, FPlanetSettings settings, FTransform Transform, FVector3f BaseColor, FVector3f HighColor);
	UFUNCTION(BlueprintCallable, Category="JR_MeshFunctions")
	static UPARAM(DisplayName = "Target Mesh") UDynamicMesh* AppendRockPlanet(UDynamicMesh* TargetMesh, FPlanetSettings settings, FTransform Transform, FVector3f BaseColor, FVector3f HighColor);


	static UDynamicMesh* EarthPlanetDeformation(UDynamicMesh* TargetMesh, FPlanetSettings settings, FVector3f BaseColor, FVector3f HighColor);
	static UDynamicMesh* RockPlanetDeformation(UDynamicMesh* TargetMesh, FPlanetSettings settings, FVector3f BaseColor, FVector3f HighColor);


private:
	struct Crater {
		FVector centre;
		float size;
		float floorHeight;
		float smoothness;
	};
	static FVector TranslateAlongWorldCoords(FVector pos, float distance);
	static UDynamicMesh* AddAsteroidCraters(UDynamicMesh* TargetMesh, FAsteroidSettings settings);
	static float CalculateCraterHeight(FVector pos, const TArray<Crater>& craters,FAsteroidSettings settings);
	static TArray<Crater> MakeCraters(FAsteroidSettings settings);
	static float HighBiasedRNG(float bias);
	static float LowBiasedRNG(float bias);
	static float smin(float a, float b, float k);
	static float smax(float a, float b, float k);
	static int AppendMidPoint(int a, int b,UE::Geometry::FDynamicMesh3* EditMesh);
};
