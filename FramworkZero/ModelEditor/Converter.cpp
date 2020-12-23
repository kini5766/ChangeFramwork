#include "stdafx.h"
#include "Converter.h"

#include "Types.h"
#include "Utilities/BinaryFile.h"
#include "Utilities/Xml.h"

Converter::Converter()
{
	importer = new Assimp::Importer();
}

Converter::~Converter()
{
	for (auto d : bones)
		SafeDelete(d);
	for (asMesh* ds : meshes)
	{
		for (auto d : ds->MeshParts)
			SafeDelete(d);
		SafeDelete(ds);
	}

	for (auto d : materials)
		SafeDelete(d);

	SafeDelete(importer);
}

void Converter::ReadFile(wstring _file)
{
	file = URI::Assets + _file;

	scene = importer->ReadFile
	(
		String::ToString(file),
		aiProcess_ConvertToLeftHanded  // LH, CW
		| aiProcess_Triangulate  // To triangleList
		| aiProcess_GenUVCoords  // uv 다시 계산
		| aiProcess_GenNormals  // generate normal vector
		| aiProcess_CalcTangentSpace  // tanget space
	);

	if (scene == nullptr)
	{
		string str = importer->GetErrorString();

		MessageBox(WinDesc::GetHandle(), String::ToWString(str).c_str(), L"Assimp Error", MB_OK);
		exit(-1);
	}
}


#pragma region BoneMesh

void Converter::ExportMesh(wstring savePath)
{
	// 최상위는 인덱스와 부모가 없음 (이름 : RootNode)
	ReadBoneData(scene->mRootNode, -1, -1);
	ReadSkinData();
	WriteMeshData(URI::Models + savePath + L".mesh");
}

void Converter::ReadBoneData(aiNode * node, int index, int parent)
{
	asBone* bone = new asBone();

	bone->Index = index;
	bone->Parent = parent;
	bone->Name = node->mName.C_Str();

	// node->mTransformation (2차원포인터)
	Matrix transform(node->mTransformation[0]);
	D3DXMatrixTranspose(&bone->Transform, &transform);

	Matrix matParent;
	if (parent < 0)
		D3DXMatrixIdentity(&matParent);
	else
		matParent = bones[parent]->Transform;

	bone->Transform = bone->Transform * matParent;  // root 기준 상대좌표

	ReadMeshData(node, index);

	bones.push_back(bone);
	for (UINT i = 0; i < node->mNumChildren; i++)
		ReadBoneData(node->mChildren[i], bones.size(), index);  // 전위순환
}

void Converter::ReadMeshData(aiNode * node, int index)
{
	// 거밍본, 가상본, 소캣(조인트) 제외
	if (node->mNumMeshes < 1) return;

	// Model은 1개의 Mesh 덩어리를 만들기 위해 여러개의 MeshPart로 분할 저장 되어 있음
	asMesh* mesh = new asMesh();
	mesh->BoneIndex = index;  // Mesh 덩어리가 붙어있어야 할 본 번호

	// Mesh part
	for (UINT i = 0; i < node->mNumMeshes; i++)
	{
		// 메쉬 참조 번호
		UINT index = node->mMeshes[i];

		// 메쉬파트 얻기
		aiMesh* srcMesh = scene->mMeshes[index];

		UINT startVertex = mesh->Vertices.size();
		UINT startIndex = mesh->Indeices.size();

		// aiMesh가 가진 정점의 개수 만큼 돌면서
		for (UINT v = 0; v < srcMesh->mNumVertices; v++)
		{
			// 지금은 pos, uv, normal만 저장  // 한 개씩 (매쉬데이터 수정예정)
			VertexModel vertex;
			memcpy(&vertex.Position, &srcMesh->mVertices[v], sizeof(Vector3));

			// 텍스쳐 레이어 UV따라 다르게 줄 수 있다.(게임에선 필요 없음)
			if (srcMesh->HasTextureCoords(0)) // UV
				memcpy(&vertex.Uv, &srcMesh->mTextureCoords[0][v], sizeof(Vector2));

			if (srcMesh->HasNormals())
				memcpy(&vertex.Normal, &srcMesh->mNormals[v], sizeof(Vector3));

			mesh->Vertices.push_back(vertex);
		}

		for (UINT f = 0; f < srcMesh->mNumFaces; f++)
		{
			aiFace& face = srcMesh->mFaces[f];

			for (UINT k = 0; k < face.mNumIndices; k++)
				mesh->Indeices.push_back(face.mIndices[k] + startVertex);
			// face마다 0부터 출발하기 때문에 보정하기 위해 +startVertex을 사용
		}

		aiMaterial* material = scene->mMaterials[srcMesh->mMaterialIndex];
		
		asMeshPart* meshPart = new asMeshPart();
		meshPart->MaterialName = material->GetName().C_Str();
		meshPart->StartVertex = startVertex;
		meshPart->StartIndex = startIndex;
		meshPart->VertexCount = srcMesh->mNumVertices;
		meshPart->IndexCount = srcMesh->mNumFaces * srcMesh->mFaces->mNumIndices;

		mesh->MeshParts.push_back(meshPart);
	} // end Mesh part

	meshes.push_back(mesh);
}

