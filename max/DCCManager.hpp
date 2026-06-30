// ===========================================================================
// Copyright (c) 2024 Autodesk, Inc. All rights reserved.
//
// Use of this software is subject to the terms of the Autodesk license
// agreement provided at the time of installation or download, or which
// otherwise accompanies this software in either electronic or hard copy form.
// ===========================================================================
//-
// ==========================================================================
//
// ==========================================================================
//+

#pragma once
#include "preinclude.hpp"

#include <ios>
#include <regex>

#include "maxtypes.h"
#include "Max.h"
#include "istdplug.h"
#include "stdmat.h"
#include "decomp.h"
#include "shape.h"
#include "splshape.h"
#include "dummy.h"
#include "simpobj.h"
#include "polyobj.h"
#include "mesh.h"
#include "MeshNormalSpec.h"
#include "maxstring.h"
#include "maxscript/foundation/numbers.h"
#include "modstack.h"
#include "iunwrap.h"
#include "meshdelta.h"
#include "MNMesh.h"
#include "mnmath.h"
#include "mncommon.h"

#include <QTreeWidgetItem>
#include <DirectXMath.h>
#include "maxutils.h"

#include "granny.h"

#define FT_MAX_MAJOR_VERSION    2
#define FT_MAX_MINOR_VERSION    7
#define FT_MAX_PATCH_VERSION    1
#define FT_MAX_BUILD_NUMBER     0
#define FT_MAX_VERSION_STR      "2.7.1"
#define FT_MAX_VERSION_NUMBER   271
#define FT_MAX_SCM_BRANCH       "3ds Max"
#define FT_MAX_SCM_TAGS         "release version-2"
#define FT_MAX_SCM_DATETIME     "2026-06-30T00:00:00.000Z"

namespace fmnext
{
	struct MaterialInstace
	{
		MaterialInstace() = default;
		MaterialInstace(std::string& path, std::shared_ptr<BundleReader::BundleData> local, std::shared_ptr<BundleReader::BundleData> instance) : path(path), local(local), instace(instance) {}

		std::string path;
		std::shared_ptr<BundleReader::BundleData> local;
		std::shared_ptr<BundleReader::BundleData> instace;
	};

	struct ModelItem
	{
		ModelItem() = default;
		ModelItem(QTreeWidgetItem* item, uint32_t type, std::shared_ptr<SceneReader::CarRenderModel11> model, std::shared_ptr<BundleReader::BundleData> bundle, std::unordered_map<int32_t, MaterialInstace> materials, const std::string& schema) : item(item), type(type), model(model), bundle(bundle), materials(materials), schema(schema) {}

		QTreeWidgetItem* item;
		uint32_t type;
		std::shared_ptr<SceneReader::CarRenderModel11> model;
		std::shared_ptr<BundleReader::BundleData> bundle;
		std::unordered_map<int32_t, MaterialInstace> materials;
		std::string schema;
	};
}

class DCCManager
{
public:
	DCCManager() = default;

	~DCCManager()
	{
	}

	static Matrix3 BuildMatrix3(Point3 pos, float rotx, float roty, float rotz, Point3 scale = Point3(1, 1, 1))
	{
		Matrix3 tm = ScaleMatrix(scale);
		tm.RotateX(rotx);
		tm.RotateY(roty);
		tm.RotateZ(rotz);
		tm.Translate(pos);
		return tm;
	}

	static Matrix3 BuildMatrix3_(Point3 pos, float rotx, float roty, float rotz, Point3 scale = Point3(1, 1, 1))
	{
		Quat quat;
		quat.SetEuler(rotx, roty, rotz);
		Matrix3 tm;
		tm.SetRotate(quat);
		tm.PreScale(scale);
		tm.Translate(pos);
		return tm;
	}

	static Matrix3 GetMatrix3(const DirectX::XMMATRIX& dxMatrix)
	{
		Matrix3 nodeTM = {};

		DirectX::XMVECTOR outScale, outRotQuat, outTrans;
		DirectX::XMMatrixDecompose(&outScale, &outRotQuat, &outTrans, dxMatrix);

		Point3 position = Point3(DirectX::XMVectorGetX(outTrans), DirectX::XMVectorGetZ(outTrans), DirectX::XMVectorGetY(outTrans));
		Quat rotation = Quat(DirectX::XMVectorGetX(outRotQuat), DirectX::XMVectorGetZ(outRotQuat), DirectX::XMVectorGetY(outRotQuat), DirectX::XMVectorGetW(outRotQuat));
		Point3 scale = Point3(DirectX::XMVectorGetX(outScale), DirectX::XMVectorGetZ(outScale), DirectX::XMVectorGetY(outScale));

		//Quat rotation_inv = rotation.Inverse();

		nodeTM.SetRotate(reinterpret_cast<const Quat&>(rotation));
		nodeTM.SetTrans(reinterpret_cast<const Point3&>(position));
		nodeTM.Scale(reinterpret_cast<const Point3&>(scale));

		return nodeTM;
	}

