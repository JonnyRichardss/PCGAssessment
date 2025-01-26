// Fill out your copyright notice in the Description page of Project Settings.


#include "SolarSystemGenerator.h"
#include "SolarSystemSettings.h"
#include "Kismet/KismetSystemLibrary.h"
FSolarSystemSettings U_JR_SolarSystemGenerator::GenerateSystemTemplate(FSolarSystemMetaSettings settings)
{
	//TODO Star and solar rings settings
	FSolarSystemSettings output = FSolarSystemSettings();
	output.Star = MakeStar(settings.Star);
	MakePlanetsAndRings(output.Planets,output.SolarRings,settings,output.Star.mass);
	MakeComets(output.Comets,settings);

	output.orbitPlane = settings.RandomPlane ? FMath::VRand() : FVector::UpVector;
	return output;
}

FVector U_JR_SolarSystemGenerator::NewtonGravity(const float& m1, const float& m2, const FVector& p1, const FVector& p2)
{
	FVector displacement = p2 - p1;
	double mag = (JR_BIG_G * m1 * m2) / (displacement.Length() * displacement.Length());
	displacement.Normalize();
	return displacement * mag;
}

const float U_JR_SolarSystemGenerator::GetBigG()
{
	return JR_BIG_G;
}


void U_JR_SolarSystemGenerator::MakeComets(TArray<FCometPlacementSettings>& comets, const FSolarSystemMetaSettings& settings)
{
	int NumComets = FMath::RandRange(settings.CometSettings.MinComets, settings.CometSettings.MaxComets);
	comets.Reserve(NumComets);
	for (int i = 0; i < NumComets; i++) {
		comets.Emplace(FCometPlacementSettings{
			FMath::FRandRange(settings.CometSettings.Radius.X,settings.CometSettings.Radius.Y),
			FMath::FRandRange(settings.CometSettings.Velocity.X,settings.CometSettings.Velocity.Y),
			FMath::FRandRange(settings.CometSettings.Size.X,settings.CometSettings.Size.Y),
			FMath::FRandRange(settings.CometSettings.Mass.X,settings.CometSettings.Mass.Y),
			FMath::FRandRange(settings.CometSettings.PlaneDeviation.X,settings.CometSettings.PlaneDeviation.Y),
			FMath::FRandRange(settings.CometSettings.TimeScale.X,settings.CometSettings.TimeScale.Y),
			FMath::FRandRange(settings.CometSettings.IonLength.X,settings.CometSettings.IonLength.Y),
			FMath::FRandRange(settings.CometSettings.IonMaxDistance.X,settings.CometSettings.IonMaxDistance.Y),
			MakeIceBall(settings.CometSettings.GenerationSettings)
			});
	}
}

void U_JR_SolarSystemGenerator::MakePlanetsAndRings(TArray<FPlanetPlacementSettings>& planets, TArray<FRingSettings>& rings, const FSolarSystemMetaSettings& settings,float SolarMass)
{
	//TODO allow biasing of planet distributions
	//way to do it would be to pull the biased RNG out to a header
	//and obv floor the result
	int NumRings = FMath::RandRange(settings.MinSolarRings, settings.MaxSolarRings);
	int NumPlanets = FMath::RandRange(settings.MinPlanets, settings.MaxPlanets);
	float minOrbit = FMath::FRandRange(settings.MinOrbit.X, settings.MinOrbit.Y);
	float maxOrbit = FMath::FRandRange(settings.MaxOrbit.X, settings.MaxOrbit.Y);


	float totalWeight = settings.EarthPlanetWeight + settings.GasPlanetWeight + settings.RockPlanetWeight;
	
	
	planets.Reserve(NumPlanets);
	rings.Reserve(NumRings);
	float rad = minOrbit;
	TArray<int> ringIdx;
	for (int i = 0; i < NumRings; i++) {
		ringIdx.Add(FMath::RandRange(0, (NumPlanets + NumRings)));
	}
	for (int i = 0; i < (NumPlanets + NumRings); i++) {
		if (rad > maxOrbit) {
			UKismetSystemLibrary::PrintString(nullptr, FString::Printf(TEXT("WARN: Not enough space, skipped planets!")));
			break;
		}
		if (ringIdx.Contains(i)) {
			float clearing = FMath::FRandRange(settings.RingClearing.X, settings.RingClearing.Y) * 0.5f;
			rad += clearing;
			UKismetSystemLibrary::PrintString(nullptr, FString::Printf(TEXT("Ring placement: %i idx %f rad"),i,rad));
			rings.Emplace(MakeRing(settings.SolarRings, rad));
			rad += clearing;
		}
		else {
			PlanetType type = (PlanetType)FMath::RandRange(0, 2);//legacy code just to initialize
			//this is some of the funkiest code ive ever written in my life
			//but the people on twitter told me not to use GOTO ever so I didn't lmao
			float weight = FMath::FRandRange(0, totalWeight);
			do {
				if (weight < settings.EarthPlanetWeight) { 
					type = PlanetType::EarthLike;
					break; 
				}
				else weight -= settings.EarthPlanetWeight;

				if (weight < settings.GasPlanetWeight) {
					type = PlanetType::GasGiant; 
					break;
				}
				else weight -= settings.GasPlanetWeight;

				if (weight < settings.RockPlanetWeight) {
					type = PlanetType::Rocky;
					break;
				}
				else weight -= settings.RockPlanetWeight;
			} while (0);

			FPlanetMetaSettings PlanetSettings;
			float size = 1.0f;
			switch (type) {
			case PlanetType::Rocky:
				PlanetSettings = settings.RockPlanetSettings;
				size = FMath::FRandRange(settings.RockPlanetSize.X, settings.RockPlanetSize.Y);
				break;
			case PlanetType::EarthLike:
				PlanetSettings = settings.EarthPlanetSettings;
				size = FMath::FRandRange(settings.EarthPlanetSize.X, settings.EarthPlanetSize.Y);
				break;
			case PlanetType::GasGiant:
				PlanetSettings = settings.GasPlanetSettings;
				size = FMath::FRandRange(settings.GasPlanetSize.X, settings.GasPlanetSize.Y);
				break;
			}
			float clearing = FMath::FRandRange(settings.PlanetClearing.X, settings.PlanetClearing.Y) * size * 0.5;
			rad += clearing;

			float speed = FMath::Sqrt((JR_BIG_G * SolarMass) / rad);

			planets.Emplace(FPlanetPlacementSettings{
				type,
				rad,
				size,
				FMath::FRandRange(settings.PlanetRotationSpeed.X,settings.PlanetRotationSpeed.Y),
				FMath::RandBool(),
				MakeRing(settings.PlanetaryRings,size),
				MakePlanet(PlanetSettings),
				speed,
				FMath::FRandRange(settings.PlanetSpeed.X,settings.PlanetSpeed.Y)
				});

			rad += clearing;
		}
	}
}