void Converter::ReadSkinData()
{
	// (1) aiMesh를 얻기 (aiMesh -> anim mesh)
	// (2) aiMesh에 붙은 Anim Bone들을 얻어온다. -> 가중치
	// (3) 렌더링에 사용되는 Mesh는 aiMesh고, 이 asMesh는 asBone에 붙어 있다.
	// -> asBone을 인덱스 번호로 찾아온 후
	// (4) 1번 aiMesh에서 끄집어낸 가중치 정보를 asMesh에 써준다.

	//  ai 전체 메쉬 다 돌면서 스킨 정보 있는지 체크
	// 있으면 asMesh 멤버 변수 Blend들에 저장
	for (UINT i = 0; i < scene->mNumMeshes; i++)
	{
		aiMesh* aiMesh = scene->mMeshes[i];

		// 메쉬는 있으나 animBone이 없는 경우 (가중치 없음)
		if (aiMesh->HasBones() == false) continue;

		asMesh* mesh = meshes[i];

		vector<asBoneWeights> boneWeights;
		boneWeights.assign(mesh->Vertices.size(), asBoneWeights());

		// anim bone fbx이므로 메쉬 하나에 여러 본이 존재
		for (UINT b = 0; b < aiMesh->mNumBones; b++)
		{
			aiBone* aiMeshBone = aiMesh->mBones[b];

			UINT boneIndex = 0;

			for (asBone* bone : bones)
			{
				// asBone과 ai Anim Bone 이름이 일치하는 본 찾음
				if (bone->Name == (string)aiMeshBone->mName.C_Str())
				{
					boneIndex = bone->Index;
					break;
				}
			}

			for (UINT w = 0; w < aiMeshBone->mNumWeights; w++)
			{
				UINT index = aiMeshBone->mWeights[w].mVertexId;  // 정점번호
				float weight = aiMeshBone->mWeights[w].mWeight;  // 가중치

				// 가중치 순서대로 내림차순으로 저장
				boneWeights[index].AddWeights(boneIndex, weight);
			}

		}// end aiMesh->mNumBones

		for (UINT w = 0; w < boneWeights.size(); w++)
		{
			boneWeights[w].Normalize();
			asBlendWeight blendweight;
			boneWeights[w].GetBlendWeights(blendweight);

			mesh->Vertices[w].BlendIndices = blendweight.Indices;
			mesh->Vertices[w].BlendWeight = blendweight.Weights;
		}

	}
}

void Converter::WriteMeshData(wstring savePath)
{
	// CreateFolders : 하위까지 만들어 줌
	// GetDirectoryName : 파일이름을 날려버림
	Path::CreateFolders(Path::GetDirectoryName(savePath));

	{
		BinaryWriter w(savePath);  // open close 안에 있음

		// save bone
		w.UInt(bones.size());
		for (asBone* bone : bones)
		{
			w.Int(bone->Index);
			w.String(bone->Name);
			w.Int(bone->Parent);
			w.Matrix(bone->Transform);
		}

		// save mesh
		w.UInt(meshes.size());
		for (asMesh* meshData : meshes)
		{
			w.Int(meshData->BoneIndex);

			UINT size;

			size = meshData->Vertices.size();
			w.UInt(size);
			w.Byte(meshData->Vertices.data(), sizeof(VertexModel) * size);

			size = meshData->Indeices.size();
			w.UInt(size);
			w.Byte(meshData->Indeices.data(), sizeof(UINT) * size);

			w.UInt(meshData->MeshParts.size());
			for (asMeshPart* part : meshData->MeshParts)
			{
				// 이름이랑 매칭
				w.String(part->MaterialName);

				w.UInt(part->StartVertex);
				w.UInt(part->VertexCount);

				w.UInt(part->StartIndex);
				w.UInt(part->IndexCount);
			}
		}
	}
}