	static Matrix3 GetMatrix3(granny_transform& LocalTransform)
	{
		Matrix3 nodeTM = {};

		std::array<float, 3> float_scale;
		float_scale[0] = static_cast<float>(*LocalTransform.ScaleShear[0]);
		float_scale[1] = static_cast<float>(*LocalTransform.ScaleShear[1]);
		float_scale[2] = static_cast<float>(*LocalTransform.ScaleShear[2]);

		Point3 position = Point3(LocalTransform.Position[0], LocalTransform.Position[2], LocalTransform.Position[1]);
		Quat rotation = Quat(LocalTransform.Orientation[0], LocalTransform.Orientation[2], LocalTransform.Orientation[1], LocalTransform.Orientation[3]);
		Point3 scale = Point3(float_scale[0], float_scale[1], float_scale[2]);

		//Quat rotation_inv = rotation.Inverse();

		nodeTM.SetRotate(reinterpret_cast<const Quat&>(rotation));
		nodeTM.SetTrans(reinterpret_cast<const Point3&>(position));
		//nodeTM.Scale(reinterpret_cast<const Point3&>(scale));

		return nodeTM;
	}

	static INode* createJoint(const std::string& boneName, const DirectX::XMMATRIX& xmMatrix)
	{
		INode* sceneRoot = GetCOREInterface()->GetRootNode();

		//SimpleObject2* pBone = (SimpleObject2*)GetCOREInterface()->CreateInstance(GEOMOBJECT_CLASS_ID, BONE_OBJ_CLASSID);
		Object* pBone = (Object*)GetCOREInterface()->CreateInstance(HELPER_CLASS_ID, Class_ID(BONE_CLASS_ID, 0));

		INode* boneNode = GetCOREInterface()->CreateObjectNode(pBone);
		std::string updated_name = updateNodeName(boneName);

		Matrix3 nodeTM = GetMatrix3(xmMatrix);

		boneNode->SetName(fmnext::convertStdStringToWide(updated_name).c_str());
		boneNode->SetNodeTM(0, nodeTM);
		boneNode->BoxMode(TRUE);
		boneNode->SetWireColor(RGB(255.0f, 0.0f, 0.0f));
		//boneNode->SetWireColor(0x80ff);
		boneNode->SetBoneNodeOnOff(TRUE, 0);
		boneNode->SetRenderable(FALSE);
		boneNode->BoneAsLine(FALSE);
		boneNode->ShowBone(1);

		//sceneRoot->AttachChild(boneNode);

		return boneNode;
	}

	static void createDummyObject(const std::string& dName, DirectX::XMMATRIX& xmMatrix)
	{
		INode* sceneRoot = GetCOREInterface()->GetRootNode();
		DummyObject* obj = (DummyObject*)CreateInstance(HELPER_CLASS_ID, Class_ID(DUMMY_CLASS_ID, 0));
		INode* node = GetCOREInterface()->CreateObjectNode(obj);

		std::string updated_name = updateNodeName(dName);

		Matrix3 nodeTM = GetMatrix3(xmMatrix);
		node->SetName(fmnext::convertStdStringToWide(updated_name).c_str());
		node->SetNodeTM(0, nodeTM);

		sceneRoot->AttachChild(node);
	}

	static INode* GetNodeByName(const std::string& name)
	{
		INode* pNode = GetCOREInterface()->GetINodeByName(fmnext::convertStdStringToWide(name).c_str());

		if (pNode != nullptr)
		{
			return pNode;
		}

		return nullptr;
	}

	static bool IsNodeLinked(INode* node)
	{
		return node->GetParentNode() != NULL && !node->GetParentNode()->IsRootNode();
	}

	static bool parentObjectByName(const std::string& parent, const std::string& child)
	{

		INode* parentNode = GetNodeByName(parent);
		INode* childNode = GetNodeByName(child);

		// must ensure both of the nodes exists
		if (parentNode != nullptr && childNode != nullptr)
		{
			parentNode->AttachChild(childNode, 0);
		}
		else
		{
			return false;
		}

		return true;
	}

	/*
	* There is a issue that rotates 90.0 all meshes on X axis, need to investigate.
	*/

