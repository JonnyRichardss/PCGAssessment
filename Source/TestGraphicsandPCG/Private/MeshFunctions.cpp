// Fill out your copyright notice in the Description page of Project Settings.


#include "MeshFunctions.h"
#include "GeometryScript/GeometryScriptTypes.h"
#include "UDynamicMesh.h"
#include "GeometryScriptingCore/Public/GeometryScript/MeshPrimitiveFunctions.h"
#include "GeometryScriptingCore/Public/GeometryScript/MeshTransformFunctions.h"
#include "GeometryScriptingCore/Public/GeometryScript/MeshNormalsFunctions.h"
#include "DynamicMesh/MeshAttributeUtil.h"

#include "GeneratorSettings.h"
#include "PerlinNoise.hpp"
#include "Kismet/KismetSystemLibrary.h"

//

static siv::PerlinNoise Perlin()
{

	const siv::PerlinNoise::seed_type seed = FMath::Rand();
	const siv::PerlinNoise perlin{seed};

	return perlin;
}

UDynamicMesh* U_JR_MeshFunctions::AppendCubeSphere(UDynamicMesh* TargetMesh,FGeometryScriptPrimitiveOptions PrimitiveOptions,FTransform Transform,int32 Steps,UGeometryScriptDebug* Debug)
{
	FTransform IdentityTransform = FTransform();
	TargetMesh = UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox(TargetMesh, PrimitiveOptions, IdentityTransform, 2, 2, 2, Steps, Steps, Steps, EGeometryScriptPrimitiveOriginMode::Center, Debug);
	TargetMesh->EditMesh([&](FDynamicMesh3& EditMesh)
		{
			for (int32 VertexID : EditMesh.VertexIndicesItr())
			{
				//https://mathproofs.blogspot.com/2005/07/mapping-cube-to-sphere.html
				FVector p = EditMesh.GetVertex(VertexID);
				float x2 = p.X * p.X;
				float y2 = p.Y * p.Y;
				float z2 = p.Z * p.Z;
				float x = p.X * sqrt(1 - (y2 + z2) / 2 + (y2 * z2) / 3);
				float y = p.Y * sqrt(1 - (x2 + z2) / 2 + (x2 * z2) / 3);
				float z = p.Z * sqrt(1 - (y2 + x2) / 2 + (y2 * x2) / 3);
				p.X = x;
				p.Y = y;
				p.Z = z;
				EditMesh.SetVertex(VertexID, p);
			}

		}, EDynamicMeshChangeType::GeneralEdit, EDynamicMeshAttributeChangeFlags::Unknown, false);
	TargetMesh = UGeometryScriptLibrary_MeshTransformFunctions::TransformMesh(TargetMesh, Transform, true);
	FGeometryScriptCalculateNormalsOptions normOptions;
	TargetMesh = UGeometryScriptLibrary_MeshNormalsFunctions::RecomputeNormals(TargetMesh,normOptions);
	return TargetMesh;
}
UPARAM(DisplayName = "Target Mesh")UDynamicMesh* U_JR_MeshFunctions::AppendIcoSphere(UDynamicMesh* TargetMesh, FTransform Transform, int32 iterations)
{
	//http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html
	double t = (1.0 + FMath::Sqrt(5.0)) / 2.0;
	
	//for some unkown reason EditMesh() doesnt work for this and nor does GetMeshRef()
	auto EditMesh = TargetMesh->GetMeshPtr();
	//inital vertices
	EditMesh->AppendVertex(FVector3d(-1, t , 0));
	EditMesh->AppendVertex(FVector3d(1 , t , 0));
	EditMesh->AppendVertex(FVector3d(-1, -t, 0));
	EditMesh->AppendVertex(FVector3d(1 , -t, 0));
					
	EditMesh->AppendVertex(FVector3d(0,-1,t ));
	EditMesh->AppendVertex(FVector3d(0,1 ,t ));
	EditMesh->AppendVertex(FVector3d(0,-1,-t));
	EditMesh->AppendVertex(FVector3d(0,1 ,-t));
					
	EditMesh->AppendVertex(FVector3d(t, 0, -1));
	EditMesh->AppendVertex(FVector3d(t,0,1));
	EditMesh->AppendVertex(FVector3d(-t,0,-1));
	EditMesh->AppendVertex(FVector3d(-t,0,1));


	//initial faces
	EditMesh->AppendTriangle(0, 11, 5);
	EditMesh->AppendTriangle(0, 5, 1);
	EditMesh->AppendTriangle(0, 1, 7);
	EditMesh->AppendTriangle(0, 7, 10);
	EditMesh->AppendTriangle(0, 10, 11);
					
	EditMesh->AppendTriangle(1, 5, 9);
	EditMesh->AppendTriangle(5, 11, 4);
	EditMesh->AppendTriangle(11, 10, 2);
	EditMesh->AppendTriangle(10, 7, 6);
	EditMesh->AppendTriangle(7, 1, 8);
					
	EditMesh->AppendTriangle(3, 9, 4);
	EditMesh->AppendTriangle(3, 4, 2);
	EditMesh->AppendTriangle(3, 2, 6);
	EditMesh->AppendTriangle(3, 6, 8);
	EditMesh->AppendTriangle(3, 8, 9);
					
	EditMesh->AppendTriangle(4, 9, 5);
	EditMesh->AppendTriangle(2, 4, 11);
	EditMesh->AppendTriangle(6, 2, 10);
	EditMesh->AppendTriangle(8, 6, 7);
	EditMesh->AppendTriangle(9, 8, 1);
			
	//Refine sphere
	for (int j = 0; j < iterations; j++)
	{
		TArray<FIntVector3> trisToAdd;
		TArray<int> trisToRemove;
		//calculate new faces
		for (int i=0;i<EditMesh->TriangleCount();i++)
		{
			auto tri = EditMesh->GetTriangle(i);
			//tri indices for new points between old ones
			int a = tri.A;
			int b = tri.B;
			int c = tri.C;
			int d = AppendMidPoint(a, b,EditMesh);
			int e = AppendMidPoint(b, c,EditMesh);
			int f = AppendMidPoint(c, a,EditMesh);
			trisToRemove.Add(i);
			//add new tris
			trisToAdd.Add(FIntVector3(a, d, f));
			trisToAdd.Add(FIntVector3(b, e, d));
			trisToAdd.Add(FIntVector3(c, f, e));
			trisToAdd.Add(FIntVector3(d, e, f));		
		}
		//remove old faces
		for (int tri : trisToRemove) {
			if (EditMesh->IsTriangle(tri)) {
				EditMesh->RemoveTriangle(tri, false, false);
			}
			else {
				int asda = 2;
			}
		}
		//re-add new faces
		for (auto tri : trisToAdd) {
			EditMesh->AppendTriangle(tri.X, tri.Y, tri.Z);
		}
	}


	//normalise all vertices
	for (int i = 0; i < EditMesh->VertexCount(); i++) {
		auto v = EditMesh->GetVertex(i);
		v.Normalize();
		EditMesh->SetVertex(i, v);
	}

	TargetMesh = UGeometryScriptLibrary_MeshNormalsFunctions::FlipNormals(TargetMesh);
	TargetMesh = UGeometryScriptLibrary_MeshTransformFunctions::TransformMesh(TargetMesh, Transform);
	return TargetMesh;
}
UDynamicMesh* U_JR_MeshFunctions::WrapPlaneToSphere(UDynamicMesh* TargetMesh, double sphereRadius)
{
	//FOR NOW: planes are always in XY
	TargetMesh->EditMesh([&](FDynamicMesh3& EditMesh)
		{
			auto test = EditMesh.GetBounds();
			double maxLong = FMath::Max(abs(test.Min.Y), abs(test.Max.Y));
			double maxLat = FMath::Max(abs(test.Min.X), abs(test.Max.X));
	
			for (int32 VertexID : EditMesh.VertexIndicesItr())
			{
				FVector p = EditMesh.GetVertex(VertexID);
				double longitude = (p.Y * PI) / maxLong;
				double latitude = (p.X * PI) / maxLat;
				double z = FMath::Sin(latitude);
				double r = FMath::Cos(latitude);
				double x = FMath::Sin(longitude) * r;
				double y = FMath::Cos(longitude) * r;
				FVector q = FVector(x, y, z) * sphereRadius;
;				EditMesh.SetVertex(VertexID, q);
			}
			UE::Geometry::CopyVertexUVsToOverlay(EditMesh, *EditMesh.Attributes()->PrimaryUV());
			
		}, EDynamicMeshChangeType::GeneralEdit, EDynamicMeshAttributeChangeFlags::Unknown, false);
	return TargetMesh;
}

