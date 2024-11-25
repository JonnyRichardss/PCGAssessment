// Fill out your copyright notice in the Description page of Project Settings.


#include "JR_SphereGenerator.h"
#include "GeometryScript/GeometryScriptTypes.h"
#include "UDynamicMesh.h"
#include "GeometryScriptingCore/Public/GeometryScript/MeshPrimitiveFunctions.h"
#include "GeometryScriptingCore/Public/GeometryScript/MeshTransformFunctions.h"
#include "GeometryScriptingCore/Public/GeometryScript/MeshNormalsFunctions.h"
#include "DynamicMesh/MeshAttributeUtil.h"
#include "FIcoSphereGenerator.h"
#include "CraterSettings.h"


UDynamicMesh* UJR_SphereGenerator::AppendCubeSphere(UDynamicMesh* TargetMesh,FGeometryScriptPrimitiveOptions PrimitiveOptions,FTransform Transform,int32 Steps,UGeometryScriptDebug* Debug)
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
UPARAM(DisplayName = "Target Mesh")UDynamicMesh* UJR_SphereGenerator::AppendIcoSphere(UDynamicMesh* TargetMesh, FTransform Transform, int32 iterations, UGeometryScriptDebug* Debug)
{
	int tesco=0;
	//http://blog.andreaskahler.com/2009/06/creating-icosphere-mesh-in-code.html
	double t = (1.0 + FMath::Sqrt(5.0)) / 2.0;
	

	auto EditMesh = TargetMesh->GetMeshPtr();
		{

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
			
			
			for (int j = 0; j < iterations; j++)
			{
				TArray<FIntVector3> trisToAdd;
				TArray<int> trisToRemove;
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
				for (int tri : trisToRemove) {
					if (EditMesh->IsTriangle(tri)) {
						EditMesh->RemoveTriangle(tri, false, false);
					}
					else {
						int asda = 2;
					}
				}
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
			

		}
	
		TargetMesh = UGeometryScriptLibrary_MeshNormalsFunctions::FlipNormals(TargetMesh);
	return TargetMesh;
}
UDynamicMesh* UJR_SphereGenerator::WrapPlaneToSphere(UDynamicMesh* TargetMesh, double sphereRadius)
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

UPARAM(DisplayName = "Target Mesh")UDynamicMesh* UJR_SphereGenerator::AddCratersToSphere(UDynamicMesh* TargetMesh, FCraterSettings settings)
{
	TArray<Crater> craters = MakeCraters(settings);
	TargetMesh->EditMesh([&](FDynamicMesh3& EditMesh) 
		{
			for (int32 VertexID : EditMesh.VertexIndicesItr()) {
				FVector v = EditMesh.GetVertex(VertexID);
				//calculate crater height for given position
				float craterHeight = CalculateCraterHeight(v,craters,settings);
				//translate vector by crater height
				v = TranslateAlongWorldCoords(v, craterHeight);
				EditMesh.SetVertex(VertexID, v);
			}
		},EDynamicMeshChangeType::GeneralEdit, EDynamicMeshAttributeChangeFlags::Unknown, false);
	return TargetMesh;
}

FVector UJR_SphereGenerator::TranslateAlongWorldCoords(FVector pos, float distance)
{
	FVector dir = pos;
	dir.Normalize();
	pos += dir * distance;
	return pos;
}

float UJR_SphereGenerator::CalculateCraterHeight(FVector pos, const TArray<Crater>& craters,FCraterSettings settings)
{
	float output = 0.0f;
	for (Crater c : craters) {
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

TArray<UJR_SphereGenerator::Crater> UJR_SphereGenerator::MakeCraters(FCraterSettings settings)
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

float UJR_SphereGenerator::HighBiasedRNG(float bias) {
	return 1 - LowBiasedRNG(bias);
}
float UJR_SphereGenerator::LowBiasedRNG(float bias)
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

float UJR_SphereGenerator::smin(float a, float b, float k)
{
	float h = FMath::Clamp(0.5 + 0.5 * (b - a) / k, 0.0, 1.0);
	return FMath::Lerp(b, a, h) - k * h * (1.0 - h);
}
float UJR_SphereGenerator::smax(float a, float b, float k)
{
	k = 0 - k;
	float h = FMath::Clamp(0.5 + 0.5 * (b - a) / k, 0.0, 1.0);
	return FMath::Lerp(b, a, h) - k * h * (1.0 - h);
}

int UJR_SphereGenerator::AppendMidPoint(int a, int b, FDynamicMesh3* EditMesh)
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