	static INode* createMesh1(const std::vector<DirectX::XMFLOAT3>& vertices, const std::vector<uint32_t>& indices, const std::vector<DirectX::XMFLOAT3>& normals, const std::vector<std::vector<DirectX::XMFLOAT2>>& uvs, const std::string& mMeshName, Mtl* material, bool useQuads)
	{
		PolyObject* pObj = CreateEditablePolyObject();
		MNMesh* mesh = &pObj->GetMesh();

		int geometry = (useQuads) ? 4 : 3;
		int numVertices = static_cast<int>(vertices.size());
		int numIndices = static_cast<int>(indices.size());
		int numPolygons = static_cast<int>(numIndices / geometry);

		mesh->setNumVerts(numVertices);
		mesh->setNumFaces(numPolygons);

		for (int i = 0; i < numVertices; ++i)
		{
			mesh->v[i].p = Point3(vertices[i].x, vertices[i].z, vertices[i].y); // Z-Up
		}

		for (int index = 0, inc = 0; index < numPolygons; index++, inc += geometry)
		{
			int v0 = indices[inc + 0];
			int v1 = indices[inc + 1];
			int v2 = indices[inc + 2];
			int v3 = (geometry == 4) ? indices[inc + 3] : 0xffffffff;

			MNFace* face = &mesh->f[index];

			if (v3 == 0xffffffff)
			{
				face->SetDeg(3);
				//face->MakePoly(3, nullptr);

				face->vtx[0] = v0;
				face->vtx[1] = v1;
				face->vtx[2] = v2;
			}
			else
			{
				face->SetDeg(4);
				//face->MakePoly(4, nullptr);

				for (int i = 0; i < geometry; ++i)
				{
					face->vtx[i] = indices[inc + i];
				}
			}

			if (!useQuads)
			{
				face->vtx[0] = v0;
				face->vtx[2] = v1;
				face->vtx[1] = v2;
			}

			face->smGroup = 0;
			face->material = 0;

			//face->ClearFlag(MN_DEAD);
		}

		
		mesh->FillInMesh();
		//mesh->buildNormals();

		mesh->SpecifyNormals();
		MNNormalSpec* normalSpec = mesh->GetSpecifiedNormals();
		//normalSpec->SetParent(mesh);
		//normalSpec->ClearAndFree();
		normalSpec->SetNumFaces(numPolygons);
		normalSpec->SetNumNormals(numVertices);
		
		for (int i = 0; i < numVertices && !normals.empty(); ++i)
		{
			normalSpec->Normal(i) = Point3(normals[i].x, normals[i].z, normals[i].y); // Z-Up;
			normalSpec->SetNormalExplicit(i, true);
		}

		mesh->SetMapNum(6);

		for (int channel = 0; channel < 5 && !uvs.empty(); ++channel)
		{
			int ch = channel + 1;
			mesh->InitMap(ch);
			MNMap* map = mesh->M(ch);

			if (map) {
				map->setNumVerts(numVertices);

				for (int i = 0; i < numVertices; ++i)
				{
					map->v[i] = Point3(uvs[channel][i].x, (1 - uvs[channel][i].y), 0.0f);
				}
			}
		}

		for (int index = 0, inc = 0; index < numPolygons && !normals.empty(); index++, inc += geometry)
		{
			MNFace* face = &mesh->f[index];
			MNNormalFace* normalFace = &normalSpec->Face(index);

			normalFace->SpecifyAll();
			normalFace->SetDegree(face->deg);

			if (!useQuads)
			{
				normalFace->SetNormalID(0, indices[inc + 0]); // X
				normalFace->SetSpecified(0, TRUE);

				normalFace->SetNormalID(2, indices[inc + 1]); // Z
				normalFace->SetSpecified(2, TRUE);

				normalFace->SetNormalID(1, indices[inc + 2]); // Y
				normalFace->SetSpecified(1, TRUE);
			}

			for (int i = 0; i < face->deg && useQuads; ++i)
			{
				normalFace->SetNormalID(i, indices[inc + i]);
				normalFace->SetSpecified(i, TRUE);
			}

			for (int channel = 0; channel < 4 && !uvs.empty(); ++channel)
			{
				MNMap* uvMap = mesh->M(channel + 1);

				uvMap->setNumFaces(numPolygons);
				MNMapFace& mapFace = uvMap->f[index];

				mapFace.MakePoly(face->deg, nullptr);

				if (!useQuads)
				{
					mapFace.tv[0] = indices[inc + 0];  // X
					mapFace.tv[2] = indices[inc + 1];  // Z
					mapFace.tv[1] = indices[inc + 2];  // Y
				}

				for (int i = 0; i < face->deg && useQuads; ++i)
				{
					mapFace.tv[i] = indices[inc + i];
				}
			}

			//normalFace->MNDebugPrint(true);
		}

		normalSpec->SetAllExplicit(true);
		normalSpec->BuildNormals();
		normalSpec->CheckNormals();

		mesh->buildNormals();
		mesh->checkNormals(TRUE);
		mesh->InvalidateGeomCache();
		mesh->InvalidateTopoCache();

		mesh->SetFlag(MN_MESH_FILLED_IN);

		//pObj->PointsWereChanged();
		pObj->SetChannelValidity(TOPO_CHAN_NUM, FOREVER);
		pObj->SetChannelValidity(GEOM_CHAN_NUM, FOREVER);
		pObj->NotifyDependents(FOREVER, PART_GEOM, REFMSG_CHANGE);

		std::string updated_name = updateNodeName(mMeshName);

		INode* node = GetCOREInterface()->CreateObjectNode(pObj);
		node->SetName(fmnext::convertStdStringToWide(updated_name).c_str());

		if (material != nullptr)
		{
			node->SetMtl(material);
		}

		node->InvalidateTM();
		node->InvalidateWS();
		node->ResetTransform(0, TRUE);

		Matrix3 nodeTM;
		//nodeTM.Invert();
		nodeTM.NoRot();
		nodeTM.NoTrans();
		nodeTM.NoScale();

		node->SetNodeTM(GetCOREInterface()->GetTime(), nodeTM);

		return node;
	}