UPARAM(DisplayName = "Target Mesh")UDynamicMesh* U_JR_MeshFunctions::AppendAsteroid(UDynamicMesh* TargetMesh, FAsteroidSettings settings, FTransform Transform)
{
	TargetMesh = AppendIcoSphere(TargetMesh, FTransform::Identity, settings.icoSphereIterations);
	TargetMesh = AddAsteroidCraters(TargetMesh, settings);
	TargetMesh = UGeometryScriptLibrary_MeshNormalsFunctions::RecomputeNormals(TargetMesh, FGeometryScriptCalculateNormalsOptions());
	TargetMesh = UGeometryScriptLibrary_MeshTransformFunctions::TransformMesh(TargetMesh, Transform);
	return TargetMesh;
}


UPARAM(DisplayName = "Target Mesh")UDynamicMesh* U_JR_MeshFunctions::AppendIceBall(UDynamicMesh* TargetMesh, FIceBallSettings settings, FTransform Transform)
{
	TargetMesh = AppendIcoSphere(TargetMesh, FTransform::Identity, settings.icoSphereIterations);
	auto perlin = Perlin();
	TargetMesh->EditMesh([&](FDynamicMesh3& EditMesh)
		{
			for (int32 VertexID : EditMesh.VertexIndicesItr()) {
				FVector v = EditMesh.GetVertex(VertexID);

				FVector baseV = v;
				baseV *= settings.BaseNoise.Scale;
				float baseHeight = perlin.octave3D_11(baseV.X, baseV.Y, baseV.Z, settings.BaseNoise.Octaves, settings.BaseNoise.Persistence);
				baseHeight *= settings.BaseNoise.Strength;
				//translate
				v = TranslateAlongWorldCoords(v, baseHeight);

				EditMesh.SetVertex(VertexID, v);
			}

		}, EDynamicMeshChangeType::GeneralEdit, EDynamicMeshAttributeChangeFlags::Unknown, false);
	TargetMesh = UGeometryScriptLibrary_MeshNormalsFunctions::RecomputeNormals(TargetMesh, FGeometryScriptCalculateNormalsOptions());
	TargetMesh = UGeometryScriptLibrary_MeshTransformFunctions::TransformMesh(TargetMesh, Transform);
	return TargetMesh;
}

