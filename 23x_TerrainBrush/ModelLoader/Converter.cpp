#include "stdafx.h"
#include "Converter.h"
#include "Types.h"
#include "Utilities/Xml.h"
#include "Utilities/BinaryFile.h"

Converter::Converter()
{
	importer = new Assimp::Importer();
}

Converter::~Converter()
{
	SafeDelete(importer);  // ���� ���� ����Ʈ��
	//for (asMaterial* d : materials)
	//	SafeDelete(d);
	//for (asBone* d : bones)
	//	SafeDelete(d);
	//for (asMesh* d : meshes)
	//	SafeDelete(d);
}

void Converter::ReadFile(wstring _file)
{
	file = URI::Assets + _file;
	scene = importer->ReadFile
	(
		String::ToString(file),
		aiProcess_ConvertToLeftHanded |
		aiProcess_Triangulate |
		aiProcess_GenUVCoords |
		aiProcess_GenNormals |
		aiProcess_CalcTangentSpace  // ��ָ� ����ϱ� ����
	);

	assert(scene != nullptr);
}

void Converter::ExportMaterial(wstring savePath, bool bOverWrite)
{
	savePath = URI::Textures + savePath + L".material";  // xml�̿�

	ReadMaterial();
	WriteMaterial(savePath, bOverWrite);
}

void Converter::ExportMesh(wstring savePath, bool bOverWrite)
{
	savePath = URI::Models + savePath + L".mesh";

	//ReadBoneData(scene->mRootNode, -1, -1);  // ���(����)
	ReadBoneData(scene->mRootNode, 0, -1);  // ���
	ReadSkinData();
	WriteMeshData(savePath, bOverWrite);
}

void Converter::ClipList(vector<wstring>* list)
{
	for (UINT i = 0; i < scene->mNumAnimations; i++)
	{
		aiAnimation* anim = scene->mAnimations[i];
		list->push_back(String::ToWString(anim->mName.C_Str()));
	}
}

void Converter::ExportAnimClip(UINT index, wstring savePath, bool bOverWrite)
{
	savePath = URI::Models + savePath + L".clip";

	asClip* clip = ReadClipData(scene->mAnimations[index]);
	WriteClipData(clip, savePath, bOverWrite);
}


#pragma region Material

void Converter::ReadMaterial()
{
	for (UINT i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* srcMaterial = scene->mMaterials[i];
		asMaterial* material = new asMaterial();

		material->Name = srcMaterial->GetName().C_Str();

		{
			aiColor3D color;  // rgb�� ����

			srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);  // ��ũ�� ����
			material->Ambient = Color(color.r, color.g, color.b, 1.0f);

			srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
			material->Diffuse = Color(color.r, color.g, color.b, 1.0f);

			srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
			material->Specular = Color(color.r, color.g, color.b, 1.0f);

			srcMaterial->Get(AI_MATKEY_SHININESS, material->Specular.a);
		}

		{
			aiString file;

			srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
			material->DiffuseFile = file.C_Str();

			srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
			material->SpecularFile = file.C_Str();

			srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
			material->NormalFile = file.C_Str();
		}

		materials.push_back(material);
	}
}