	static INode* createMesh(const fmnext::Mesh* pMesh, const std::string& mMeshName, Mtl* material, bool useQuads)
	{
		PolyObject* pObj = CreateEditablePolyObject();
		MNMesh* mesh = &pObj->GetMesh();

		int geometry = (useQuads) ? 4 : 3;
		int numVertices = static_cast<int>(pMesh->vertices.size());
		int numIndices = static_cast<int>(pMesh->indices.size());
		int numPolygons = static_cast<int>(numIndices / geometry);

		mesh->setNumVerts(numVertices);
		mesh->setNumFaces(numPolygons);

		for (int i = 0; i < numVertices; ++i)
		{
			mesh->v[i].p = Point3(pMesh->vertices[i].x, pMesh->vertices[i].z, pMesh->vertices[i].y); // Z-Up
		}

		for (int index = 0, inc = 0; index < numPolygons; index++, inc += geometry)
		{
			int v0 = pMesh->indices[inc + 0];
			int v1 = pMesh->indices[inc + 1];
			int v2 = pMesh->indices[inc + 2];
			int v3 = (geometry == 4) ? pMesh->indices[inc + 3] : 0xffffffff;

			MNFace* face = &mesh->f[index];

			if (v3 == 0xffffffff)
			{
				face->SetDeg(3);
				//face->MakePoly(3, nullptr);

				face->vtx[0] = v0;
				face->vtx[1] = v1;
				face->vtx[2] = v2;
			}
			else
			{
				face->SetDeg(4);
				//face->MakePoly(4, nullptr);

				for (int i = 0; i < geometry; ++i)
				{
					face->vtx[i] = pMesh->indices[inc + i];
				}
			}

			if (!useQuads)
			{
				face->vtx[0] = v0;
				face->vtx[2] = v1;
				face->vtx[1] = v2;
			}

			face->smGroup = 0;
			face->material = 0;

			//face->ClearFlag(MN_DEAD);
		}


		mesh->FillInMesh();
		//mesh->buildNormals();

		mesh->SpecifyNormals();
		MNNormalSpec* normalSpec = mesh->GetSpecifiedNormals();
		//normalSpec->SetParent(mesh);
		//normalSpec->ClearAndFree();
		normalSpec->SetNumFaces(numPolygons);
		normalSpec->SetNumNormals(numVertices);

		for (int i = 0; i < numVertices && !pMesh->normals.empty(); ++i)
		{
			normalSpec->Normal(i) = Point3(pMesh->normals[i].x, pMesh->normals[i].z, pMesh->normals[i].y); // Z-Up;
			normalSpec->SetNormalExplicit(i, true);
		}

		mesh->SetMapNum(6);

		for (int channel = 0; channel < 5 && !pMesh->uvs.empty(); ++channel)
		{
			int ch = channel + 1;
			mesh->InitMap(ch);
			MNMap* map = mesh->M(ch);

			if (map) {
				map->setNumVerts(numVertices);

				for (int i = 0; i < numVertices; ++i)
				{
					map->v[i] = Point3(pMesh->uvs[channel][i].x, (1 - pMesh->uvs[channel][i].y), 0.0f);
				}
			}
		}

		for (int index = 0, inc = 0; index < numPolygons && !pMesh->normals.empty(); index++, inc += geometry)
		{
			MNFace* face = &mesh->f[index];
			MNNormalFace* normalFace = &normalSpec->Face(index);

			normalFace->SpecifyAll();
			normalFace->SetDegree(face->deg);

			if (!useQuads)
			{
				normalFace->SetNormalID(0, pMesh->indices[inc + 0]); // X
				normalFace->SetSpecified(0, TRUE);

				normalFace->SetNormalID(2, pMesh->indices[inc + 1]); // Z
				normalFace->SetSpecified(2, TRUE);

				normalFace->SetNormalID(1, pMesh->indices[inc + 2]); // Y
				normalFace->SetSpecified(1, TRUE);
			}

			for (int i = 0; i < face->deg && useQuads; ++i)
			{
				normalFace->SetNormalID(i, pMesh->indices[inc + i]);
				normalFace->SetSpecified(i, TRUE);
			}

			for (int channel = 0; channel < 4 && !pMesh->uvs.empty(); ++channel)
			{
				MNMap* uvMap = mesh->M(channel + 1);

				uvMap->setNumFaces(numPolygons);
				MNMapFace& mapFace = uvMap->f[index];

				mapFace.MakePoly(face->deg, nullptr);

				if (!useQuads)
				{
					mapFace.tv[0] = pMesh->indices[inc + 0];  // X
					mapFace.tv[2] = pMesh->indices[inc + 1];  // Z
					mapFace.tv[1] = pMesh->indices[inc + 2];  // Y
				}

				for (int i = 0; i < face->deg && useQuads; ++i)
				{
					mapFace.tv[i] = pMesh->indices[inc + i];
				}
			}

			//normalFace->MNDebugPrint(true);
		}

		normalSpec->SetAllExplicit(true);
		normalSpec->BuildNormals();
		normalSpec->CheckNormals();

		mesh->buildNormals();
		mesh->checkNormals(TRUE);
		mesh->InvalidateGeomCache();
		mesh->InvalidateTopoCache();

		mesh->SetFlag(MN_MESH_FILLED_IN);

		//pObj->PointsWereChanged();
		pObj->SetChannelValidity(TOPO_CHAN_NUM, FOREVER);
		pObj->SetChannelValidity(GEOM_CHAN_NUM, FOREVER);
		pObj->NotifyDependents(FOREVER, PART_GEOM, REFMSG_CHANGE);

		std::string updated_name = updateNodeName(mMeshName);

		INode* node = GetCOREInterface()->CreateObjectNode(pObj);
		node->SetName(fmnext::convertStdStringToWide(updated_name).c_str());

		if (material != nullptr)
		{
			node->SetMtl(material);
		}

		node->InvalidateTM();
		node->InvalidateWS();
		node->ResetTransform(0, TRUE);

		Matrix3 nodeTM;
		//nodeTM.Invert();
		nodeTM.NoRot();
		nodeTM.NoTrans();
		nodeTM.NoScale();

		node->SetNodeTM(GetCOREInterface()->GetTime(), nodeTM);

		return node;
	}

	static void weldVertsByThreshold(TriObject* triObj, float threshold)
	{
		Mesh& mesh = triObj->GetMesh();

		BitArray& targetVerts = mesh.VertSel();
		targetVerts.SetSize(mesh.getNumVerts());
		targetVerts.SetAll();

		MeshDelta meshDelta;
		meshDelta.WeldByThreshold(mesh, targetVerts, threshold);
		meshDelta.Apply(mesh);
	}