static bool CopyVertexColorsToOverlay(const FDynamicMesh3& Mesh, UE::Geometry::FDynamicMeshColorOverlay& overlayOut) {
	if (!Mesh.HasVertexColors()) 
	{
		return false;
	}
	if (overlayOut.ElementCount() > 0) {
		overlayOut.ClearElements();
	}
	overlayOut.BeginUnsafeElementsInsert();
	for (int32 VertexID : Mesh.VertexIndicesItr()) {
		FVector3f c = Mesh.GetVertexColor(VertexID);
		FVector4f color = FVector4f(c.X, c.Y, c.Z, 1.0f);
		overlayOut.InsertElement(VertexID, &color.X, true);
	}
	overlayOut.EndUnsafeElementsInsert();
	for (int32 TriangleID : Mesh.TriangleIndicesItr()) {
		overlayOut.SetTriangle(TriangleID, Mesh.GetTriangle(TriangleID));
	}
	return true;
}
UPARAM(DisplayName = "Target Mesh")UDynamicMesh* U_JR_MeshFunctions::AppendEarthPlanet(UDynamicMesh* TargetMesh, FPlanetSettings settings, FTransform Transform, FVector3f BaseColor, FVector3f HighColor)
{
	TargetMesh = AppendIcoSphere(TargetMesh, FTransform::Identity, settings.icoSphereIterations);
	TargetMesh = EarthPlanetDeformation(TargetMesh, settings, BaseColor,HighColor);
	TargetMesh = UGeometryScriptLibrary_MeshNormalsFunctions::RecomputeNormals(TargetMesh, FGeometryScriptCalculateNormalsOptions());
	TargetMesh = UGeometryScriptLibrary_MeshTransformFunctions::TransformMesh(TargetMesh, Transform);
	return TargetMesh;
}
UPARAM(DisplayName = "Target Mesh")UDynamicMesh* U_JR_MeshFunctions::AppendRockPlanet(UDynamicMesh* TargetMesh, FPlanetSettings settings, FTransform Transform, FVector3f BaseColor, FVector3f HighColor)
{
	TargetMesh = AppendIcoSphere(TargetMesh, FTransform::Identity, settings.icoSphereIterations);
	TargetMesh = RockPlanetDeformation(TargetMesh, settings, BaseColor, HighColor);
	TargetMesh = UGeometryScriptLibrary_MeshNormalsFunctions::RecomputeNormals(TargetMesh, FGeometryScriptCalculateNormalsOptions());
	TargetMesh = UGeometryScriptLibrary_MeshTransformFunctions::TransformMesh(TargetMesh, Transform);
	return TargetMesh;
}
UDynamicMesh* U_JR_MeshFunctions::EarthPlanetDeformation(UDynamicMesh* TargetMesh, FPlanetSettings settings,FVector3f BaseColor,FVector3f HighColor)
{
	float minHeight = MAX_flt;
	float maxHeight = MIN_flt;
	auto perlin = Perlin();
	TargetMesh->EditMesh([&](FDynamicMesh3& EditMesh)
		{
			EditMesh.EnableVertexColors(BaseColor);

			for (int32 VertexID : EditMesh.VertexIndicesItr()) {
				FVector v = EditMesh.GetVertex(VertexID);
				FVector3f c = EditMesh.GetVertexColor(VertexID);

				FVector baseV = v;
				baseV *= settings.BaseNoise.Scale;
				float baseHeight = perlin.octave3D_11(baseV.X, baseV.Y, baseV.Z, settings.BaseNoise.Octaves, settings.BaseNoise.Persistence);
				baseHeight *= settings.BaseNoise.Strength;

				FVector contV = v;
				contV *= settings.ContinentNoise.Scale;
				float continentHeight = perlin.octave3D_11(contV.X, contV.Y, contV.Z, settings.ContinentNoise.Octaves, settings.ContinentNoise.Persistence);
				continentHeight += settings.landBias;
				continentHeight = smax(continentHeight, -settings.oceanDepth, settings.oceanSmoothing);

				if (continentHeight < 0) {
					continentHeight *= 1 + settings.oceanDepthMultiplier;
				}
				continentHeight *= settings.ContinentNoise.Strength;
				float finalHeight = baseHeight + continentHeight;


				//translate
				v = TranslateAlongWorldCoords(v, finalHeight);

				minHeight = finalHeight < minHeight ? finalHeight : minHeight;
				maxHeight = finalHeight > maxHeight ? finalHeight : maxHeight;

				if (finalHeight <= settings.SeaLevel) {
					c = BaseColor;
				}
				else {
					c = HighColor;
				}
				//c = FMath::Lerp(BaseColor, HighColor, finalHeight);

				EditMesh.SetVertex(VertexID, v);
				EditMesh.SetVertexColor(VertexID, c);
			}
			EditMesh.Attributes()->EnablePrimaryColors();
			CopyVertexColorsToOverlay(EditMesh, *(EditMesh.Attributes()->PrimaryColors()));

			//UKismetSystemLibrary::PrintString(TargetMesh, FString::Printf(TEXT("Planet height displacement : \n   min : % f\n   max : %f"), minHeight, maxHeight));

		}, EDynamicMeshChangeType::GeneralEdit, EDynamicMeshAttributeChangeFlags::Unknown, false);
	return TargetMesh;
}
UDynamicMesh* U_JR_MeshFunctions::RockPlanetDeformation(UDynamicMesh* TargetMesh, FPlanetSettings settings,FVector3f BaseColor,FVector3f HighColor)
{
	float minHeight = MAX_flt;
	float maxHeight = MIN_flt;
	auto perlin = Perlin();
	TargetMesh->EditMesh([&](FDynamicMesh3& EditMesh)
		{
			EditMesh.EnableVertexColors(BaseColor);
			TArray<float> heights;
			heights.Reserve(EditMesh.VertexCount());
			for (int32 VertexID : EditMesh.VertexIndicesItr()) {
				FVector v = EditMesh.GetVertex(VertexID);

				FVector baseV = v;
				baseV *= settings.BaseNoise.Scale;
				float baseHeight = perlin.octave3D_11(baseV.X, baseV.Y, baseV.Z, settings.BaseNoise.Octaves, settings.BaseNoise.Persistence);
				baseHeight *= settings.BaseNoise.Strength;

				FVector contV = v;
				contV *= settings.ContinentNoise.Scale;
				float continentHeight = perlin.octave3D_11(contV.X, contV.Y, contV.Z, settings.ContinentNoise.Octaves, settings.ContinentNoise.Persistence);
				continentHeight += settings.landBias;
				continentHeight = smax(continentHeight, -settings.oceanDepth, settings.oceanSmoothing);

				if (continentHeight < 0) {
					continentHeight *= 1 + settings.oceanDepthMultiplier;
				}
				continentHeight *= settings.ContinentNoise.Strength;
				float finalHeight = baseHeight + continentHeight;
				heights.Add(finalHeight);

				//translate
				v = TranslateAlongWorldCoords(v, finalHeight);

				minHeight = finalHeight < minHeight ? finalHeight : minHeight;
				maxHeight = finalHeight > maxHeight ? finalHeight : maxHeight;

				EditMesh.SetVertex(VertexID, v);
			}
			float range = maxHeight - minHeight;
			int i = 0;
			float minHeight2 = MAX_flt;
			float maxHeight2 = MIN_flt;
			for (int32 VertexID : EditMesh.VertexIndicesItr()) {
				
				FVector3f c = EditMesh.GetVertexColor(VertexID);
				float height = heights[i];
				height -= minHeight;
				height /= range;
				height -= .5;
				height *= 1.5;
				c = FMath::Lerp(BaseColor, HighColor,height);

				minHeight2 = height < minHeight2 ? height : minHeight2;
				maxHeight2 = height > maxHeight2 ? height : maxHeight2;

				EditMesh.SetVertexColor(VertexID, c);
				i++;
			}
			EditMesh.Attributes()->EnablePrimaryColors();
			CopyVertexColorsToOverlay(EditMesh, *(EditMesh.Attributes()->PrimaryColors()));

			//UKismetSystemLibrary::PrintString(TargetMesh, FString::Printf(TEXT("Planet height displacement : \n   min : % f\n   max : %f"), minHeight2, maxHeight2));

		}, EDynamicMeshChangeType::GeneralEdit, EDynamicMeshAttributeChangeFlags::Unknown, false);
	return TargetMesh;
}