void Converter::WriteMaterial(wstring savePath, bool bOverWrite)
{
	if (bOverWrite == false)
	{
		if (Path::ExistFile(savePath))
		{
			// �ߺ��� ����
			//assert(false);
			return;
		}
	}

	string folder = String::ToString(Path::GetDirectoryName(savePath));
	string file = String::ToString(Path::GetFileName(savePath));

	Path::CreateFolders(folder);

	Xml::XMLDocument* document = new Xml::XMLDocument();

	Xml::XMLDeclaration* decl = document->NewDeclaration();
	document->LinkEndChild(decl);

	// <Materials xmlns:xsi = "http://www.w3.org/2001/XMLSchema-instance" xmlns:xsd = "http://www.w3.org/2001/XMLSchema"/>
	Xml::XMLElement* root = document->NewElement("Materials");  // �±�
	// SetAttribute : �±� �ȿ� �Ӽ� ��
	root->SetAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
	root->SetAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema");
	document->LinkEndChild(root);

	for (asMaterial* mat : materials)
	{
		// <Material>
		Xml::XMLElement* node = document->NewElement("Material");
		root->LinkEndChild(node);

		Xml::XMLElement* element = nullptr;

		// <Name>
		element = document->NewElement("Name");
		element->SetText(mat->Name.c_str());
		node->LinkEndChild(element);
		// </Name>

		// <DiffuseFile>
		element = document->NewElement("DiffuseFile");
		element->SetText(WriteTexture(folder, mat->DiffuseFile).c_str());
		node->LinkEndChild(element);
		// </DiffuseFile>

		// <SpecularFile>
		element = document->NewElement("SpecularFile");
		element->SetText(WriteTexture(folder, mat->SpecularFile).c_str());
		node->LinkEndChild(element);
		// </SpecularFile>

		// <NormalFile>
		element = document->NewElement("NormalFile");
		element->SetText(WriteTexture(folder, mat->NormalFile).c_str());
		node->LinkEndChild(element);
		// </NormalFile>

		// <Ambient R G B A />
		element = document->NewElement("Ambient");
		element->SetAttribute("R", mat->Ambient.r);
		element->SetAttribute("G", mat->Ambient.g);
		element->SetAttribute("B", mat->Ambient.b);
		element->SetAttribute("A", mat->Ambient.a);
		node->LinkEndChild(element);

		// <Diffuse R G B A />
		element = document->NewElement("Diffuse");
		element->SetAttribute("R", mat->Diffuse.r);
		element->SetAttribute("G", mat->Diffuse.g);
		element->SetAttribute("B", mat->Diffuse.b);
		element->SetAttribute("A", mat->Diffuse.a);
		node->LinkEndChild(element);

		// <Specular R G B A />
		element = document->NewElement("Specular");
		element->SetAttribute("R", mat->Specular.r);
		element->SetAttribute("G", mat->Specular.g);
		element->SetAttribute("B", mat->Specular.b);
		element->SetAttribute("A", mat->Specular.a);
		node->LinkEndChild(element);

		// </Material>
		SafeDelete(mat);
	}

	document->SaveFile((folder + file).c_str());
	SafeDelete(document);
}

string Converter::WriteTexture(string savePath, string file)
{
	if (file.length() < 1)
	{
		// ���� ����
		// assert(false);
		return "";
	}

	string fileName = Path::GetFileName(file);
	const aiTexture* texture = scene->GetEmbeddedTexture(file.c_str());

	string path = "";
	if (texture != nullptr)
	{
		// �𵨿� �ؽ��İ� ����

		path = savePath + Path::GetFileNameWithoutExtension(file) + ".png";

		if (texture->mHeight < 1)
		{
			// ������ ������
			BinaryWriter w;
			w.Open(String::ToWString(path));
			w.BYTE(texture->pcData, texture->mWidth);
			w.Close();
		}
		else
		{
			// ������ ������
			ID3D11Texture2D* dest;
			D3D11_TEXTURE2D_DESC destDesc;
			ZeroMemory(&destDesc, sizeof(D3D11_TEXTURE2D_DESC));
			destDesc.Width = texture->mWidth;
			destDesc.Height = texture->mHeight;
			destDesc.MipLevels = 1;  // 1�� �⺻ ��
			destDesc.ArraySize = 1;  // Texture2DArray�� ����
			destDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // ����Ʈ�� ũ��
			destDesc.SampleDesc.Count = 1;
			destDesc.SampleDesc.Quality = 0;
			destDesc.Usage = D3D11_USAGE_IMMUTABLE;  // ���ľ��� �Ұ�

			D3D11_SUBRESOURCE_DATA subResorce = { 0 };
			subResorce.pSysMem = texture->pcData;

			HRESULT hr;
			hr = D3D::GetDevice()->CreateTexture2D(&destDesc, &subResorce, &dest);
			assert(SUCCEEDED(hr));

			// ����
			D3DX11SaveTextureToFileA(D3D::GetDC(), dest, D3DX11_IFF_PNG, savePath.c_str());
		}
	}
	else
	{
		// �𵨰� �ؽ��İ� �и�
		string directory = Path::GetDirectoryName(String::ToString(this->file));
		string origin = directory + file;
		String::Replace(&origin, "\\", "/");

		if (Path::ExistFile(origin) == false)
		{
			// ���� ���� (���� ������ �ִ� ���)
			return "";
		}

		path = savePath + fileName;

		// ����
		CopyFileA(origin.c_str(), path.c_str(), FALSE);

		String::Replace(&path, String::ToString(URI::Textures), "");
	}

	return Path::GetFileName(path);
}