	static bool objExists(const std::string& name)
	{
		INode* pNode = GetCOREInterface()->GetINodeByName(fmnext::convertStdStringToWide(name).c_str());

		if (pNode != nullptr)
		{
			return true;
		}

		return false;
	}

	static bool nodeExists(const std::string& name)
	{
		INode* pNode = GetCOREInterface()->GetINodeByName(fmnext::convertStdStringToWide(name).c_str());

		if (pNode != nullptr && pNode->ClassID() == Class_ID(BONE_CLASS_ID, 0))
		{
			return true;
		}

		return false;
	}

	static bool MtlExists(const std::string& Name)
	{
		std::wstring widestr = std::wstring(Name.begin(), Name.end());
		MSTR wName = widestr.c_str();

		Mtl* material = GetCOREInterface7()->FindMtlNameInScene(wName);

		if (material != nullptr)
		{
			return true;
		}

		return false;
	}

	static Mtl* GetSceneMtl(const std::string& Name)
	{
		std::wstring widestr = std::wstring(Name.begin(), Name.end());
		MSTR wName = widestr.c_str();

		Mtl* material = GetCOREInterface7()->FindMtlNameInScene(wName);

		if (material != nullptr)
		{
			return material;
		}

		return nullptr;
	}

	static bool isNodeInScene(INode* node)
	{
		for (INode* par = node->GetParentNode();
			par != nullptr;
			node = par, par = node->GetParentNode()) {
		}
		return node == GetCOREInterface()->GetRootNode();
	}

	static bool isUsedInScene(ReferenceTarget* mt)
	{
		DependentIterator di(mt);
		RefMakerHandle rm;

		if (mt->SuperClassID() == BASENODE_CLASS_ID) {
			return isNodeInScene(static_cast <INode*>(mt));
		}

		while ((rm = di.Next()) != NULL) {
			if (rm != mt && rm->IsRefTarget()
				&& isUsedInScene(static_cast<ReferenceTarget*>(rm)))
				return true;
		}
		return false;
	}

	static Mtl* createMaterialfromMemory(const std::string& Name, const std::shared_ptr<fmnext::BundleReader::BundleData>& primary_material)
	{
		Mtl* mtl = NewPhysicalMaterial();
		std::string updated_name = updateNodeName(Name);

		mtl->SetName(fmnext::convertStdStringToWide(updated_name).c_str());

		if (primary_material)
		{
			//MGlobal::displayWarning("Using shader information");
			std::vector<DirectX::XMFLOAT4> colors;

			for (const auto& param : primary_material->ShaderParameters)
			{
				if (param.type == fmnext::ShaderParameter_Color)
				{
					colors.push_back(std::any_cast<DirectX::XMFLOAT4>(param.value));

					DirectX::XMFLOAT4 diffuse = std::any_cast<DirectX::XMFLOAT4>(param.value);
				}

				if (param.type == fmnext::ShaderParameter_Texture2D)
				{
					std::string path = std::any_cast<std::string>(param.value);
					//MGlobal::displayInfo(path.c_str());
				}
			}

			const int NumParamBlocks = mtl->NumParamBlocks();

			for (int j = 0; j < NumParamBlocks; ++j)
			{
				IParamBlock2* ParamBlock2 = mtl->GetParamBlockByID(j);

				// The the descriptor to 'decode'
				ParamBlockDesc2* ParamBlockDesc = ParamBlock2->GetDesc();

				// Loop through all the defined parameters therein
				for (int i = 0; i < ParamBlockDesc->count; ++i)
				{
					const ParamDef& ParamDefinition = ParamBlockDesc->paramdefs[i];

					// base
					if (_tcsicmp(ParamDefinition.int_name, _T("base_weight")) == 0)
					{
						ParamBlock2->SetValue(ParamDefinition.ID, GetCOREInterface()->GetTime(), (float)0.5);
					}

					if (_tcsicmp(ParamDefinition.int_name, _T("base_color")) == 0)
					{
						if (!colors.empty())
						{
							ParamBlock2->SetValue(ParamDefinition.ID, GetCOREInterface()->GetTime(), Color(colors[0].x, colors[0].y, colors[0].z));
						}

						if (colors.empty())
						{
							ParamBlock2->SetValue(ParamDefinition.ID, GetCOREInterface()->GetTime(), Color(0x15161b));
						}
					}

					if (_tcsicmp(ParamDefinition.int_name, _T("reflectivity")) == 0)
					{
						ParamBlock2->SetValue(ParamDefinition.ID, GetCOREInterface()->GetTime(), (float)1.0f);
					}

					if (_tcsicmp(ParamDefinition.int_name, _T("roughness")) == 0)
					{
						ParamBlock2->SetValue(ParamDefinition.ID, GetCOREInterface()->GetTime(), (float)0.5f);
					}

					// coat
					/*
					if (_tcsicmp(ParamDefinition.int_name, _T("coating")) == 0)
					{
						ParamBlock2->SetValue(ParamDefinition.ID, GetCOREInterface()->GetTime(), (float)1.0);
					}
					if (_tcsicmp(ParamDefinition.int_name, _T("coat_affect_roughness")) == 0)
					{
						ParamBlock2->SetValue(ParamDefinition.ID, GetCOREInterface()->GetTime(), (float)0.0);
					}
					if (_tcsicmp(ParamDefinition.int_name, _T("coat_ior")) == 0)
					{
						ParamBlock2->SetValue(ParamDefinition.ID, GetCOREInterface()->GetTime(), (float)1.5);
					}
					if (_tcsicmp(ParamDefinition.int_name, _T("coat_anisotropy")) == 0)
					{
						ParamBlock2->SetValue(ParamDefinition.ID, GetCOREInterface()->GetTime(), (float)0.5);
					}

					if (_tcsicmp(ParamDefinition.int_name, _T("transparency")) == 0)
					{
						//ParamBlock2->SetValue(ParamDefinition.ID, GetCOREInterface()->GetTime(), (float)0.5);
						//if (Name.find("_gls") != std::string::npos)
						{

						}
					}
					*/
				}

				ParamBlock2->ReleaseDesc();
			}

		}

		return mtl;
	}

