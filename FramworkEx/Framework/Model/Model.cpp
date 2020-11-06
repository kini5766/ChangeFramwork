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
	for (ModelMaterial* mat : materials)
		SafeDelete(mat);

	for (ModelBone* d : bones)
		SafeDelete(d);

	for (MeshData* d : meshes)
		SafeDelete(d);

	for (ModelClip* d : clips)
		SafeDelete(d);
}

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
		t = src->GetTexture("DiffuseMap");
		if (t != nullptr)
			dst->SetTexture("DiffuseMap", t->GetFile());
		else dst->SetTexture("DiffuseMap", nullptr);

		t = src->GetTexture("SpecularMap");
		if (t != nullptr)
			dst->SetTexture("SpecularMap", t->GetFile());
		else dst->SetTexture("SpecularMap", nullptr);

		t = src->GetTexture("NormalMap");
		if (t != nullptr)
			dst->SetTexture("NormalMap", t->GetFile());
		else dst->SetTexture("NormalMap", nullptr);

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
			memcpy(&dst->transform, src->transform, sizeof(Matrix));

			// 보정 값 적용
			if (offset != nullptr)
				dst->transform = dst->transform * offset->World();

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
	for (MeshData* src : model->Meshes())
	{
		MeshData* dst = new MeshData();

		dst->NewBindValue();
		dst->PBind->MaterialName = src->PBind->MaterialName;
		dst->PBind->Name = src->PBind->Name;

		for (BoneNum swap : changes)
		{
			if (swap.oldNum == src->PBind->BoneIndex)
			{
				dst->PBind->BoneIndex = swap.newNum;
				dst->PBind->PBone = bones[dst->PBind->BoneIndex];
				break;
			}
		}

		Model::ModelVertex* vertices = new Model::ModelVertex[src->VertexCount];

		UINT verticesSize = src->VertexCount * sizeof(Model::ModelVertex);
		memcpy_s(vertices, verticesSize, src->Vertices, verticesSize);

		dst->VertexCount = src->VertexCount;
		dst->Vertices = vertices;
		dst->Stride = sizeof(Model::ModelVertex);

		dst->NewIndices(src->IndexCount);
		memcpy(dst->Indices, src->Indices, dst->IndexCount * sizeof(UINT));

		//dst->Binding(this);
		//dst->SetShader(shader);

		meshes.push_back(dst);
	}
}

void Model::AttachWeakly(Model * model, int parentBoneIndex, Transform * offset)
{
	// CopyParentBone
	ModelBone* newParent = new ModelBone();
	ModelBone* srcBone = BoneByIndex(parentBoneIndex);
	{
		newParent->name = srcBone->name;
		newParent->index = 0;
		newParent->parentIndex = -1;
		newParent->parent = nullptr;

		Matrix m;
		memcpy(&m, &srcBone->transform, sizeof(Matrix));

		
		ModelBone* curr = srcBone->parent;
		while (curr != nullptr)
		{
			m = m * curr->transform;
			curr = curr->parent;
		}
		

		memcpy(&newParent->transform, &m, sizeof(Matrix));
	}

	// Change Bone Index
	for (ModelBone* bone : model->Bones())
	{
		// 보정 값 적용
		if (offset != nullptr)
			bone->transform = bone->transform * offset->World();

		if (bone->parent != nullptr)
		{
			// 번호 교체
			++bone->index;
			++bone->parentIndex;
		}
		else
		{
			// 루트 노드 교체
			bone->parentIndex = 0;
			bone->parent = newParent;
			bone->parent->childs.push_back(bone);
		}
	}

	model->bones.insert(model->bones.begin(), newParent);

	// Change Bone Index (Mesh)
	for (MeshData* mesh : model->Meshes())
		++mesh->PBind->BoneIndex;


	// Copy Clip
	for (ModelClip* src : clips)
	{
		ModelClip* dst = new ModelClip();
		dst->name = src->name;
		dst->duration = src->duration;
		dst->frameRate = src->frameRate;
		dst->frameCount = src->frameCount;

		ModelKeyframe* dstFrame = new ModelKeyframe();
		dstFrame->BoneName = newParent->name;
		for (UINT f = 0; f < src->FrameCount(); f++)
		{
			Matrix m;
			D3DXMatrixIdentity(&m);

			ModelBone* curr = srcBone;
			while (curr != nullptr)
			{
				ModelKeyframeData& data = src->keyframeMap[curr->name]->Transforms[f];

				Matrix S, R, T;
				D3DXMatrixScaling(&S, data.Scale.x, data.Scale.y, data.Scale.z);
				D3DXMatrixRotationQuaternion(&R, &data.Rotation);
				D3DXMatrixTranslation(&T, data.Translation.x, data.Translation.y, data.Translation.z);
				
				Matrix animation = S * R * T;
				m = m * animation;

				curr = curr->parent;
			}

			ModelKeyframeData dstData;
			D3DXMatrixDecompose(&dstData.Scale, &dstData.Rotation, &dstData.Translation, &m);

			dstFrame->Transforms.push_back(dstData);
		}
		dst->keyframeMap[dstFrame->BoneName] = dstFrame;

		(*model).clips.push_back(dst);
	}
}