#pragma endregion


#pragma region Material

void Converter::ExportMaterial(wstring savePath, bool bOverWrite)
{
	savePath = URI::Textures + savePath + L".material";

	if (bOverWrite == false)
	{
		if (Path::ExistFile(savePath))
			return;
	}

	ReadMaterialData();
	WriteMaterialData(savePath);
}

void Converter::ReadMaterialData()
{
	for (UINT i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* srcMaterial = scene->mMaterials[i];
		if (FoundMaterialData(srcMaterial) == false)
			continue;

		asMaterial* dstMaterial = new asMaterial();
		dstMaterial->Name = srcMaterial->GetName().C_Str();
		
		aiColor3D color;
		srcMaterial->Get(AI_MATKEY_COLOR_AMBIENT, color);
		dstMaterial->Ambient = Color(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, color);
		dstMaterial->Diffuse = Color(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_SPECULAR, color);
		dstMaterial->Specular = Color(color.r, color.g, color.b, 1.0f);

		srcMaterial->Get(AI_MATKEY_COLOR_EMISSIVE, color);
		dstMaterial->Emissive = Color(color.r, color.g, color.b, 1.0f);


		aiString file;

		srcMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &file);
		dstMaterial->DiffuseFile = file.C_Str();

		srcMaterial->GetTexture(aiTextureType_NORMALS, 0, &file);
		dstMaterial->NormalFile = file.C_Str();

		srcMaterial->GetTexture(aiTextureType_SPECULAR, 0, &file);
		dstMaterial->SpecularFile = file.C_Str();

		materials.push_back(dstMaterial);
	}
}

bool Converter::FoundMaterialData(aiMaterial * _material)
{
	string materialName = _material->GetName().C_Str();

	for (asMesh* mesh : meshes)
		for (asMeshPart* part : mesh->MeshParts)
			if (part->MaterialName == materialName)
				return true;

	return false;
}

void Converter::WriteMaterialData(wstring savePath)
{
	wstring folder = Path::GetDirectoryName(savePath);
	wstring file = Path::GetFileName(savePath);

	Path::CreateFolders(folder);

	// document
	{
		Xml::XMLDocument document;

		Xml::XMLDeclaration* decl = document.NewDeclaration();
		document.LinkEndChild(decl);

		// <Materials>
		Xml::XMLElement* root = document.NewElement("Materials");
		root->SetAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
		root->SetAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema");

		for (asMaterial* material : materials)
		{
			// <Material>
			Xml::XMLElement* node = document.NewElement("Material");
			Xml::XMLElement* e;

			// <Name>
			e = document.NewElement("Name");
			e->SetText(material->Name.c_str());
			node->LinkEndChild(e);
			// </Name>

			// <DiffuseFile>
			e = document.NewElement("DiffuseFile");
			e->SetText(WriteTexture(folder, material->DiffuseFile).c_str());
			node->LinkEndChild(e);
			// </DiffuseFile>

			// <SpecularFile>
			e = document.NewElement("SpecularFile");
			e->SetText(WriteTexture(folder, material->SpecularFile).c_str());
			node->LinkEndChild(e);
			// </SpecularFile>

			// <NormalFile>
			e = document.NewElement("NormalFile");
			e->SetText(WriteTexture(folder, material->NormalFile).c_str());
			node->LinkEndChild(e);
			// </NormalFile>

			// <Ambient>
			e = document.NewElement("Ambient");
			e->SetAttribute("R", material->Ambient.r);
			e->SetAttribute("G", material->Ambient.g);
			e->SetAttribute("B", material->Ambient.b);
			e->SetAttribute("A", material->Ambient.a);
			node->LinkEndChild(e);
			// </Ambient>

			// <Diffuse>
			e = document.NewElement("Diffuse");
			e->SetAttribute("R", material->Diffuse.r);
			e->SetAttribute("G", material->Diffuse.g);
			e->SetAttribute("B", material->Diffuse.b);
			e->SetAttribute("A", material->Diffuse.a);
			node->LinkEndChild(e);
			// </Diffuse>

			// <Specular>
			e = document.NewElement("Specular");
			e->SetAttribute("R", material->Specular.r);
			e->SetAttribute("G", material->Specular.g);
			e->SetAttribute("B", material->Specular.b);
			e->SetAttribute("A", material->Specular.a);
			node->LinkEndChild(e);
			// </Specular>

			// <Emissive>
			e = document.NewElement("Emissive");
			e->SetAttribute("R", material->Emissive.r);
			e->SetAttribute("G", material->Emissive.g);
			e->SetAttribute("B", material->Emissive.b);
			e->SetAttribute("A", material->Emissive.a);
			node->LinkEndChild(e);
			// </Emissive>

			root->LinkEndChild(node);
			// </Material>
		}
		document.LinkEndChild(root);
		// </Materials>
		
		document.SaveFile(String::ToString((folder + file)).c_str());
	}
}