#pragma endregion


#pragma region Mesh

void Converter::ReadBoneData(aiNode * node, int index, int parent)
{
	asBone* bone = new asBone();
	bone->Index = index;
	bone->Parent = parent;
	bone->Name = node->mName.C_Str();

	Matrix transform(node->mTransformation[0]);  // <- �����ּҷ� ����
	// ��켱���(������) -> ���켱���(�޼�)�� ��ġ
	D3DXMatrixTranspose(&bone->Transform, &transform);

	// �θ� ����
	Matrix matParent;
	if (parent < 0)
		D3DXMatrixIdentity(&matParent);  // �ֻ��� ���
	else
		matParent = bones[parent]->Transform;

	bone->Transform = bone->Transform * matParent;  // �ڽ� �θ� ��

	bones.push_back(bone);

	ReadMeshData(node, index);

	for (UINT i = 0; i < node->mNumChildren; i++)  // mNumChildren : �ڽ� ����
		ReadBoneData(node->mChildren[i], bones.size(), index);  // �θ� �ڽĵ鿡 ���� ���
}

void Converter::ReadMeshData(aiNode * node, int bone)
{
	if (node->mNumMeshes < 1) return;  // �޽����� ��

	asMesh* mesh = new asMesh();
	// �޽� �̸�
	mesh->Name = node->mName.C_Str();
	// �޽� �� �ε���
	mesh->BoneIndex = bone;

	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		UINT index = node->mMeshes[i];
		aiMesh* srcMesh = scene->mMeshes[index];

		// �޽� ���׸��� �̸�
		aiMaterial* material = scene->mMaterials[srcMesh->mMaterialIndex];
		mesh->MaterialName = material->GetName().C_Str();

		// �޽� ����
		UINT startVertex = mesh->Vertices.size();  // ��ó�� 0��
		for (UINT v = 0; v < srcMesh->mNumVertices; v++)
		{
			// ���� ��ġ
			Model::ModelVertex vertex;
			memcpy(&vertex.Position, &srcMesh->mVertices[v], sizeof(Vector3));

			// UV��ǥ
			if (srcMesh->HasTextureCoords(0))
				memcpy(&vertex.Uv, &srcMesh->mTextureCoords[0][v], sizeof(Vector2));  // mTextureCoords[0] uvw�� ��� 1�� �÷�����

			// ��ֺ���
			if (srcMesh->HasNormals())
				memcpy(&vertex.Normal, &srcMesh->mNormals[v], sizeof(Vector3));

			// ź��Ʈ
			if (srcMesh->HasTangentsAndBitangents())
				memcpy(&vertex.Tangent, &srcMesh->mTangents[v], sizeof(Vector3));

			mesh->Vertices.push_back(vertex);
		}

		// �޽� ���� �ε���
		for (UINT f = 0; f < srcMesh->mNumFaces; f++)
		{
			// srcMesh���� 012, 012�θ� ���� (��ȯ �ʿ�)
			aiFace& face = srcMesh->mFaces[f];

			for (UINT k = 0; k < face.mNumIndices; k++)
			{
				mesh->Indices.push_back(face.mIndices[k]);
				mesh->Indices.back() += startVertex;  // 012, 345
			}
		}

		//meshes.push_back(mesh);
	}

	meshes.push_back(mesh);
}

