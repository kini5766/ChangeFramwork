#pragma once

#include "stdafx.h"


#pragma region ExportMesh

// Bone
struct asBone
{
	int Index;
	string Name;

	int Parent;
	Matrix Transform;
};

// Mesh
struct asMeshPart
{
	string MaterialName;

	UINT StartVertex;
	UINT VertexCount;
	UINT StartIndex;
	UINT IndexCount;
};

struct asMesh
{
	int BoneIndex;

	vector<VertexModel> Vertices;
	vector<UINT> Indeices;

	vector<asMeshPart*> MeshParts;
};

#pragma endregion


#pragma region ExportMaterial

struct asMaterial
{
	string Name;

	Color Ambient;
	Color Diffuse;
	Color Specular;
	Color Emissive;

	string DiffuseFile;
	string SpecularFile;
	string NormalFile;
};

#pragma endregion


#pragma region BoneWeight

// 가중치 제일 큰 4개만 할당 (최종 저장 될 값)
struct asBlendWeight
{
	Vector4 Indices = Vector4(0, 0, 0, 0);  // 영향을 받을 Bone인덱스
	Vector4 Weights = Vector4(0, 0, 0, 0);  // 영향을 받을 가중치

	void Set(UINT index, UINT boneIndex, float weight)
	{
		float i = (float)boneIndex;
		float w = weight;

		switch (index)
		{
		case 0: Indices.x = i; Weights.x = w; break;
		case 1: Indices.y = i; Weights.y = w; break;
		case 2: Indices.z = i; Weights.z = w; break;
		case 3: Indices.w = i; Weights.w = w; break;
		}
	}
};

// 1Vertex -> nBoneWeight
struct asBoneWeights
{
private:
	// first 연결된 Bone 번호, second 가중치
	typedef pair<int, float> Pair;
	vector<Pair> BoneWeights;

public:
	void AddWeights(UINT boneIndex, float boneWeight)
	{
		// 정점이 땡겨질 거리가 없으면 패스
		if (boneWeight <= 0.0f) return;

		bool bAdd = false;

		// 내림차순 정렬 삽입
		vector<Pair>::iterator iter = BoneWeights.begin();
		while (iter != BoneWeights.end())
		{
			if (boneWeight > iter->second)
			{
				// insert : 기존 인덱스 뒤로 밀림
				BoneWeights.insert(iter, Pair(boneIndex, boneWeight));
				bAdd = true;

				break;
			}
			iter++;
		}

		// 현재 제일 낮은 가중치
		if (bAdd == false)
			BoneWeights.push_back(Pair(boneIndex, boneWeight));
	}

	void GetBlendWeights(asBlendWeight& blendWeight)
	{
		for (UINT i = 0; i < BoneWeights.size(); i++)
		{
			if (i >= 4) return;

			blendWeight.Set(i, BoneWeights[i].first, BoneWeights[i].second);
		}
	}

	void Normalize()
	{
		float totalWeight = 0.0f;

		int i = 0;
		vector<Pair>::iterator iter = BoneWeights.begin();

		while (iter != BoneWeights.end())
		{
			if (i < 4)
			{
				totalWeight += iter->second;
				i++; iter++;
			}
			else iter = BoneWeights.erase(iter);
		}

		float scale = 1.0f / totalWeight;
		iter = BoneWeights.begin();
		while (iter != BoneWeights.end())
		{
			iter->second *= scale;
			iter++;
		}
	}
};

#pragma endregion


#pragma region ExportClip

struct asExKeyVector
{
	Vector3 Value;
	float Time;
};

struct asExKeyQuat
{
	Quaternion Value;
	float Time;
};


struct asExClipBone
{
	vector<asExKeyVector> KeyPositions;
	vector<asExKeyQuat> KeyRotations;
	vector<asExKeyVector> KeyScales;
	UINT MaxFrame;  // 애닝본 없는 경우 0

	aiString BoneName;  // 저장 안 함> (메쉬 : 애닝 = 1:1) 매칭 하기 위한 변수
};

struct asExClip
{
	string Name;
	float FrameRate;
	float Duration;
	vector<asExClipBone*> Bones; // (메쉬본 : 에닝본)=(1 : 1) 인덱스 매칭
};

#pragma endregion