string Converter::WriteTexture(wstring saveFolder, string _file)
{
	if (file.length() < 1) return "";

	string fileName = Path::GetFileName(_file);

	// 내장 택스쳐
	const aiTexture* texture = scene->GetEmbeddedTexture(_file.c_str());

	wstring path = L"";
	if (texture != nullptr)
	{
		// 내장 택스쳐가 있는 경우

		// GetFileNameWithoutExtension : 파일이름만
		path = saveFolder + String::ToWString(Path::GetFileNameWithoutExtension(_file)) + L".png";
		
		// 1차원 텍스쳐 저장
		{
			BinaryWriter w(path);
			w.Byte(texture->pcData, texture->mWidth);
			// 2차원 데이터 텍스터는 없었음
		}
	}
	else
	{
		// 내장 택스쳐가 없는 경우
		string directory = String::ToString(Path::GetDirectoryName(this->file));
		string origin = directory + _file;
		String::Replace(&origin, "\\", "/");

		if (Path::ExistFile(origin) == false) return ""; // 텍스쳐 없음

		path = saveFolder + String::ToWString(fileName);
		CopyFileA(origin.c_str(), String::ToString(path).c_str(), FALSE); // 파일 복사

		String::Replace(&path, URI::Textures, L"");
	}

	return String::ToString(Path::GetFileName(path));
}
#pragma endregion


#pragma region Clip

void Converter::GetClipList(vector<wstring>* list)
{
	for (UINT i = 0; i < scene->mNumAnimations; i++)
	{
		aiAnimation* srcAnim = scene->mAnimations[i];
		(*list).push_back(String::ToWString(srcAnim->mName.C_Str()));
	}
}

void Converter::ExportAnimClip(UINT index, wstring savePath)
{
	savePath = URI::Models + savePath + L".clip";

	asExClip* clip = ReadClipData(scene->mAnimations[index]);
	WriteClipData(clip, savePath);
}