FPlanetPlacementSettings U_JR_SolarSystemGenerator::MakeSinglePlanet(const FPlanetMetaSettings& settings,PlanetType type,float size,float rotSpeed)
{

	return(FPlanetPlacementSettings{
		type,
		0,
		size,
		rotSpeed,
		FMath::RandBool(),
		FRingSettings{},
		MakePlanet(settings),
		0,
		0
		});
}


FRingSettings U_JR_SolarSystemGenerator::MakeRing(const FRingMetaSettings& settings,float PlanetSize)
{
	return FRingSettings{
					FMath::FRandRange(settings.OrbitRadius.X,settings.OrbitRadius.Y) * (PlanetSize / 1000),
					FMath::FRandRange(settings.DiscCoverage.X,settings.DiscCoverage.Y),
					FMath::FRandRange(settings.RingRadius.X,settings.RingRadius.Y),
					FMath::RandRange(settings.MinSpawnCount,settings.MaxSpawnCount),
					FMath::FRandRange(settings.MaxRotationSpeed.X,settings.MaxRotationSpeed.Y),
					FMath::FRandRange(settings.RockScale.X,settings.RockScale.Y),
					FMath::VRand()
	};
}

FIceBallSettings U_JR_SolarSystemGenerator::MakeIceBall(const FIceBallMetaSettings& settings)
{
	return FIceBallSettings{
		settings.icoSphereIterations,
		MakeNoise(settings.BaseNoise)
	};
}

FPlanetSettings U_JR_SolarSystemGenerator::MakePlanet(const FPlanetMetaSettings& settings)
{
	return FPlanetSettings{
				settings.icoSphereIterations,
				MakeNoise(settings.BaseNoise),
				MakeNoise(settings.ContinentNoise),
				FMath::FRandRange(settings.SeaLevel.X,settings.SeaLevel.Y),
				FMath::FRandRange(settings.oceanDepth.X,settings.oceanDepth.Y),
				FMath::FRandRange(settings.oceanDepthMultiplier.X,settings.oceanDepthMultiplier.Y),
				FMath::FRandRange(settings.oceanSmoothing.X,settings.oceanSmoothing.Y),
				FMath::FRandRange(settings.landBias.X,settings.landBias.Y)
	};
}

FNoiseSettings U_JR_SolarSystemGenerator::MakeNoise(const FNoiseMetaSettings& settings)
{
	return FNoiseSettings{
					FMath::FRandRange(settings.Octaves.X,settings.Octaves.Y),
					FMath::FRandRange(settings.Persistence.X,settings.Persistence.Y),
					FMath::FRandRange(settings.Scale.X,settings.Scale.Y),
					FMath::FRandRange(settings.Strength.X,settings.Strength.Y)
	};
}

FStarSettings U_JR_SolarSystemGenerator::MakeStar(const FStarMetaSettings& settings)
{
	return FStarSettings{
		FMath::FRandRange(settings.size.X,settings.size.Y),
		FMath::FRandRange(settings.brightness.X,settings.brightness.Y),
		FMath::FRandRange(settings.mass.X,settings.mass.Y),
		MakeColourHSV(settings.hue1,settings.sat1,settings.val1),
		MakeColourHSV(settings.hue2,settings.sat2,settings.val2)
	};
}

FLinearColor U_JR_SolarSystemGenerator::MakeColourHSV(const FVector2f& hue, const FVector2f& sat, const FVector2f& val)
{
	uint8 h = FMath::Floor(FMath::FRandRange(hue.X, hue.Y) * (255.9999f/360.0f));
	uint8 s = FMath::Floor(FMath::FRandRange(sat.X, sat.Y) * (255.9999f/1.0f));
	uint8 v = FMath::Floor(FMath::FRandRange(hue.X, hue.Y) * (255.9999f/1.0f));
	return FLinearColor::MakeFromHSV8(h,s,v);
}
