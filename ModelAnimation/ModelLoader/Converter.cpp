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
	SafeDelete(importer);  // 씬도 같이 딜리트됨
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
		aiProcess_CalcTangentSpace  // 노멀맵 사용하기 위해
	);

	assert(scene != nullptr);
}

void Converter::ExportMaterial(wstring savePath, bool bOverWrite)
{
	savePath = URI::Textures + savePath + L".material";  // xml이용

	ReadMaterial();
	WriteMaterial(savePath, bOverWrite);
}

void Converter::ExportMesh(wstring savePath, bool bOverWrite)
{
	savePath = URI::Models + savePath + L".mesh";

	//ReadBoneData(scene->mRootNode, -1, -1);  // 재귀(원본)
	ReadBoneData(scene->mRootNode, 0, -1);  // 재귀
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

	asExAnimation* clip = ReadClipData(scene->mAnimations[index]);
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
			aiColor3D color;  // rgb만 존재

			srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);  // 매크로 인자
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
			// 중복된 파일
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
	Xml::XMLElement* root = document->NewElement("Materials");  // 태그
	// SetAttribute : 태그 안에 속성 값
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
		// 파일 없음
		// assert(false);
		return "";
	}

	string fileName = Path::GetFileName(file);
	const aiTexture* texture = scene->GetEmbeddedTexture(file.c_str());

	string path = "";
	if (texture != nullptr)
	{
		// 모델에 텍스쳐가 포함

		path = savePath + Path::GetFileNameWithoutExtension(file) + ".png";

		if (texture->mHeight < 1)
		{
			// 일차원 데이터
			BinaryWriter w;
			w.Open(String::ToWString(path));
			w.BYTE(texture->pcData, texture->mWidth);
			w.Close();
		}
		else
		{
			// 이차원 데이터
			ID3D11Texture2D* dest;
			D3D11_TEXTURE2D_DESC destDesc;
			ZeroMemory(&destDesc, sizeof(D3D11_TEXTURE2D_DESC));
			destDesc.Width = texture->mWidth;
			destDesc.Height = texture->mHeight;
			destDesc.MipLevels = 1;  // 1은 기본 값
			destDesc.ArraySize = 1;  // Texture2DArray의 개수
			destDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;  // 바이트의 크기
			destDesc.SampleDesc.Count = 1;
			destDesc.SampleDesc.Quality = 0;
			destDesc.Usage = D3D11_USAGE_IMMUTABLE;  // 고쳐쓰기 불가

			D3D11_SUBRESOURCE_DATA subResorce = { 0 };
			subResorce.pSysMem = texture->pcData;

			HRESULT hr;
			hr = D3D::GetDevice()->CreateTexture2D(&destDesc, &subResorce, &dest);
			assert(SUCCEEDED(hr));

			// 저장
			D3DX11SaveTextureToFileA(D3D::GetDC(), dest, D3DX11_IFF_PNG, savePath.c_str());
		}
	}
	else
	{
		// 모델과 텍스쳐가 분리
		string directory = Path::GetDirectoryName(String::ToString(this->file));
		string origin = directory + file;
		String::Replace(&origin, "\\", "/");

		if (Path::ExistFile(origin) == false)
		{
			// 파일 없음 (별도 폴더에 있는 경우)
			return "";
		}

		path = savePath + fileName;

		// 복사
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

	Matrix transform(node->mTransformation[0]);  // <- 시작주소로 복사
	// 행우선행렬(오른손) -> 열우선행렬(왼손)로 전치
	D3DXMatrixTranspose(&bone->Transform, &transform);

	// 부모 설정
	Matrix matParent;
	if (parent < 0)
		D3DXMatrixIdentity(&matParent);  // 최상위 노드
	else
		matParent = bones[parent]->Transform;

	bone->Transform = bone->Transform * matParent;  // 자식 부모 곱

	bones.push_back(bone);

	ReadMeshData(node, index);

	for (UINT i = 0; i < node->mNumChildren; i++)  // mNumChildren : 자식 개수
		ReadBoneData(node->mChildren[i], bones.size(), index);  // 부모가 자식들에 대한 재귀
}

