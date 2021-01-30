#include "stdafx.h"
#include "MeshInstancingFile.h"

#include "Utilities/BinaryFile.h"
#include "MeshInstancingDesc.h"

MeshInstancingFile::MeshInstancingFile(MeshInstancingDesc * desc)
	: desc(desc)
{
}

MeshInstancingFile::~MeshInstancingFile()
{
}

void MeshInstancingFile::Save(BinaryWriter * w)
{
	if (desc->Mesh == nullptr)
	{
		w->Int(-1);
		return;
	}
	w->Int(desc->MeshType);

	// meshInstancing
	w->Float(desc->_F1);
	w->Float(desc->_F2);
	w->UInt(desc->_U1);
	w->UInt(desc->_U2);
	desc->MImGui->Save(w);

	// meshInstance
	UINT size = desc->Meshes.size();
	w->UInt(size);
	for (UINT i = 0; i < size; i++)
	{
		MeshInstance* mesh = desc->Meshes[i];
		desc->TImGui->Save(mesh->GetTransform(), w);
		ColliderBox* collider = desc->Colliders[i];

		bool hasCollider = collider != nullptr;
		w->Bool(hasCollider);
		if (hasCollider)
			desc->CImGui->Save(collider, w);
	}
}

void MeshInstancingFile::Load(BinaryReader * r)
{
	desc->MeshType = r->Int();
	if (desc->MeshType == -1) return;

	// meshInstancing
	desc->_F1 = r->Float();
	desc->_F2 = r->Float();
	desc->_U1 = r->UInt();
	desc->_U2 = r->UInt();

	desc->MImGui->Load(r);

	desc->SetInstancing();

	// meshInstance
	UINT size = r->UInt();
	for (UINT i = 0; i < size; i++)
	{
		desc->AddInstance();

		MeshInstance* mesh = desc->Meshes[i];
		desc->TImGui->Load(mesh->GetTransform(), r);

		bool hasCollider = r->Bool();
		if (hasCollider)
			desc->CImGui->Load(desc->AddCollider(i), r);
	}

	desc->Mesh->UpdateTransforms();
}
