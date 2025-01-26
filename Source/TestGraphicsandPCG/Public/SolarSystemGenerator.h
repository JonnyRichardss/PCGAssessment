// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SolarSystemGenerator.generated.h"
/**
 * 
 */
struct FSolarSystemSettings;
struct FSolarSystemMetaSettings;

struct FPlanetPlacementSettings;
struct FCometPlacementSettings;

struct FNoiseSettings;
struct FPlanetSettings;
struct FRingSettings;
struct FStarSettings;
struct FIceBallSettings;

struct FNoiseMetaSettings;
struct FIceBallMetaSettings;
struct FPlanetMetaSettings;
struct FAsteroidMetaSettings;
struct FRingMetaSettings;
struct FStarMetaSettings;

enum class PlanetType : uint8;
UCLASS()
class TESTGRAPHICSANDPCG_API U_JR_SolarSystemGenerator : public UBlueprintFunctionLibrary
{
public:
	GENERATED_BODY()
	UFUNCTION(BlueprintCallable, Category = "JR_SolarSystem")
	static FSolarSystemSettings GenerateSystemTemplate(FSolarSystemMetaSettings settings);
	UFUNCTION(BlueprintCallable,BlueprintPure, Category = "JR_Maths")
	//always returns m1 -> m2 FORCE
	static FVector NewtonGravity(const float& m1, const float& m2, const FVector& p1, const FVector& p2);
	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "JR_Maths")
	static const float GetBigG();


	//TODO add makeStar
	UFUNCTION(BlueprintCallable, Category = "JR_SolarSystem")
	static void MakeComets(TArray<FCometPlacementSettings>& comets, const FSolarSystemMetaSettings& settings);
	UFUNCTION(BlueprintCallable, Category = "JR_SolarSystem")
	static void MakePlanetsAndRings(TArray<FPlanetPlacementSettings>& planets,TArray<FRingSettings>& rings, const FSolarSystemMetaSettings& settings,float SolarMass);
	UFUNCTION(BlueprintCallable, Category = "JR_SolarSystem")
	static FPlanetPlacementSettings MakeSinglePlanet(const FPlanetMetaSettings& settings, PlanetType type, float size, float rotSpeed);
	UFUNCTION(BlueprintCallable, Category = "JR_SolarSystem")
	static FRingSettings MakeRing(const FRingMetaSettings& settings, float PlanetSize);
	UFUNCTION(BlueprintCallable, Category = "JR_SolarSystem")
	static FIceBallSettings MakeIceBall(const FIceBallMetaSettings& settings);
	UFUNCTION(BlueprintCallable, Category = "JR_SolarSystem")
	static FPlanetSettings MakePlanet(const FPlanetMetaSettings& settings);
	UFUNCTION(BlueprintCallable, Category = "JR_SolarSystem")
	static FNoiseSettings MakeNoise(const FNoiseMetaSettings& settings);
	UFUNCTION(BlueprintCallable, Category = "JR_SolarSystem")
	static FStarSettings MakeStar(const FStarMetaSettings& settings);
	static FLinearColor MakeColourHSV(const FVector2f& hue, const FVector2f& sat, const FVector2f& val);
}; 