void Converter::ReadMeshData(aiNode * node, int bone)
{
	if (node->mNumMeshes < 1) return;  // 메쉬없는 본

	asMesh* mesh = new asMesh();
	// 메쉬 이름
	mesh->Name = node->mName.C_Str();
	// 메쉬 본 인덱스
	mesh->BoneIndex = bone;

	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		UINT index = node->mMeshes[i];
		aiMesh* srcMesh = scene->mMeshes[index];

		// 메쉬 머테리얼 이름
		aiMaterial* material = scene->mMaterials[srcMesh->mMaterialIndex];
		mesh->MaterialName = material->GetName().C_Str();

		// 메쉬 정점
		UINT startVertex = mesh->Vertices.size();  // 맨처음 0개
		for (UINT v = 0; v < srcMesh->mNumVertices; v++)
		{
			// 정점 위치
			Model::ModelVertex vertex;
			memcpy(&vertex.Position, &srcMesh->mVertices[v], sizeof(Vector3));

			// UV좌표
			if (srcMesh->HasTextureCoords(0))
				memcpy(&vertex.Uv, &srcMesh->mTextureCoords[0][v], sizeof(Vector2));  // mTextureCoords[0] uvw인 경우 1로 올려야함

			// 노멀벡터
			if (srcMesh->HasNormals())
				memcpy(&vertex.Normal, &srcMesh->mNormals[v], sizeof(Vector3));

			// 탄젠트
			if (srcMesh->HasTangentsAndBitangents())
				memcpy(&vertex.Tangent, &srcMesh->mTangents[v], sizeof(Vector3));

			mesh->Vertices.push_back(vertex);
		}

		// 메쉬 정점 인덱스
		for (UINT f = 0; f < srcMesh->mNumFaces; f++)
		{
			// srcMesh에는 012, 012로만 나옴 (변환 필요)
			aiFace& face = srcMesh->mFaces[f];

			for (UINT k = 0; k < face.mNumIndices; k++)
			{
				UINT index = face.mIndices[k] + startVertex;// 012, 345
				mesh->Indices.push_back(index);
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

		if (aiMesh->HasBones() == false) continue;  // 본없는 메쉬(애니메이션없음)

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

			// 정점 가중치 초기화
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

asExAnimation* Converter::ReadClipData(aiAnimation * srcClip)
{
	asExAnimation* dstClip = new asExAnimation();
	dstClip->ClipName = srcClip->mName.C_Str();
	dstClip->FrameRate = (float)srcClip->mTicksPerSecond;
	dstClip->Duration = (float)srcClip->mDuration;

	for (UINT i = 0; i < srcClip->mNumChannels; i++)
	{
		aiNodeAnim* srcAnimNode = srcClip->mChannels[i];

		asExAnimBone* dstAnimBone = new asExAnimBone();
		dstAnimBone->BoneName = srcAnimNode->mNodeName.C_Str();

		for (UINT i = 0; i < srcAnimNode->mNumPositionKeys; i++)
		{
			aiVectorKey srcKey = srcAnimNode->mPositionKeys[i];
			asExKeyVector dstKey;

			float currTime = (float)srcKey.mTime;
			float nextTime = currTime;
			if (srcAnimNode->mNumPositionKeys != i + 1) 
				nextTime = (float)srcAnimNode->mPositionKeys[i + 1].mTime;
			dstKey.Time = nextTime - currTime;

			memcpy_s(&dstKey.Value, sizeof(Vector3), &srcKey.mValue, sizeof(aiVector3D));
			dstAnimBone->KeyPositions.push_back(dstKey);
		}

		for (UINT i = 0; i < srcAnimNode->mNumRotationKeys; i++)
		{
			aiQuatKey srcKey = srcAnimNode->mRotationKeys[i];
			asExKeyQuat dstKey;

			float currTime = (float)srcKey.mTime;
			float nextTime = currTime;
			if (srcAnimNode->mNumRotationKeys != i + 1)
				nextTime = (float)srcAnimNode->mRotationKeys[i + 1].mTime;
			dstKey.Time = nextTime - currTime;

			dstKey.Value.x = srcKey.mValue.x;
			dstKey.Value.y = srcKey.mValue.y;
			dstKey.Value.z = srcKey.mValue.z;
			dstKey.Value.w = srcKey.mValue.w;
			dstAnimBone->KeyRotations.push_back(dstKey);
		}

		for (UINT i = 0; i < srcAnimNode->mNumScalingKeys; i++)
		{
			aiVectorKey srcKey = srcAnimNode->mScalingKeys[i];
			asExKeyVector dstKey;

			float currTime = (float)srcKey.mTime;
			float nextTime = currTime;
			if (srcAnimNode->mNumScalingKeys != i + 1)
				nextTime = (float)srcAnimNode->mScalingKeys[i + 1].mTime;
			dstKey.Time = nextTime - currTime;

			memcpy_s(&dstKey.Value, sizeof(Vector3), &srcKey.mValue, sizeof(aiVector3D));
			dstAnimBone->KeyScales.push_back(dstKey);
		}

		dstClip->Bones.push_back(dstAnimBone);
	}  // for i

	return dstClip;
}

void Converter::WriteClipData(asExAnimation * clip, wstring savePath, bool bOverWrite)
{
	if (bOverWrite == false)
	{
		if (Path::ExistFile(savePath))
			return;
	}

	Path::CreateFolders(Path::GetDirectoryName(savePath));
	BinaryWriter w;
	w.Open(savePath);
	
	w.String(clip->ClipName);
	w.Float(clip->Duration);
	w.Float(clip->FrameRate);

	w.UInt(clip->Bones.size());
	for (asExAnimBone* animBone : clip->Bones)
	{
		w.String(animBone->BoneName);

		UINT size;
		
		size = animBone->KeyPositions.size();
		w.UInt(size);
		w.BYTE(animBone->KeyPositions.data(), sizeof(asExKeyVector) * size);

		size = animBone->KeyRotations.size();
		w.UInt(size);
		w.BYTE(animBone->KeyRotations.data(), sizeof(asExKeyQuat) * size);

		size = animBone->KeyScales.size();
		w.UInt(size);
		w.BYTE(animBone->KeyScales.data(), sizeof(asExKeyVector) * size);

		SafeDelete(animBone);
	}

	SafeDelete(clip);
	w.Close();
}

#pragma endregion

// 베지어 곡선
// SmoothStep
// lerp * 2