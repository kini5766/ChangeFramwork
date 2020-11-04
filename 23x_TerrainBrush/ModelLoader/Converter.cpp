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

asClip * Converter::ReadClipData(aiAnimation * animation)
{
	asClip* clip = new asClip();
	clip->Name = animation->mName.C_Str();
	clip->FrameRate = (float)animation->mTicksPerSecond;
	clip->FrameCount = (UINT)animation->mDuration + 1;

	// 채널분리
	vector<asClipNode> aniNodeInfos;
	for (UINT i = 0; i < animation->mNumChannels; i++)
	{
		aiNodeAnim* aiNode = animation->mChannels[i];

		asClipNode aniNodeInfo;
		aniNodeInfo.Name = aiNode->mNodeName;  // 본 하나의 정보

		// mNumPositionKeys mNumRotationKeys mNumScalingKeys
		// 각각 키들이 분리된 것 들 중 마지막 프레임 얻기
		UINT keyCount = max(aiNode->mNumPositionKeys, aiNode->mNumRotationKeys);
		keyCount = max(keyCount, aiNode->mNumScalingKeys);

		// 분리된 키들을 메트릭스에 합치기
		asKeyframeData frameData;
		for (UINT k = 0; k < keyCount; k++)
		{
			bool bFound = false;
			UINT t = aniNodeInfo.Keyframe.size();

			// 포지션키
			// aiNode->mPositionKeys[k].mTime == t -> 부동 소수점 오차 때문에 절댓값에 D3DX_16F_EPSILON 사용
			if (fabsf((float)aiNode->mPositionKeys[k].mTime - (float)t) <= D3DX_16F_EPSILON)
			{
				aiVectorKey key = aiNode->mPositionKeys[k];
				memcpy_s(&frameData.Translation, sizeof(Vector3), &key.mValue, sizeof(aiVector3D));  // 크기가 다를 수 있어서 _s 사용
				frameData.Time = (float)aiNode->mPositionKeys[k].mTime;

				bFound = true;
			}

			// 로테이션키
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

			// 스케일키
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

		// 비어있는 마지막 키 추가
		if (aniNodeInfo.Keyframe.size() < clip->FrameCount)
		{
			UINT count = clip->FrameCount - aniNodeInfo.Keyframe.size();

			asKeyframeData keyFrame = aniNodeInfo.Keyframe.back();

			for (UINT n = 0; n < count; n++)
				aniNodeInfo.Keyframe.push_back(keyFrame);
		}

		// 보통은 clip->Duration가 큼
		clip->Duration = max(clip->Duration, aniNodeInfo.Keyframe.back().Time);

		aniNodeInfos.push_back(aniNodeInfo);

	}  // for i

	// 재귀 함수
	ReadKeyframeData(clip, scene->mRootNode, aniNodeInfos);

	return clip;
}

// vector<asKeyframe*> asClip::Keyframes 저장하기 위한 함수
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
			// 같은 이름이 없을 때
			// (외부에서 붙인 모델이란 뜻)
			Matrix transform(node->mTransformation[0]);
			// 전치
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

// 베지어 곡선
// SmoothStep
// lerp * 2