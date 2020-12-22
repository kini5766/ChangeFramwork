#include "Framework.h"
#include "ModelData.h"

#include "Utilities/BinaryFile.h"
#include "Utilities/Xml.h"

ModelData::ModelData()
{
}

ModelData::~ModelData()
{
	for (auto d : materials)
		SafeDelete(d);

	for (auto d : meshes)
	{
		d->Mesh->SafeDeleteData();
		SafeDelete(d->Mesh);
		SafeDelete(d);
	}

	for (auto d : clips)
	{
		for (auto b : d->Bones)
			SafeDelete(b);

		SafeDelete(d);
	}
}

UINT ModelData::MaterialIndexByName(wstring name)
{
	for (UINT i = 0; i < materials.size(); i++)
	{
		if (materials[i]->Name() == name)
		{
			return i;
		}
	}

	return materials.size();
}

#pragma region ReadFile

void ModelData::ReadMaterial(wstring file)
{
	file = URI::Textures + file + L".material";

	// document
	{
		Xml::XMLDocument document;
		Xml::XMLError error = document.LoadFile(String::ToString(file).c_str());
		assert(error == Xml::XML_SUCCESS);

		Xml::XMLElement* root = document.FirstChildElement();
		Xml::XMLElement* materialNode = root->FirstChildElement();

		do
		{
			// <Material>
			Material* material = new Material();

			// <Name>
			Xml::XMLElement* node = materialNode->FirstChildElement();
			material->Name(String::ToWString(node->GetText()));
			// </Name>

			wstring directory = Path::GetDirectoryName(file);
			String::Replace(&directory, URI::Textures, L"");

			wstring texture = L"";

			// <DiffuseFile>
			node = node->NextSiblingElement();
			texture = String::ToWString(node->GetText());
			if (texture.length() > 0)
				material->DiffuseMap(directory + texture);
			// </DiffuseFile>

			// <SpecularFile>
			node = node->NextSiblingElement();
			texture = String::ToWString(node->GetText());
			if (texture.length() > 0)
				material->SpecularMap(directory + texture);
			// </SpecularFile>

			// <NormalFile>
			node = node->NextSiblingElement();
			texture = String::ToWString(node->GetText());
			if (texture.length() > 0)
				material->NormalMap(directory + texture);
			// </NormalFile>

			Color color;

			// <Ambient>
			node = node->NextSiblingElement();
			color.r = node->FloatAttribute("R");
			color.g = node->FloatAttribute("G");
			color.b = node->FloatAttribute("B");
			color.a = node->FloatAttribute("A");
			material->Ambient(color);
			// </Ambient>

			// <Diffuse>
			node = node->NextSiblingElement();
			color.r = node->FloatAttribute("R");
			color.g = node->FloatAttribute("G");
			color.b = node->FloatAttribute("B");
			color.a = node->FloatAttribute("A");
			material->Diffuse(color);
			// </Diffuse>

			// <Specular>
			node = node->NextSiblingElement();
			color.r = node->FloatAttribute("R");
			color.g = node->FloatAttribute("G");
			color.b = node->FloatAttribute("B");
			color.a = node->FloatAttribute("A");
			material->Specular(color);
			// </Specular>

			// <Emissive>
			node = node->NextSiblingElement();
			color.r = node->FloatAttribute("R");
			color.g = node->FloatAttribute("G");
			color.b = node->FloatAttribute("B");
			color.a = node->FloatAttribute("A");
			material->Emissive(color);
			// </Emissive>

			materials.push_back(material);

			materialNode = materialNode->NextSiblingElement();
			// </Material>
		} while (materialNode != nullptr);
	}

}

void ModelData::ReadMesh(wstring file)
{
	file = URI::Models + file + L".mesh";
	BinaryReader r(file);

	UINT count = 0;

	count = r.UInt();
	for (UINT i = 0; i < count; i++)
	{
		ModelBoneData* data = new ModelBoneData();
		data->Index = r.Int();
		data->Name = String::ToWString(r.String());
		data->ParentIndex = r.Int();
		data->Transform = r.Matrix();

		bones.push_back(data);

	} // End Bone

	count = r.UInt();
	for (UINT i = 0; i < count; i++)
	{
		ModelMeshData* data = new ModelMeshData();
		data->Mesh = new MeshData();

		data->BoneIndex = r.Int();

		// Vertex
		UINT vertexCount = r.UInt();
		data->Mesh->NewVertices<VertexModel>(vertexCount);
		r.Byte(&data->Mesh->Vertices, data->Mesh->Stride * data->Mesh->VertexCount);

		// Index
		UINT indexCount = r.UInt();
		data->Mesh->NewIndices(indexCount);
		r.Byte((void**)&data->Mesh->Indices, sizeof(UINT) * data->Mesh->IndexCount);

		UINT partCount = r.UInt();
		data->Mesh->NewSubMesh(partCount);
		for (UINT k = 0; k < partCount; k++)
		{
			SubMeshData& part = data->Mesh->SubMeshes[k];
			part.MaterialIndex = MaterialIndexByName(String::ToWString(r.String()));

			part.StartVertex = r.UInt();
			part.VertexCount = r.UInt();
			part.StartIndex = r.UInt();
			part.IndexCount = r.UInt();

		} // End Part

		meshes.push_back(data);
	}// End Mesh
}

void ModelData::ReadClip(wstring file)
{
	file = URI::Models + file + L".clip";

	BinaryReader r(file);

	ModelClipData* clip = new ModelClipData();
	clip->Name = String::ToWString(r.String());
	clip->FrameRate = r.Float();
	clip->Duration = r.Float();

	UINT boneSize = r.UInt();
	for (UINT i = 0; i < boneSize; i++)
	{
		ClipBoneData* bone = new ClipBoneData();

		UINT count;

		// Position Key
		{
			count = r.UInt();
			bone->KeyTranslations.assign(count, KeyVector());

			void* ptr = (void*)bone->KeyTranslations.data();
			r.Byte(&ptr, sizeof(KeyVector) * count);
		}

		// Rotation Key
		{
			count = r.UInt();
			bone->KeyRotations.assign(count, KeyQuat());

			void* ptr = (void*)bone->KeyRotations.data();
			r.Byte(&ptr, sizeof(KeyQuat) * count);
		}

		// Scale Key
		{
			count = r.UInt();
			bone->KeyScales.assign(count, KeyVector());

			void* ptr = (void*)bone->KeyScales.data();
			r.Byte(&ptr, sizeof(KeyVector) * count);
		}

		bone->MaxFrame = r.UInt();

		clip->Bones.push_back(bone);
	}
	clips.push_back(clip);

}

#pragma endregion