void Model::Copy(Model * out)
{
	// Copy Material
	for (ModelMaterial* src : materials)
	{
		ModelMaterial* dst = new ModelMaterial();
		dst->Name(src->Name());
		dst->Ambient(src->Ambient());
		dst->Diffuse(src->Diffuse());
		dst->Specular(src->Specular());

		Texture* t;
		t = src->GetTexture("DiffuseMap");
		if (t != nullptr)
			dst->SetTexture("DiffuseMap", t->GetFile());
		else dst->SetTexture("DiffuseMap", nullptr);

		t = src->GetTexture("SpecularMap");
		if (t != nullptr)
			dst->SetTexture("SpecularMap", t->GetFile());
		else dst->SetTexture("SpecularMap", nullptr);

		t = src->GetTexture("NormalMap");
		if (t != nullptr)
			dst->SetTexture("NormalMap", t->GetFile());
		else dst->SetTexture("NormalMap", nullptr);

		(*out).materials.push_back(dst);
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
			dst->parent = (*out).bones[dst->parentIndex];
			dst->parent->childs.push_back(dst);
		}
		else
		{
			// 루트 노드
			dst->parent = nullptr;
			(*out).root = dst;
		}

		(*out).bones.push_back(dst);
	}

	// Copy Mesh
	for (MeshData* src : meshes)
	{
		MeshData* dst = new MeshData();

		dst->NewBindValue();
		dst->PBind->Name = src->PBind->Name;
		dst->PBind->BoneIndex = src->PBind->BoneIndex;
		dst->PBind->MaterialName = src->PBind->MaterialName;

		dst->VertexCount = src->VertexCount;
		dst->Vertices = new Model::ModelVertex[src->VertexCount];
		memcpy(dst->Vertices, src->Vertices, dst->VertexCount * sizeof(ModelVertex));

		dst->IndexCount = src->IndexCount;
		dst->Indices = new UINT[dst->IndexCount];
		memcpy(dst->Indices, src->Indices, dst->IndexCount * sizeof(UINT));

		//dst->bone = (*out)->bones[dst->boneIndex];
		//dst->Binding(*out);
		//dst->SetShader(src->shader);

		(*out).meshes.push_back(dst);
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

		(*out).clips.push_back(dst);
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
			material->SetTexture("DiffuseMap", directory + texture);
		else material->SetTexture("DiffuseMap", nullptr);

		node = node->NextSiblingElement();
		texture = String::ToWString(node->GetText());
		if (texture.length() > 0)
			material->SetTexture("SpecularMap", directory + texture);
		else material->SetTexture("SpecularMap", nullptr);

		node = node->NextSiblingElement();
		texture = String::ToWString(node->GetText());
		if (texture.length() > 0)
			material->SetTexture("NormalMap", directory + texture);
		else material->SetTexture("NormalMap", nullptr);

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
		MeshData* mesh = new MeshData();

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

			Model::ModelVertex* dstVertices = new Model::ModelVertex[count];
			copy(
				vertices.begin(), vertices.end(),
				stdext::checked_array_iterator<Model::ModelVertex*>(dstVertices, count)
			);

			mesh->VertexCount = count;
			mesh->Stride = sizeof(Model::ModelVertex);
			mesh->Vertices = dstVertices;
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
		if (bone->Name() == name)
			return bone;
	}

	return nullptr;
}

// ModelMesh
MeshData * Model::MeshByName(wstring name)
{
	for (MeshData* mesh : meshes)
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
	for (MeshData* mesh : meshes)
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


