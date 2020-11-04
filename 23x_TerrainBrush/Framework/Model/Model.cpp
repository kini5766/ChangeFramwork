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

	for (ModelMeshData* d : meshes)
		SafeDelete(d);

	for (ModelClip* d : clips)
		SafeDelete(d);
}

void Model::Attach(Shader * shader, Model * model, int parentBoneIndex, Transform * offset)
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
			if (offset != nullptr)
				dst->transform = dst->transform * offset->World();
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
	for (ModelMeshData* src : model->Meshes())
	{
		ModelMeshData* dst = new ModelMeshData();

		dst->NewBindValue();
		memcpy(dst->PBind, src->PBind, sizeof(BindValue));

		for (BoneNum swap : changes)
		{
			if (swap.oldNum == src->PBind->BoneIndex)
			{
				dst->PBind->BoneIndex = swap.newNum;
				break;
			}
		}


		UINT verticesSize = src->VertexCount * sizeof(Model::ModelVertex);
		dst->NewVertices(src->VertexCount);
		//UINT count = src->VertexCount;
		//dst->VertexCount = count;
		//dst->NewPositions();
		//dst->NewUvs();
		//dst->NewNormals();
		//dst->NewTangents();
		//dst->NewBoneWeights();
		memcpy_s(dst->Vertices, verticesSize, src->Vertices, verticesSize);
		//memcpy(dst->Positions, src->Positions,		count * sizeof(Vector3));
		//memcpy(dst->Uvs, src->Uvs,					count * sizeof(Vector2));
		//memcpy(dst->Normals, src->Normals,			count * sizeof(Vector3));
		//memcpy(dst->Tangents, src->Tangents,		count * sizeof(Vector3));
		//memcpy(dst->BoneWeights, src->BoneWeights,	count * sizeof(BoneWeight));

		dst->NewIndices(src->IndexCount);
		//memcpy_s(dst->indices, indicesSize, src->indices, indicesSize);
		memcpy(dst->Indices, src->Indices, dst->IndexCount * sizeof(UINT));

		//dst->Binding(this);
		//dst->SetShader(shader);

		meshes.push_back(dst);
	}

}

/*
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
	for (MeshData* src : meshes)
	{
		MeshData* dst = new MeshData();

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
*/

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
		ModelMeshData* mesh = new ModelMeshData();

		mesh->NewBindValue();
		mesh->PBind->Name = String::ToWString(r.String());
		mesh->PBind->BoneIndex = r.Int();

		mesh->PBind->MaterialName = String::ToWString(r.String());

		// VertexData
		{
			UINT count = r.UInt();

			vector<Model::ModelVertex> vertices;
			vertices.assign(count, Model::ModelVertex());

			void* ptr = (void*)&(vertices[0]);
			r.BYTE(&ptr, sizeof(Model::ModelVertex) * count);

			mesh->NewVertices(count);
			copy(
				vertices.begin(), vertices.end(),
				stdext::checked_array_iterator<Model::ModelVertex*>(mesh->Vertices, count)
			);

			/*
			mesh->VertexCount = count;
			mesh->NewPositions();
			mesh->NewUvs();
			mesh->NewNormals();
			mesh->NewTangents();
			mesh->NewBoneWeights();

			for (UINT i = 0; i < count; i++)
			{
				memcpy(&mesh->Positions[i], &vertices[i].Position, sizeof(Vector3));
				memcpy(&mesh->Uvs[i], &vertices[i].Uv, sizeof(Vector2));
				memcpy(&mesh->Normals[i], &vertices[i].Normal, sizeof(Vector3));
				memcpy(&mesh->Tangents[i], &vertices[i].Tangent, sizeof(Vector3));
				memcpy(&mesh->BoneWeights[i].BlendIndices, &vertices[i].BlendIndices, sizeof(Vector4));
				memcpy(&mesh->BoneWeights[i].BlendWeights, &vertices[i].BlendWeights, sizeof(Vector4));
			}
			*/
		}

		// Index Data
		{
			UINT count = r.UInt();

			vector<UINT> indices;
			indices.assign(count, UINT());

			void* ptr = (void*)&(indices[0]);
			r.BYTE(&ptr, sizeof(UINT) * count);

			mesh->Indices = new UINT[count];
			mesh->IndexCount = count;

			copy(
				indices.begin(), indices.end(),
				stdext::checked_array_iterator<UINT*>(mesh->Indices, count)
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
ModelMeshData * Model::MeshByName(wstring name)
{
	for (ModelMeshData* mesh : meshes)
	{
		if (mesh->PBind->Name == name)
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
	for (ModelMeshData* mesh : meshes)
	{
		for (ModelBone* bone : bones)
		{
			if (mesh->PBind->BoneIndex == bone->index)
			{
				mesh->PBind->PBone = bone;

				break;
			}
		}
	}
}

#pragma endregion


