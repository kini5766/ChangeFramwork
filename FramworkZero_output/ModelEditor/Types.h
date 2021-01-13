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

// ����ġ ���� ū 4���� �Ҵ� (���� ���� �� ��)
struct asBlendWeight
{
	Vector4 Indices = Vector4(0, 0, 0, 0);  // ������ ���� Bone�ε���
	Vector4 Weights = Vector4(0, 0, 0, 0);  // ������ ���� ����ġ

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
	// first ����� Bone ��ȣ, second ����ġ
	typedef pair<int, float> Pair;
	vector<Pair> BoneWeights;

public:
	void AddWeights(UINT boneIndex, float boneWeight)
	{
		// ������ ������ �Ÿ��� ������ �н�
		if (boneWeight <= 0.0f) return;

		bool bAdd = false;

		// �������� ���� ����
		vector<Pair>::iterator iter = BoneWeights.begin();
		while (iter != BoneWeights.end())
		{
			if (boneWeight > iter->second)
			{
				// insert : ���� �ε��� �ڷ� �и�
				BoneWeights.insert(iter, Pair(boneIndex, boneWeight));
				bAdd = true;

				break;
			}
			iter++;
		}

		// ���� ���� ���� ����ġ
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
	UINT MaxFrame;  // �ִ׺� ���� ��� 0

	aiString BoneName;  // ���� �� ��> (�޽� : �ִ� = 1:1) ��Ī �ϱ� ���� ����
};

struct asExClip
{
	string Name;
	float FrameRate;
	float Duration;
	vector<asExClipBone*> Bones; // (�޽��� : ���׺�)=(1 : 1) �ε��� ��Ī
};

#pragma endregion