void Converter::WriteMeshData(wstring savePath, bool bOverWrite)
{
	if (bOverWrite == false)
	{
		if (Path::ExistFile(savePath))
			return;
	}

	Path::CreateFolders(Path::GetDirectoryName(savePath));

	BinaryWriter w;
	w.Open(savePath);

	// Bones
	w.UInt(bones.size());
	for (asBone* bone : bones)
	{
		w.Int(bone->Index);
		w.String(bone->Name);
		w.Int(bone->Parent);
		w.Matrix(bone->Transform);

		SafeDelete(bone);
	}
	
	// Meshes
	w.UInt(meshes.size());
	for (asMesh* data : meshes)
	{
		w.String(data->Name);
		w.Int(data->BoneIndex);
		w.String(data->MaterialName);
		w.UInt(data->Vertices.size());
		w.BYTE(data->Vertices.data(), sizeof(Model::ModelVertex) * data->Vertices.size());
		//w.BYTE(&data->Vertices[0], sizeof(Model::ModelVertex) * data->Vertices.size());
		w.UInt(data->Indices.size());
		w.BYTE(data->Indices.data(), sizeof(UINT) * data->Indices.size());
		//w.BYTE(&data->Indices[0], sizeof(UINT) * data->Indices.size());

		SafeDelete(data);
	}

	w.Close();
}

void Converter::ReadSkinData()
{
	for (UINT i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* aiMesh = scene->mMeshes[i];

		if (aiMesh->HasBones() == false) continue;  // ������ �޽�(�ִϸ��̼Ǿ���)

		asMesh* mesh = meshes[i];

		vector<asBoneWeights> boneWeights;
		boneWeights.assign(mesh->Vertices.size(), asBoneWeights());

		for (UINT b = 0; b < aiMesh->mNumBones; b++)
		{
			aiBone* aiBone = aiMesh->mBones[b];

			UINT boneIndex = 0;
			for (asBone* bone : bones)
			{
				if (bone->Name == aiBone->mName.C_Str())
				{
					boneIndex = bone->Index;
					break;
				}
			}

			// ���� ����ġ �ʱ�ȭ
			for (UINT w = 0; w < aiBone->mNumWeights; w++)
			{
				UINT index = aiBone->mWeights[w].mVertexId;
				float weight = aiBone->mWeights[w].mWeight;

				boneWeights[index].AddWeights(boneIndex, weight);
			}
		}

		for (UINT w = 0; w < boneWeights.size(); w++)
		{
			boneWeights[w].Normalize();
			asBlendWeight blendWieght;
			boneWeights[w].GetBlendWeights(blendWieght);

			mesh->Vertices[w].BlendIndices = blendWieght.Indices;
			mesh->Vertices[w].BlendWeights = blendWieght.Weights;
		}
	}
}

#pragma endregion


#pragma region Animation

asClip * Converter::ReadClipData(aiAnimation * animation)
{
	asClip* clip = new asClip();
	clip->Name = animation->mName.C_Str();
	clip->FrameRate = (float)animation->mTicksPerSecond;
	clip->FrameCount = (UINT)animation->mDuration + 1;

	// ä�κи�
	vector<asClipNode> aniNodeInfos;
	for (UINT i = 0; i < animation->mNumChannels; i++)
	{
		aiNodeAnim* aiNode = animation->mChannels[i];

		asClipNode aniNodeInfo;
		aniNodeInfo.Name = aiNode->mNodeName;  // �� �ϳ��� ����

		// mNumPositionKeys mNumRotationKeys mNumScalingKeys
		// ���� Ű���� �и��� �� �� �� ������ ������ ���
		UINT keyCount = max(aiNode->mNumPositionKeys, aiNode->mNumRotationKeys);
		keyCount = max(keyCount, aiNode->mNumScalingKeys);

		// �и��� Ű���� ��Ʈ������ ��ġ��
		asKeyframeData frameData;
		for (UINT k = 0; k < keyCount; k++)
		{
			bool bFound = false;
			UINT t = aniNodeInfo.Keyframe.size();

			// ������Ű
			// aiNode->mPositionKeys[k].mTime == t -> �ε� �Ҽ��� ���� ������ ���񰪿� D3DX_16F_EPSILON ���
			if (fabsf((float)aiNode->mPositionKeys[k].mTime - (float)t) <= D3DX_16F_EPSILON)
			{
				aiVectorKey key = aiNode->mPositionKeys[k];
				memcpy_s(&frameData.Translation, sizeof(Vector3), &key.mValue, sizeof(aiVector3D));  // ũ�Ⱑ �ٸ� �� �־ _s ���
				frameData.Time = (float)aiNode->mPositionKeys[k].mTime;

				bFound = true;
			}

			// �����̼�Ű
			if (fabsf((float)aiNode->mRotationKeys[k].mTime - (float)t) <= D3DX_16F_EPSILON)
			{
				aiQuatKey key = aiNode->mRotationKeys[k];
				// memcpy_s(&frameData.rotation, sizeof(Quaternion), &key.mValue, sizeof(aiQuaternion));
				frameData.Rotation.x = key.mValue.x;
				frameData.Rotation.y = key.mValue.y;
				frameData.Rotation.z = key.mValue.z;
				frameData.Rotation.w = key.mValue.w;
				frameData.Time = (float)aiNode->mRotationKeys[k].mTime;

				bFound = true;
			}

			// ������Ű
			if (fabsf((float)aiNode->mScalingKeys[k].mTime - (float)t) <= D3DX_16F_EPSILON)
			{
				aiVectorKey key = aiNode->mScalingKeys[k];
				memcpy_s(&frameData.Scale, sizeof(Vector3), &key.mValue, sizeof(aiVector3D));
				frameData.Time = (float)aiNode->mScalingKeys[k].mTime;

				bFound = true;
			}


			if (bFound)
				aniNodeInfo.Keyframe.push_back(frameData);
		}  // for k

		// ����ִ� ������ Ű �߰�
		if (aniNodeInfo.Keyframe.size() < clip->FrameCount)
		{
			UINT count = clip->FrameCount - aniNodeInfo.Keyframe.size();

			asKeyframeData keyFrame = aniNodeInfo.Keyframe.back();

			for (UINT n = 0; n < count; n++)
				aniNodeInfo.Keyframe.push_back(keyFrame);
		}

		// ������ clip->Duration�� ŭ
		clip->Duration = max(clip->Duration, aniNodeInfo.Keyframe.back().Time);

		aniNodeInfos.push_back(aniNodeInfo);

	}  // for i

	// ��� �Լ�
	ReadKeyframeData(clip, scene->mRootNode, aniNodeInfos);

	return clip;
}

