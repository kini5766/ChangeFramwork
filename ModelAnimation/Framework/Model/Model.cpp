#include "Framework.h"
#include "Model.h"
#include "ModelMesh.h"

#include "Utilities/Xml.h"
#include "Utilities/BinaryFile.h"

Model::Model()
{
}

Model::~Model()
{
	for (auto mat : materials)
		SafeDelete(mat);

	for (auto d : bones)
		SafeDelete(d);

	for (auto d : meshes)
		SafeDelete(d);

	for (auto clip : clips)
	{
		for (auto bone : clip->Bones)
			SafeDelete(bone);

		SafeDelete(clip);
	}
}


#pragma region Attach

void Model::Attach(Shader * shader, Model * model, int parentBoneIndex, Transform * offset)
{
	// src 기존 -> dst 추가에 맞게 변경된 값

	// Copy Material
	for (ModelMaterial* src : model->Materials())
	{
		ModelMaterial* dst = new ModelMaterial(shader);
		dst->Name(src->Name());
		dst->Ambient(src->Ambient());
		dst->Diffuse(src->Diffuse());
		dst->Specular(src->Specular());

		Texture* t;
		t = src->DiffuseMap();
		if (t != nullptr)
			dst->DiffuseMap(t->GetFile());

		t = src->SpecularMap();
		if (t != nullptr)
			dst->SpecularMap(t->GetFile());

		t = src->NormalMap();
		if (t != nullptr)
			dst->NormalMap(t->GetFile());

		materials.push_back(dst);
	}

	struct BoneNum
	{
		int oldNum;
		int newNum;
	};
	vector<BoneNum> changes;

	// Copy Bone
	for (ModelBone* srcBone : model->Bones())
	{
		ModelBone* dstBone = new ModelBone();

		ModelBoneData* src = srcBone->Data();
		ModelBoneData* dst = dstBone->Data();

		dst->Name = src->Name;
		memcpy(&dst->Transform, src->Transform, sizeof(Matrix));

		// 보정 값 적용
		if (offset != nullptr)
			dst->Transform = dst->Transform * offset->World() * bones[parentBoneIndex]->Data()->Transform;

		if (src->ParentIndex > -1)
		{
			// 번호 교체
			int parentIndex = src->ParentIndex;

			for (BoneNum swap : changes)
			{
				dst->ParentIndex = swap.newNum;
				break;
			}
		}
		else
		{
			// 루트 노드
			dst->ParentIndex = parentBoneIndex;
		}

		dst->Index = bones.size();

		dstBone->BindBone(bones[dst->ParentIndex]);

		changes.push_back({ src->Index, dst->Index });

		bones.push_back(dstBone);
	}// for src

	// Copy Mesh
	for (ModelMesh* srcMesh : model->Meshes())
	{
		const ModelMeshData* src = srcMesh->Data();
		ModelMeshData* dst = new ModelMeshData();
		ModelMeshData::Copy<Model::ModelVertex>(dst, src);

		for (BoneNum swap : changes)
		{
			if (swap.oldNum == src->PBind->BoneIndex)
			{
				dst->PBind->BoneIndex = swap.newNum;
				dst->PBind->PBone = bones[dst->PBind->BoneIndex];
				break;
			}
		}

		ModelMesh* dstMesh = new ModelMesh();
		dstMesh->Data(dst);
		dst->SafeDeleteData();

		SafeDelete(dst);

		meshes.push_back(dstMesh);
	}
}

#pragma endregion


#pragma region ReadFile

void Model::ReadMaterial(wstring file)
{
	file = URI::Textures + file + L".material";

	Xml::XMLDocument* document = new Xml::XMLDocument();
	Xml::XMLError error = document->LoadFile(String::ToString(file).c_str());
	assert(error == Xml::XML_SUCCESS);

	Xml::XMLElement* root = document->FirstChildElement();
	Xml::XMLElement* materialNode = root->FirstChildElement();

	do
	{
		ModelMaterial* material = new ModelMaterial();
		Xml::XMLElement* node = nullptr;

		wstring texture = L"";
		wstring directory = Path::GetDirectoryName(file);
		String::Replace(&directory, URI::Textures, L"");

		Color color;

		node = materialNode->FirstChildElement();
		material->Name(String::ToWString(node->GetText()));

		node = node->NextSiblingElement();
		texture = String::ToWString(node->GetText());
		if (texture.length() > 0)
			material->DiffuseMap(directory + texture);

		node = node->NextSiblingElement();
		texture = String::ToWString(node->GetText());
		if (texture.length() > 0)
			material->SpecularMap(directory + texture);

		node = node->NextSiblingElement();
		texture = String::ToWString(node->GetText());
		if (texture.length() > 0)
			material->NormalMap(directory + texture);

		node = node->NextSiblingElement();
		color.r = node->FloatAttribute("R");
		color.g = node->FloatAttribute("G");
		color.b = node->FloatAttribute("B");
		color.a = node->FloatAttribute("A");
		material->Ambient(color);

		node = node->NextSiblingElement();
		color.r = node->FloatAttribute("R");
		color.g = node->FloatAttribute("G");
		color.b = node->FloatAttribute("B");
		color.a = node->FloatAttribute("A");
		material->Diffuse(color);

		node = node->NextSiblingElement();
		color.r = node->FloatAttribute("R");
		color.g = node->FloatAttribute("G");
		color.b = node->FloatAttribute("B");
		color.a = node->FloatAttribute("A");
		material->Specular(color);

		materials.push_back(material);

		materialNode = materialNode->NextSiblingElement();
	} while (materialNode != nullptr);
}