UDynamicMesh* U_JR_MeshFunctions::AddAsteroidCraters(UDynamicMesh* TargetMesh, FAsteroidSettings settings)
{
	TArray<Crater> craters = MakeCraters(settings);
	auto perlin = Perlin();
	TargetMesh->EditMesh([&](FDynamicMesh3& EditMesh) 
		{
				
			for (int32 VertexID : EditMesh.VertexIndicesItr()) {
				FVector v = EditMesh.GetVertex(VertexID);
				//calculate crater height for given position
				float craterHeight = CalculateCraterHeight(v,craters,settings) * settings.craterMult;
				float noiseHeight = perlin.octave3D(v.X, v.Y, v.Z, settings.BaseNoise.Octaves, settings.BaseNoise.Persistence) * settings.BaseNoise.Strength;

				float finalHeight = craterHeight + noiseHeight;
				//translate vector by crater height
				v = TranslateAlongWorldCoords(v, finalHeight);
				EditMesh.SetVertex(VertexID, v);
			}
		},EDynamicMeshChangeType::GeneralEdit, EDynamicMeshAttributeChangeFlags::Unknown, false);
	return TargetMesh;
}


FVector U_JR_MeshFunctions::TranslateAlongWorldCoords(FVector pos, float distance)
{
	FVector dir = pos;
	dir.Normalize();
	pos += dir * distance;
	return pos;
}

