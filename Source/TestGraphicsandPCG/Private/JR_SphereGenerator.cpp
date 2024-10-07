// Fill out your copyright notice in the Description page of Project Settings.


#include "JR_SphereGenerator.h"
#include "GeometryScript/GeometryScriptTypes.h"
#include "UDynamicMesh.h"
#include "GeometryScriptingCore/Public/GeometryScript/MeshPrimitiveFunctions.h"
#include "GeometryScriptingCore/Public/GeometryScript/MeshTransformFunctions.h"
#include "GeometryScriptingCore/Public/GeometryScript/MeshNormalsFunctions.h"
#include "DynamicMesh/MeshAttributeUtil.h"
FGeometryScriptVectorList UJR_SphereGenerator::Sphere()
{
	FGeometryScriptVectorList* output = new FGeometryScriptVectorList();
	//TArray<FVector>& List = VectorList.List.ToSharedRef().Get();
	output->Reset();
	output->List->Reserve(8);
	output->List->Emplace(FVector(0, 0, 0));
	output->List->Emplace(FVector(1, 0, 0));
	output->List->Emplace(FVector(0, 1, 0));
	output->List->Emplace(FVector(1, 1, 0));
	output->List->Emplace(FVector(0, 0, 1));
	output->List->Emplace(FVector(1, 0, 1));
	output->List->Emplace(FVector(0, 1, 1));
	output->List->Emplace(FVector(1, 1, 1));
	return *output;
}

double UJR_SphereGenerator::Number()
{
	return 1.0f;
}

UDynamicMesh* UJR_SphereGenerator::NormalisePoints(UDynamicMesh* meshIn)
{
	meshIn->EditMesh([&](FDynamicMesh3& EditMesh)
		{
			
			for (int32 VertexID : EditMesh.VertexIndicesItr())
			{
				FVector NewVertex = EditMesh.GetVertex(VertexID);
				NewVertex /= sqrt(NewVertex.X * NewVertex.X + NewVertex.Y * NewVertex.Y + NewVertex.Z * NewVertex.Z);
				EditMesh.SetVertex(VertexID, NewVertex);
			}
			
		}, EDynamicMeshChangeType::GeneralEdit, EDynamicMeshAttributeChangeFlags::Unknown, false);
	return meshIn;
}

UDynamicMesh* UJR_SphereGenerator::AppendCubeSphere(UDynamicMesh* TargetMesh,
	FGeometryScriptPrimitiveOptions PrimitiveOptions,
	FTransform Transform,
	int32 Steps,
	UGeometryScriptDebug* Debug)
{
	FTransform t2 = FTransform();
	UDynamicMesh* meshIn = UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox(TargetMesh, PrimitiveOptions, t2, 2, 2, 2, Steps, Steps, Steps, EGeometryScriptPrimitiveOriginMode::Center, Debug);
	meshIn->EditMesh([&](FDynamicMesh3& EditMesh)
		{

			for (int32 VertexID : EditMesh.VertexIndicesItr())
			{
				FVector p = EditMesh.GetVertex(VertexID);
				//p.X *= sqrt(1 - ((p.Y * p.Y) / 2) - ((p.Z * p.Z) / 2) + ((p.Y * p.Y * p.Z * p.Z) / 2));
				//p.Y *= sqrt(1 - ((p.Z * p.Z) / 2) - ((p.X * p.X) / 2) + ((p.X * p.X * p.Z * p.Z) / 2));
				//p.X *= sqrt(1 - ((p.X * p.X) / 2) - ((p.Y * p.Y) / 2) + ((p.Y * p.Y * p.X * p.X) / 2));
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
	meshIn = UGeometryScriptLibrary_MeshTransformFunctions::TransformMesh(meshIn, Transform, true);
	FGeometryScriptCalculateNormalsOptions normOptions;
	meshIn = UGeometryScriptLibrary_MeshNormalsFunctions::RecomputeNormals(meshIn,normOptions);
	return meshIn;
}
UDynamicMesh* UJR_SphereGenerator::PlaneToSphere(UDynamicMesh* meshIn, double sphereRadius)
{
	//FOR NOW: planes are always in XY
	meshIn->EditMesh([&](FDynamicMesh3& EditMesh)
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
	return meshIn;
}
void UJR_SphereGenerator::Unused(FGeometryScriptVectorList& VectorIn)
{
	if (!VectorIn.List.IsValid()) VectorIn.Reset();
	for (auto pos = VectorIn.List->begin(); pos != VectorIn.List->end();++pos) {
		(*pos).Normalize();
	}
}