void Model::ReadMesh(wstring file)
{
	file = URI::Models + file + L".mesh";

	BinaryReader r;
	r.Open(file);

	UINT count = 0;

	// ModelBone
	count = r.UInt();
	for (UINT i = 0; i < count; i++)
	{
		ModelBone* bone = new ModelBone();

		bone->Data()->Index = r.Int();
		bone->Data()->Name = String::ToWString(r.String());
		bone->Data()->ParentIndex = r.Int();
		memcpy(&bone->Data()->Transform, &r.Matrix(), sizeof(Matrix));

		bones.push_back(bone);
	}

	// ModelMesh
	count = r.UInt();
	for (UINT i = 0; i < count; i++)
	{
		ModelMeshData data;

		data.NewBindValue();
		data.PBind->Name = String::ToWString(r.String());
		data.PBind->BoneIndex = r.Int();

		data.PBind->MaterialName = String::ToWString(r.String());

		// VertexData
		{
			UINT count = r.UInt();
			data.NewVertices<Model::ModelVertex>(count);
			r.BYTE(&data.Vertices, data.Stride * count);
		}

		// Index Data
		{
			UINT count = r.UInt();
			data.NewIndices(count);
			r.BYTE((void**)&data.Indices, sizeof(UINT) * count);
		}

		ModelMesh* mesh = new ModelMesh();
		mesh->Data(&data);
		data.SafeDeleteData();

		meshes.push_back(mesh);
	}

	r.Close();

	BindBone();
	BindMesh();
}

void Model::ReadClip(wstring file)
{
	file = URI::Models + file + L".clip";

	BinaryReader r;
	r.Open(file);

	ClipData* data = new ClipData();

	data->ClipName = String::ToWString(r.String());
	data->Duration = r.Float();
	data->FrameRate = r.Float();

	UINT boneSize = r.UInt();
	for (UINT i = 0; i < boneSize; i++)
	{
		ClipBoneData* bone = new ClipBoneData();
		bone->BoneName = String::ToWString(r.String());

		UINT count;

		// Position Key
		{
			count = r.UInt();
			bone->KeyPositions.assign(count, KeyVector());

			void* ptr = (void*)&(bone->KeyPositions[0]);
			r.BYTE(&ptr, sizeof(KeyVector) * count);
		}

		// Rotation Key
		{
			count = r.UInt();
			bone->KeyRotations.assign(count, KeyQuat());

			void* ptr = (void*)&(bone->KeyRotations[0]);
			r.BYTE(&ptr, sizeof(KeyQuat) * count);
		}

		// Scale Key
		{
			count = r.UInt();
			bone->KeyScales.assign(count, KeyVector());

			void* ptr = (void*)&(bone->KeyScales[0]);
			r.BYTE(&ptr, sizeof(KeyVector) * count);
		}

		data->Bones.push_back(bone);
	}

	r.Close();

	clips.push_back(data);

}

#pragma endregion


#pragma region Getter

// Material
ModelMaterial * Model::MaterialByName(wstring name)
{
	for (ModelMaterial* material : materials)
	{
		if (material->Name() == name)
			return material;
	}

	return nullptr;
}

// ModelBone
ModelBone * Model::BoneByName(wstring name)
{
	for (ModelBone* bone : bones)
	{
		if (bone->Data()->Name == name)
			return bone;
	}

	return nullptr;
}

ClipData * Model::ClipByName(wstring name)
{
	for (ClipData* clip : clips)
	{
		if (clip->ClipName == name)
			return clip;
	}

	return nullptr;
}

#pragma endregion


#pragma region Bind

void Model::BindBone()
{
	root = bones[0];
	root->BindBone(nullptr);
	for (ModelBone* bone : bones)
	{
		int parentIndex = bone->Data()->ParentIndex;
		if (parentIndex > -1)
			bone->BindBone(bones[parentIndex]);
	}
}

void Model::BindMesh()
{
	// 메쉬 그려질 위치 찾기
	for (ModelMesh* mesh : meshes)
	{
		for (ModelBone* bone : bones)
		{
			if (mesh->BoneIndex() == bone->Data()->Index)
			{
				mesh->BindBone(bone);
				break;
			}
		}
	}
}

#pragma endregion