	static Mtl* createMaterialShader(const std::string& Name)
	{
		Mtl* mtl = NewPhysicalMaterial();
		std::string updated_name = updateNodeName(Name);

		mtl->SetName(fmnext::convertStdStringToWide(updated_name).c_str());
		mtl->SetDiffuse(Color(0.451, 0.451, 0.451), 0);

		return mtl;
	}

	static Mtl* createCarpaintfromMemory(const std::string& Name, const DirectX::XMFLOAT3& carpaint)
	{
		Mtl* mtl = NewPhysicalMaterial();
		std::string updated_name = updateNodeName(Name);

		mtl->SetName(fmnext::convertStdStringToWide(updated_name).c_str());
		mtl->SetDiffuse(Color(carpaint.x, carpaint.y, carpaint.z), 0);

		return mtl;
	}

	static Mtl* createGlassfromMemory(const std::string& Name)
	{
		Mtl* mtl = NewPhysicalMaterial();
		std::string updated_name = updateNodeName(Name);

		mtl->SetName(fmnext::convertStdStringToWide(updated_name).c_str());
		mtl->SetDiffuse(Color(0.451, 0.451, 0.451), 0);

		const int NumParamBlocks = mtl->NumParamBlocks();

		for (int j = 0; j < NumParamBlocks; ++j)
		{
			IParamBlock2* ParamBlock2 = mtl->GetParamBlockByID(j);

			// The the descriptor to 'decode'
			ParamBlockDesc2* ParamBlockDesc = ParamBlock2->GetDesc();

			// Loop through all the defined parameters therein
			for (int i = 0; i < ParamBlockDesc->count; ++i)
			{
				const ParamDef& ParamDefinition = ParamBlockDesc->paramdefs[i];

				// base
				if (_tcsicmp(ParamDefinition.int_name, _T("base_weight")) == 0)
				{
					ParamBlock2->SetValue(ParamDefinition.ID, GetCOREInterface()->GetTime(), (float)1.0);
				}

				if (_tcsicmp(ParamDefinition.int_name, _T("base_color")) == 0)
				{
					ParamBlock2->SetValue(ParamDefinition.ID, GetCOREInterface()->GetTime(), Color(0.f, 0.f, 0.f));
				}

				// coat
				if (_tcsicmp(ParamDefinition.int_name, _T("coating")) == 0)
				{
					ParamBlock2->SetValue(ParamDefinition.ID, GetCOREInterface()->GetTime(), (float)0.0);
				}

				if (_tcsicmp(ParamDefinition.int_name, _T("coat_affect_roughness")) == 0)
				{
					ParamBlock2->SetValue(ParamDefinition.ID, GetCOREInterface()->GetTime(), (float)0.5);
				}

				if (_tcsicmp(ParamDefinition.int_name, _T("coat_affect_color")) == 0)
				{
					ParamBlock2->SetValue(ParamDefinition.ID, GetCOREInterface()->GetTime(), (float)0.5);
				}

				if (_tcsicmp(ParamDefinition.int_name, _T("coat_ior")) == 0)
				{
					ParamBlock2->SetValue(ParamDefinition.ID, GetCOREInterface()->GetTime(), (float)1.5);
				}

				if (_tcsicmp(ParamDefinition.int_name, _T("coat_anisotropy")) == 0)
				{
					ParamBlock2->SetValue(ParamDefinition.ID, GetCOREInterface()->GetTime(), (float)0.5);
				}

				if (_tcsicmp(ParamDefinition.int_name, _T("transparency")) == 0)
				{
					ParamBlock2->SetValue(ParamDefinition.ID, GetCOREInterface()->GetTime(), (float)1.0);
				}

				if (_tcsicmp(ParamDefinition.int_name, _T("thin_walled")) == 0)
				{
					ParamBlock2->SetValue(ParamDefinition.ID, GetCOREInterface()->GetTime(), 0);
				}

				if (_tcsicmp(ParamDefinition.int_name, _T("sss_depth")) == 0)
				{
					ParamBlock2->SetValue(ParamDefinition.ID, GetCOREInterface()->GetTime(), 10);
				}

				if (_tcsicmp(ParamDefinition.int_name, _T("sss_scale")) == 0)
				{
					ParamBlock2->SetValue(ParamDefinition.ID, GetCOREInterface()->GetTime(), (float)1.0);
				}

			}

			ParamBlock2->ReleaseDesc();
		}



		return mtl;
	}

	static bool parentMeshtoShader(INode* mesh_obj, Mtl* shader_obj)
	{
		mesh_obj->SetMtl(shader_obj);

		return true;
	}