float U_JR_MeshFunctions::CalculateCraterHeight(FVector pos, const TArray<Crater>& craters,FAsteroidSettings settings)
{
	float output = 0.0f;
	for (const Crater& c : craters) {
		pos.Normalize();
		float x = (pos - c.centre).Length() / FMath::Max(c.size, 0.001);
		float cavity = x * x - 1;
		float rimX = FMath::Min(x - 1 - settings.rimWidth, 0);
		float rim = settings.rimSteepness * rimX * rimX;

		float craterShape = smax(cavity, c.floorHeight, c.smoothness);
		craterShape = smin(craterShape, rim, c.smoothness);
		output += craterShape * c.size;
	}
	return output;
}

TArray<U_JR_MeshFunctions::Crater> U_JR_MeshFunctions::MakeCraters(FAsteroidSettings settings)
{
	TArray<Crater> output;
	output.Reserve(settings.numCraters);
	for (int i = 0; i < settings.numCraters; i++) {
		double fac = LowBiasedRNG(settings.sizeBias);
		//construct crater and add to output
		//i just learned about emplace VS add (pushback in vector land) and its cool
		output.Emplace(
			FMath::VRand(), //centre
			FMath::Lerp(settings.sizeMinMax.X, settings.sizeMinMax.Y, fac), //size
			FMath::Lerp(settings.floorHeightMinMax.X,settings.floorHeightMinMax.Y,fac + LowBiasedRNG(0.3)), //floorHeight
			FMath::Lerp(settings.smoothnessMinMax.X,settings.smoothnessMinMax.Y,1 - fac) //smoothness
			);
	}
	return output;
}

