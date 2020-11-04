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
	for (Material* mat : materials)
		SafeDelete(mat);

	for (ModelBone* d : bones)
		SafeDelete(d);

	for (ModelMesh* d : meshes)
		SafeDelete(d);

	for (ModelClip* d : clips)
		SafeDelete(d);
}

void Model::Attach(Shader * shader, Model * model, int parentBoneIndex, Transform * transform)
{
	// src 기존 -> dst 추가에 맞게 변경된 값

	// Copy Material
	for (Material* src : model->Materials())
	{
		Material* dst = new Material(shader);
		dst->Name(src->Name());
		dst->Ambient(src->Ambient());
		dst->Diffuse(src->Diffuse());
		dst->Specular(src->Specular());

		if (src->DiffuseMap())
			dst->DiffuseMap(src->DiffuseMap()->GetFile());
		if (src->SpecularMap())
			dst->SpecularMap(src->SpecularMap()->GetFile());
		if (src->NormalMap())
			dst->DiffuseMap(src->NormalMap()->GetFile());

		materials.push_back(dst);
	}

	struct BoneNum
	{
		int oldNum;
		int newNum;
	};
	vector<BoneNum> changes;

	// Copy Bone
	{
		ModelBone* parentBone = BoneByIndex(parentBoneIndex);

		for (ModelBone* src : model->Bones())
		{
			ModelBone* dst = new ModelBone();
			dst->name = src->name;
			dst->transform = src->transform;

			// 보정 값 적용
			if (transform != nullptr)
				dst->transform = dst->transform * transform->World();
			else
				int a = 0;

			if (src->parent != nullptr)
			{
				// 번호 교체
				int parentIndex = src->parentIndex;

				for (BoneNum swap : changes)
				{
					dst->parentIndex = swap.newNum;
					dst->parent = bones[dst->parentIndex];
					dst->parent->childs.push_back(dst);
					break;
				}
			}
			else
			{
				// 루트 노드
				dst->parentIndex = parentBoneIndex;
				dst->parent = parentBone;
				dst->parent->childs.push_back(dst);
			}

			dst->index = bones.size();
			changes.push_back({ src->index, dst->index});
			bones.push_back(dst);
		}// for src
	}

	// Copy Mesh
	for (ModelMesh* src : model->Meshes())
	{
		ModelMesh* dst = new ModelMesh();

		for (BoneNum swap : changes)
		{
			if (swap.oldNum == src->boneIndex)
			{
				dst->boneIndex = swap.newNum;
				break;
			}
		}

		dst->bone = bones[dst->boneIndex];
		dst->name = src->name;
		dst->materialName = src->materialName;

		dst->vertexCount = src->vertexCount;
		dst->indexCount = src->indexCount;

		UINT verticesSize = dst->vertexCount * sizeof(ModelVertex);
		dst->vertices = new ModelVertex[dst->vertexCount];
		memcpy_s(dst->vertices, verticesSize, src->vertices, verticesSize);

		UINT indicesSize = dst->indexCount * sizeof(UINT);
		dst->indices = new UINT[dst->indexCount];
		memcpy_s(dst->indices, indicesSize, src->indices, indicesSize);

		dst->Binding(this);
		dst->SetShader(shader);

		meshes.push_back(dst);
	}

}