	static INode* createLocator(DirectX::XMMATRIX& xmMatrix)
	{
		DummyObject* obj = (DummyObject*)CreateInstance(HELPER_CLASS_ID, Class_ID(DUMMY_CLASS_ID, 0));
		INode* node = GetCOREInterface()->CreateObjectNode(obj);

		Matrix3 nodeTM = GetMatrix3(xmMatrix);

		//node->SetName(fmnext::convertMaxTchar(dName.c_str()));
		node->SetNodeTM(0, nodeTM);

		return node;
	}

	static INode* createLocator()
	{
		DummyObject* obj = (DummyObject*)CreateInstance(HELPER_CLASS_ID, Class_ID(DUMMY_CLASS_ID, 0));
		INode* node = GetCOREInterface()->CreateObjectNode(obj);

		return node;
	}

	static void setNodeTransformation(INode* node, const DirectX::XMMATRIX& xmMatrix)
	{
		Matrix3 nodeTM = GetMatrix3(xmMatrix);
		node->SetNodeTM(0, nodeTM);
	}

	static const std::string GetContainerDirection(const std::string& container)
	{
		std::vector<std::string> lNodePos = { "LF", "RF", "RR", "LR", "LM", "RM" };

		for (const auto& position : lNodePos) {
			if (std::regex_search(container, std::regex(position, std::regex_constants::icase))) {
				return position;
			}
		}

		return std::string();
	}

	static fmnext::PartLocation GetPartDirection(const std::string& containerName)
	{
		std::string lcontainerPos = GetContainerDirection(containerName);
		std::vector<std::string> lNodePos = { "LF", "RF", "RR", "LR", "LM", "RM" };
		std::unordered_map<std::string, fmnext::PartLocation> lpartMap 
		{
			{ "LF", fmnext::PartLocation(fmnext::PartLocation::FRONT, fmnext::PartLocation::LEFT) },
			{ "RF", fmnext::PartLocation(fmnext::PartLocation::FRONT, fmnext::PartLocation::RIGHT) },

			{ "LR", fmnext::PartLocation(fmnext::PartLocation::REAR, fmnext::PartLocation::LEFT) },
			{ "RR", fmnext::PartLocation(fmnext::PartLocation::REAR, fmnext::PartLocation::RIGHT) },

			{ "LM", fmnext::PartLocation(fmnext::PartLocation::MID, fmnext::PartLocation::LEFT) },
			{ "RM", fmnext::PartLocation(fmnext::PartLocation::MID, fmnext::PartLocation::RIGHT) }
		};

		for (const auto& position : lNodePos) {
			if (std::regex_match(lcontainerPos, std::regex(position, std::regex_constants::icase))) {
				if (const auto& result = lpartMap.find(position); result != lpartMap.end()) {
					return result->second;
				}
			}
		}

		return fmnext::PartLocation();
	}

	static void SetImporterFrameRate()
	{
		if (GetFrameRate() != 60 && GetTimeDisplayMode() != TimeDisp::DISPTIME_FRAMES)
		{
			SetFrameRate(60);
			SetTimeDisplayMode(TimeDisp::DISPTIME_FRAMES);
		}
	}

	static void LoadSkeleton(const std::vector<fmnext::BundleReader::Bones>& skeleton)
	{
		std::vector<INode*> joints;
		joints.reserve(skeleton.size());

		for (size_t i = 0; i < skeleton.size(); ++i)
		{
			std::string bone_name = (skeleton[i].name == "<root>") ? "BundleRootBone" : skeleton[i].name;

			INode* locatorObj = createJoint(bone_name, skeleton[i].transform);

			joints.push_back(locatorObj);

			if (skeleton[i].parent_index != GrannyNoParentBone)
			{
				int ParentIndex = skeleton[i].parent_index;

				joints[ParentIndex]->AttachChild(joints[i], 0);
			}
		}
	}

	static void handleSkeleton(granny_skeleton* Skeleton)
	{
		std::string skel_name(Skeleton->Name);

		if (!objExists(skel_name))
		{
			std::vector<INode*> joints;
			joints.reserve(Skeleton->BoneCount);

			for (size_t i = 0; i < Skeleton->BoneCount; ++i)
			{
				granny_transform LocalTransform = Skeleton->Bones[i].LocalTransform; // Position[3], Orientation[4], ScaleShear[3]
				std::string Name(Skeleton->Bones[i].Name);

				INode* locatorObj = createJoint(Name, LocalTransform);

				joints.push_back(locatorObj);

				if (Skeleton->Bones[i].ParentIndex != GrannyNoParentBone)
				{
					granny_int32 ParentIndex = Skeleton->Bones[i].ParentIndex;

					joints[ParentIndex]->AttachChild(joints[i], 0);
				}
			}
		}
		else
		{
			//std::string message;
			//message.format("^1s already exists, ignoring next... \n", skel_name.c_str());
			//MGlobal::displayWarning(message);
		}

		//GetCOREInterface()->ForceCompleteRedraw(TRUE);
	}

	static bool skeletonExists(granny_skeleton* Skeleton = nullptr)
	{
		std::string skel_name(Skeleton->Name);

		if (objExists(skel_name))
		{
			return true;
		}

		return false;
	}

