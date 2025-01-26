// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GeneratorSettings.h"
#include "CoreMinimal.h"
#include "SolarSystemSettings.generated.h"


/**
 *
 */
UENUM(BlueprintType)
enum class PlanetType : uint8 {
	EarthLike,
	GasGiant,
	Rocky
};
USTRUCT(BlueprintType)
struct TESTGRAPHICSANDPCG_API FRingSettings {
public:
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OrbitRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DiscCoverage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RingRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SpawnCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxRotationSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RockScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector orbitPlane;
};
USTRUCT(BlueprintType)
struct TESTGRAPHICSANDPCG_API FRingMetaSettings {
public:
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f OrbitRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f DiscCoverage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f RingRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MinSpawnCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxSpawnCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f MaxRotationSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f RockScale;
};
USTRUCT(BlueprintType)
struct TESTGRAPHICSANDPCG_API FPlanetPlacementSettings {
public:
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	PlanetType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OrbitRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlanetSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotationSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bHasRing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRingSettings RingSettings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlanetSettings GenerationSettings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OrbitSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OrbitTimeScale;
};

USTRUCT(BlueprintType)
struct TESTGRAPHICSANDPCG_API FCometPlacementSettings {
public:
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitialRadius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float InitialVelocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Size;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Mass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlaneDeviation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float OrbitTimeScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float IonLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float IonMaxDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIceBallSettings GeneratorSettings;
};
USTRUCT(BlueprintType)
struct TESTGRAPHICSANDPCG_API FCometMetaSettings {
public:
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MinComets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxComets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f PlaneDeviation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f Size;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f Mass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f TimeScale;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f Radius;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f Velocity;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f IonLength;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f IonMaxDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIceBallMetaSettings GenerationSettings;
};
USTRUCT(BlueprintType)
struct TESTGRAPHICSANDPCG_API FSolarSystemSettings {
public:
	GENERATED_BODY()
	//TODO? Add binary stars
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FStarSettings Star;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPlanetPlacementSettings> Planets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRingSettings> SolarRings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FCometPlacementSettings> Comets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector orbitPlane;
	
};


USTRUCT(BlueprintType)
struct TESTGRAPHICSANDPCG_API FSolarSystemMetaSettings {
public:
	GENERATED_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FStarMetaSettings Star;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f MinOrbit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f MaxOrbit;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MinPlanets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxPlanets;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MinSolarRings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxSolarRings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f PlanetClearing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f RingClearing;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f PlanetSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f PlanetRotationSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f RockPlanetSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RockPlanetWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlanetMetaSettings RockPlanetSettings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f EarthPlanetSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EarthPlanetWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlanetMetaSettings EarthPlanetSettings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector2f GasPlanetSize;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GasPlanetWeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlanetMetaSettings GasPlanetSettings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRingMetaSettings PlanetaryRings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRingMetaSettings SolarRings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FCometMetaSettings CometSettings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool RandomPlane;
	//TODO TOADD sun specifc settings

};