asExClip * Converter::ReadClipData(aiAnimation * srcAnimation)
{
	asExClip* dstClip = new asExClip();
	dstClip->Name = srcAnimation->mName.C_Str();
	dstClip->FrameRate = (float)srcAnimation->mTicksPerSecond;
	dstClip->Duration = (float)srcAnimation->mDuration;

	vector<asExClipBone*> bones;
	for (UINT i = 0; i < srcAnimation->mNumChannels; i++)
	{
		// anim Bone 저장
		aiNodeAnim* srcAniNode = srcAnimation->mChannels[i];

		asExClipBone* dstAnimBone = new asExClipBone();
		dstAnimBone->BoneName = srcAniNode->mNodeName.C_Str();
		UINT maxFrame = 0;
		maxFrame = max(maxFrame, srcAniNode->mNumPositionKeys);
		maxFrame = max(maxFrame, srcAniNode->mNumRotationKeys);
		maxFrame = max(maxFrame, srcAniNode->mNumScalingKeys);
		dstAnimBone->MaxFrame = maxFrame;

		// Position
		for (UINT i = 0; i < srcAniNode->mNumPositionKeys; i++)
		{
			aiVectorKey srcKey = srcAniNode->mPositionKeys[i];
			asExKeyVector dstKey;

			float currTime = (float)srcKey.mTime;
			dstKey.Time = currTime;

			float nextTime = currTime;
			if (srcAniNode->mNumPositionKeys != i + 1)
				nextTime = (float)srcAniNode->mPositionKeys[i + 1].mTime;

			memcpy_s(&dstKey.Value, sizeof(Vector3), &srcKey.mValue, sizeof(aiVector3D));
			dstAnimBone->KeyPositions.push_back(dstKey);
		}

		// Rotation
		for (UINT i = 0; i < srcAniNode->mNumRotationKeys; i++)
		{
			aiQuatKey srcKey = srcAniNode->mRotationKeys[i];
			asExKeyQuat dstKey;

			float currTime = (float)srcKey.mTime;
			dstKey.Time = currTime;

			float nextTime = currTime;
			if (srcAniNode->mNumRotationKeys != i + 1)
				nextTime = (float)srcAniNode->mRotationKeys[i + 1].mTime;

			dstKey.Value.x = srcKey.mValue.x;
			dstKey.Value.y = srcKey.mValue.y;
			dstKey.Value.z = srcKey.mValue.z;
			dstKey.Value.w = srcKey.mValue.w;
			dstAnimBone->KeyRotations.push_back(dstKey);
		}

		// Scale
		for (UINT i = 0; i < srcAniNode->mNumScalingKeys; i++)
		{
			aiVectorKey srcKey = srcAniNode->mScalingKeys[i];
			asExKeyVector dstKey;

			float currTime = (float)srcKey.mTime;
			dstKey.Time = currTime;

			float nextTime = currTime;
			if (srcAniNode->mNumScalingKeys != i + 1)
				nextTime = (float)srcAniNode->mScalingKeys[i + 1].mTime;

			memcpy_s(&dstKey.Value, sizeof(Vector3), &srcKey.mValue, sizeof(aiVector3D));
			dstAnimBone->KeyScales.push_back(dstKey);
		}

		bones.push_back(dstAnimBone);

	} // End srcAnimation->mNumChannels

	ReadKeyframeData(dstClip, scene->mRootNode, bones);

	return dstClip;
}

void Converter::ReadKeyframeData(asExClip * clip, aiNode * node, vector<asExClipBone*>& aniBones)
{
	asExClipBone* bone = nullptr;

	for (UINT n = 0; n < aniBones.size(); n++)
	{
		// 이름으로 검색
		if (aniBones[n]->BoneName == node->mName)
		{
			bone = aniBones[n];
			break;
		}
	}

	// MeshBone은 있지만 AnimBone이 없는 경우
	if (bone == nullptr)
	{
		bone = new asExClipBone();
		bone->MaxFrame = 0;
		bone->BoneName = node->mName;
	}

	clip->Bones.push_back(bone);

	// Pre order 재귀
	for (UINT i = 0; i < node->mNumChildren; i++)
		ReadKeyframeData(clip, node->mChildren[i], aniBones);
}

void Converter::WriteClipData(asExClip * clip, wstring savePath)
{
	Path::CreateFolders(Path::GetDirectoryName(savePath));

	BinaryWriter w(savePath);

	w.String(clip->Name);
	w.Float(clip->FrameRate);
	w.Float(clip->Duration);

	w.UInt(clip->Bones.size());
	for (asExClipBone* bone : clip->Bones)
	{
		UINT size;

		size = bone->KeyPositions.size();
		w.UInt(size);
		w.Byte(bone->KeyPositions.data(), sizeof(asExKeyVector) * size);

		size = bone->KeyRotations.size();
		w.UInt(size);
		w.Byte(bone->KeyRotations.data(), sizeof(asExKeyQuat) * size);

		size = bone->KeyScales.size();
		w.UInt(size);
		w.Byte(bone->KeyScales.data(), sizeof(asExKeyVector) * size);

		w.UInt(bone->MaxFrame);

		SafeDelete(bone);
	}

	SafeDelete(clip);
}


#pragma endregion