	static bool handleAnimation(granny_skeleton* Skeleton = nullptr, granny_animation* Animation = nullptr)
	{
		if (Skeleton && Animation)
		{
			// Assume there is only a single animation/track on each gr2
			granny_track_group* TrackGroup = Animation->TrackGroups[0];

			uint32_t TotalSamples = (uint32_t)(Animation->Duration * 30); // amount of frames

			for (granny_int32 TrackIndex = 0; TrackIndex < TrackGroup->TransformTrackCount; ++TrackIndex) {

				std::string boneName = TrackGroup->TransformTracks[TrackIndex].Name;

				granny_int32x BoneIndex{};
				if (!GrannyFindBoneByName(Skeleton, TrackGroup->TransformTracks[TrackIndex].Name, &BoneIndex)) {

					std::string message = "Unable to find bone ";
					message += boneName;
					message += " \n";

					fmnext::PrinttoListener(message);

					continue;
				}

				//Interval animation_range(0, Integer(TotalSamples).to_timevalue());

				if (BoneIndex != GrannyNoParentBone)
				{
					INode* mObject = GetCOREInterface()->GetINodeByName(fmnext::convertStdStringToWide(updateNodeName(boneName)).c_str());

					if (!mObject)
					{
						std::string message = "Unable to find INode for bone ";
						message += boneName;
						message += " \n";

						fmnext::PrinttoListener(message);

						return false;
					}

					auto posControl = mObject->GetTMController()->GetPositionController();
					auto rotControl = mObject->GetTMController()->GetRotationController();

					AnimateOn();

					for (uint32_t CurrentFrame = 0; CurrentFrame < TotalSamples; ++CurrentFrame)
					{
						granny_real32 CurrentTime = CurrentFrame * (Animation->Duration / (TotalSamples - 1));

						// Get the three curves that specify the transform
						granny_curve2* PositionCurve = &TrackGroup->TransformTracks[TrackIndex].PositionCurve;
						granny_curve2* OrientationCurve = &TrackGroup->TransformTracks[TrackIndex].OrientationCurve;
						granny_curve2* ScaleShearCurve = &TrackGroup->TransformTracks[TrackIndex].ScaleShearCurve; // going to ignore this for a while

						granny_transform EvalResult{};
						GrannyMakeIdentity(&EvalResult);

						GrannyEvaluateCurveAtT(3, false, false, PositionCurve, true, Animation->Duration, CurrentTime, EvalResult.Position, GrannyCurveIdentityPosition);
						GrannyEvaluateCurveAtT(4, false, false, OrientationCurve, true, Animation->Duration, CurrentTime, EvalResult.Orientation, GrannyCurveIdentityOrientation);

						TimeValue keyFrame = GetCOREInterface()->GetTime() +  Integer(CurrentFrame).to_timevalue();

						Point3 pos(EvalResult.Position[0], EvalResult.Position[2], EvalResult.Position[1]);
						posControl->SetValue(keyFrame, (void*)(&pos));

						Quat quat(EvalResult.Orientation[0], EvalResult.Orientation[2], EvalResult.Orientation[1], EvalResult.Orientation[3]);
						rotControl->SetValue(keyFrame, (void*)(&quat));

						//printf("Track: %s, Frame: %i: Position[% .3f, % .3f, % .3f]\n", TrackGroup->TransformTracks[TrackIndex].Name, CurrentFrame, EvalResult.Position[0], EvalResult.Position[1], EvalResult.Position[2]);
						//printf("Time: %.3f: Rotation[% .3f, % .3f, % .3f, % .3f]\n", CurrentTime, EvalResult.Orientation[0], EvalResult.Orientation[1], EvalResult.Orientation[2], EvalResult.Orientation[3]);
					}

					AnimateOff();

					//GetCOREInterface()->SetAnimRange(animation_range);
				}

			}

			return true;
		}


		return false;
	}


	static std::string updateNodeName(const std::string& str)
	{
		return std::regex_replace(str, std::regex("([^a-zA-Z0-9\\w]+|\\s+|^[0-9]+)"), std::string("_"));;
	}

	static std::filesystem::path deduplicatePath(const std::string& path) {
		std::filesystem::path result(path);
		int index = 1;

		while (std::filesystem::exists(result))
		{
			std::string filename = std::filesystem::path(path).stem().string();
			filename += "_";
			filename += std::to_string(index);
			filename += std::filesystem::path(path).extension().string();

			result = std::filesystem::path(path).replace_filename(filename);

			++index;
		}

		return result;
	}

private:
	static INode* createJoint(const std::string& mName, granny_transform& LocalTransform)
	{
		//SimpleObject2* pBone = (SimpleObject2*)GetCOREInterface()->CreateInstance(GEOMOBJECT_CLASS_ID, BONE_OBJ_CLASSID);
		Object* pBone = (Object*)GetCOREInterface()->CreateInstance(HELPER_CLASS_ID, Class_ID(BONE_CLASS_ID, 0));

		INode* boneNode = GetCOREInterface()->CreateObjectNode(pBone);
		std::string updated_name = updateNodeName(mName);

		Matrix3 matrix = GetMatrix3(LocalTransform);

		boneNode->SetName(fmnext::convertStdStringToWide(updated_name).c_str());
		boneNode->SetNodeTM(0, matrix);
		boneNode->BoxMode(TRUE);
		boneNode->SetWireColor(RGB(255.0f, 0.0f, 0.0f));
		//boneNode->SetWireColor(0x80ff);
		boneNode->SetBoneNodeOnOff(TRUE, 0);
		boneNode->SetRenderable(FALSE);
		boneNode->BoneAsLine(FALSE);
		boneNode->ShowBone(1);

		return boneNode;
	}
};