void Model::Copy(Model ** out)
{
	(*out) = new Model();

	// Copy Material
	for (Material* src : materials)
	{
		Material* dst = new Material();
		dst->Name(src->Name());
		dst->Ambient(src->Ambient());
		dst->Diffuse(src->Diffuse());
		dst->Specular(src->Specular());

		if (src->DiffuseMap())
			dst->DiffuseMap(src->DiffuseMap()->GetFile());
		if (src->SpecularMap())
			dst->SpecularMap(src->SpecularMap()->GetFile());
		if (src->NormalMap())
			dst->DiffuseMap(src->NormalMap()->GetFile());

		(*out)->materials.push_back(dst);
	}

	// Copy Bone
	for (ModelBone* src : bones)
	{
		ModelBone* dst = new ModelBone();
		dst->name = src->name;
		dst->transform = src->transform;
		memcpy(&dst->transform, &src->transform, sizeof(Matrix));
		dst->index = src->index;
		dst->parentIndex = src->parentIndex;

		if (src->parent != nullptr)
		{
			dst->parent = (*out)->bones[dst->parentIndex];
			dst->parent->childs.push_back(dst);
		}
		else
		{
			// 루트 노드
			dst->parent = nullptr;
			(*out)->root = dst;
		}

		(*out)->bones.push_back(dst);
	}

	// Copy Mesh
	for (ModelMesh* src : meshes)
	{
		ModelMesh* dst = new ModelMesh();

		dst->name = src->name;
		dst->boneIndex = src->boneIndex;
		dst->materialName = src->materialName;

		dst->vertexCount = src->vertexCount;
		dst->vertices = new ModelVertex[dst->vertexCount];
		memcpy(dst->vertices, src->vertices, dst->vertexCount * sizeof(ModelVertex));

		dst->indexCount = src->indexCount;
		dst->indices = new UINT[dst->indexCount];
		memcpy(dst->indices, src->indices, dst->indexCount * sizeof(UINT));

		dst->bone = (*out)->bones[dst->boneIndex];
		dst->Binding(*out);
		dst->SetShader(src->shader);

		(*out)->meshes.push_back(dst);
	}

	// Copy Clip
	for (ModelClip* src : clips)
	{
		ModelClip* dst = new ModelClip();
		dst->name = src->name;
		dst->duration = src->duration;
		dst->frameRate = src->frameRate;
		dst->frameCount = src->frameCount;

		for (auto i = src->keyframeMap.cbegin(); i != src->keyframeMap.cend(); i++)
		{		
			ModelKeyframe* dstFrame = new ModelKeyframe();
			dstFrame->BoneName = (*i).second->BoneName;
			dstFrame->Transforms = (*i).second->Transforms;
			dst->keyframeMap[dstFrame->BoneName] = dstFrame;
		}

		(*out)->clips.push_back(dst);
	}
}


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
		Material* material = new Material();
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

		bone->index = r.Int();
		bone->name = String::ToWString(r.String());
		bone->parentIndex = r.Int();
		bone->transform = r.Matrix();

		bones.push_back(bone);
	}

	// ModelMesh
	count = r.UInt();
	for (UINT i = 0; i < count; i++)
	{
		ModelMesh* mesh = new ModelMesh();

		mesh->name = String::ToWString(r.String());
		mesh->boneIndex = r.Int();

		mesh->materialName = String::ToWString(r.String());

		// VertexData
		{
			UINT count = r.UInt();

			vector<Model::ModelVertex> vertices;
			vertices.assign(count, Model::ModelVertex());

			void* ptr = (void*)&(vertices[0]);
			r.BYTE(&ptr, sizeof(Model::ModelVertex) * count);

			mesh->vertices = new Model::ModelVertex[count];
			mesh->vertexCount = count;

			copy(
				vertices.begin(), vertices.end(),
				stdext::checked_array_iterator<Model::ModelVertex*>(mesh->vertices, count)
			);
		}

		// Index Data
		{
			UINT count = r.UInt();

			vector<UINT> indices;
			indices.assign(count, UINT());

			void* ptr = (void*)&(indices[0]);
			r.BYTE(&ptr, sizeof(UINT) * count);

			mesh->indices = new UINT[count];
			mesh->indexCount = count;

			copy(
				indices.begin(), indices.end(),
				stdext::checked_array_iterator<UINT*>(mesh->indices, count)
			);
		}

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

	ModelClip* clip = new ModelClip();

	clip->name = String::ToWString(r.String());
	clip->duration = r.Float();
	clip->frameRate = r.Float();
	clip->frameCount = r.UInt();

	UINT keyframesCount = r.UInt();
	for (UINT i = 0; i < keyframesCount; i++)
	{
		ModelKeyframe* keyframe = new ModelKeyframe();
		keyframe->BoneName = String::ToWString(r.String());

		UINT size = r.UInt();
		if (size > 0)
		{
			keyframe->Transforms.assign(size, ModelKeyframeData());

			void* ptr = (void*)keyframe->Transforms.data();
			r.BYTE(&ptr, sizeof(ModelKeyframeData) * size);
		}

		clip->keyframeMap[keyframe->BoneName] = keyframe;
	}

	r.Close();

	clips.push_back(clip);

}

#pragma endregion


#pragma region Getter

// Material
Material * Model::MaterialByName(wstring name)
{
	for (Material* material : materials)
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
		if (bone->Name() == name)
			return bone;
	}

	return nullptr;
}

// ModelMesh
ModelMesh * Model::MeshByName(wstring name)
{
	for (ModelMesh* mesh : meshes)
	{
		if (mesh->Name() == name)
			return mesh;
	}

	return nullptr;
}

ModelClip * Model::ClipByName(wstring name)
{
	for (ModelClip* clip : clips)
	{
		if (clip->name == name)
			return clip;
	}

	return nullptr;
}

#pragma endregion


#pragma region Bind

void Model::BindBone()
{
	root = bones[0];
	for (ModelBone* bone : bones)
	{
		if (bone->parentIndex > -1)
		{
			bone->parent = bones[bone->parentIndex];
			bone->parent->childs.push_back(bone);
		}
		else
			bone->parent = nullptr;
	}
}

void Model::BindMesh()
{
	// 메쉬 그려질 위치 찾기
	for (ModelMesh* mesh : meshes)
	{
		for (ModelBone* bone : bones)
		{
			if (mesh->boneIndex == bone->index)
			{
				mesh->bone = bone;

				break;
			}
		}

		mesh->Binding(this);
	}
}

#pragma endregion