float U_JR_MeshFunctions::HighBiasedRNG(float bias) {
	return 1 - LowBiasedRNG(bias);
}
float U_JR_MeshFunctions::LowBiasedRNG(float bias)
{
	float t = FMath::FRand();

	// Avoid possible division by zero
	if (bias == 1) {
		return 0;
	}

	// Remap strength for nicer input -> output relationship
	float k = FMath::Clamp(1 - bias,0,1);
	k = k * k * k - 1;

	// Thanks to www.shadertoy.com/view/Xd2yRd
	return FMath::Clamp((t + t * k) / (t * k + 1),0,1);
}

float U_JR_MeshFunctions::smin(float a, float b, float k)
{
	float h = FMath::Clamp(0.5 + 0.5 * (b - a) / k, 0.0, 1.0);
	return FMath::Lerp(b, a, h) - k * h * (1.0 - h);
}
float U_JR_MeshFunctions::smax(float a, float b, float k)
{
	k = 0 - k;
	float h = FMath::Clamp(0.5 + 0.5 * (b - a) / k, 0.0, 1.0);
	return FMath::Lerp(b, a, h) - k * h * (1.0 - h);
}

int U_JR_MeshFunctions::AppendMidPoint(int a, int b, FDynamicMesh3* EditMesh)
{
	auto v1 = EditMesh->GetVertex(a);
	auto v2 = EditMesh->GetVertex(b);
	FVector3d mid = (v1 + v2) / 2;

	mid.Normalize();
	//mid *= (1.0 + FMath::Sqrt(5.0)) / 2.0;
	return EditMesh->AppendVertex(mid);
}


/*
///TODO: This code  from SebLague
/// It can bias RNGs which he uses for crater generator



/// Random value [0, 1]
	/// Output is increasingly biased toward 1 as biasStrength goes from 0 to 1
	public float ValueBiasUpper (float biasStrength) {
		return 1 - ValueBiasLower (biasStrength);
	}

	/// Random value [0, 1]
	/// Output is increasingly biased toward 0 as biasStrength goes from 0 to 1
	public float ValueBiasLower (float biasStrength) {
		float t = Value ();

		// Avoid possible division by zero
		if (biasStrength == 1) {
			return 0;
		}

		// Remap strength for nicer input -> output relationship
		float k = Mathf.Clamp01 (1 - biasStrength);
		k = k * k * k - 1;

		// Thanks to www.shadertoy.com/view/Xd2yRd
		return Mathf.Clamp01 ((t + t * k) / (t * k + 1));
	}



*/