// vector<asKeyframe*> asClip::Keyframes �����ϱ� ���� �Լ�
void Converter::ReadKeyframeData(asClip * clip, aiNode * node, vector<struct asClipNode>& aiNodeInfos)
{
	asKeyframe* keyFrame = new asKeyframe();
	keyFrame->BoneName = node->mName.C_Str();

	for (UINT i = 0; i < clip->FrameCount; i++)
	{
		asClipNode* asClipNode = nullptr;
		for (UINT n = 0; n < aiNodeInfos.size(); n++)
		{
			if (aiNodeInfos[n].Name == node->mName)
				asClipNode = &aiNodeInfos[n];
		}

		asKeyframeData frameData;
		if (asClipNode == nullptr)
		{
			// ���� �̸��� ���� ��
			// (�ܺο��� ���� ���̶� ��)
			Matrix transform(node->mTransformation[0]);
			// ��ġ
			D3DXMatrixTranspose(&transform, &transform);

			D3DXMatrixDecompose(&frameData.Scale, &frameData.Rotation, &frameData.Translation, &transform);
			frameData.Time = (float)i;
		}
		else
		{
			frameData = asClipNode->Keyframe[i];
		}
		keyFrame->Transforms.push_back(frameData);
	}

	clip->Keyframes.push_back(keyFrame);

	for (UINT i = 0; i < node->mNumChildren; i++)
		ReadKeyframeData(clip, node->mChildren[i], aiNodeInfos);
}

void Converter::WriteClipData(asClip * clip, wstring savePath, bool bOverWrite)
{
	if (bOverWrite == false)
	{
		if (Path::ExistFile(savePath))
			return;
	}

	Path::CreateFolders(Path::GetDirectoryName(savePath));
	BinaryWriter w;
	w.Open(savePath);
	
	w.String(clip->Name);
	w.Float(clip->Duration);
	w.Float(clip->FrameRate);
	w.UInt(clip->FrameCount);

	w.UInt(clip->Keyframes.size());
	for (asKeyframe* keyFrame : clip->Keyframes)
	{
		w.String(keyFrame->BoneName);

		w.UInt(keyFrame->Transforms.size());
		w.BYTE(keyFrame->Transforms.data(), sizeof(asKeyframeData) * keyFrame->Transforms.size());

		SafeDelete(keyFrame);
	}

	w.Close();
}

#pragma endregion

// ������ �
// SmoothStep
// lerp * 2