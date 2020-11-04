#pragma once

#include "stdafx.h"

struct asMaterial
{
	string Name;  // �ѱ� �� ��

	// ������ �޾��� �� ����� ����
	Color Ambient;
	Color Diffuse;
	Color Specular;  // .a : ���̴Ͻ� ����

	string DiffuseFile;  // albedo
	string SpecularFile;  // phong
	string NormalFile;  // tangentMap
};

struct asBone
{
	int Index;
	string Name;

	// �ι��� Ű�׽�ƽ
	int Parent;
	Matrix Transform;
};

struct asMesh
{
	string Name;
	int BoneIndex;

	aiMesh* mesh;

	string MaterialName;

	vector<Model::ModelVertex> Vertices;
	vector<UINT> Indices;
};

struct asBlendWeight
{
	// ���� ���� ����ġ ��ȣ
	Vector4 Indices = Vector4(0, 0, 0, 0);

	// ���� ���� ����ġ ũ��
	Vector4 Weights = Vector4(0, 0, 0, 0);

	void Set(UINT index, UINT boneIndex, float weight)
	{
		float i = (float)boneIndex;
		float w = weight;

		// ����ġ���� 1�� ������ �� �ȴ�.
		switch (index)
		{
		case 0: Indices.x = i; Weights.x = w; break;
		case 1: Indices.y = i; Weights.y = w; break;
		case 2: Indices.z = i; Weights.z = w; break;
		case 3: Indices.w = i; Weights.w = w; break;
		}
	}
};

struct asBoneWeights
{
private:
	// �ε���, ����ġ
	typedef pair<int, float> Pair;
	vector<Pair> BoneWeights;

public:
	// ����ġ �ְ� ������� �߰�
	void AddWeights(UINT boneIndex, float boneWeights)
	{
		// ������ �߸��� ����, 0�� �ʿ����
		if (boneWeights <= 0.0f) return;

		bool bAdd = false;
		vector<Pair>::iterator iter = BoneWeights.begin();

		while (iter != BoneWeights.end())
		{
			if (boneWeights > iter->second)
			{
				BoneWeights.insert(iter, Pair(boneIndex, boneWeights));
				bAdd = true;

				break;
			}

			iter++;
		}

		if (bAdd == false)
			BoneWeights.push_back(Pair(boneIndex, boneWeights));
	}

	// �ְ� ����ġ 4�������� �����ϴ� �Լ�
	void GetBlendWeights(asBlendWeight& blendWeight)
	{
		for (UINT i = 0; i < BoneWeights.size(); i++)
		{
			if (i >= 4) return;

			blendWeight.Set(i, BoneWeights[i].first, BoneWeights[i].second);
		}
	}

	// ����ġ ���� 1������ ������ ����ȭ
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
			else
			{
				// 4�� �̻��� ����
				iter = BoneWeights.erase(iter);
			}
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

struct asKeyframeData
{
	float Time;
	
	Vector3 Scale;
	Quaternion Rotation;
	Vector3 Translation;
};

struct asKeyframe
{
	string BoneName;
	vector<asKeyframeData> Transforms;
};

// ä�� �и�
struct asClipNode
{
	vector<asKeyframeData> Keyframe;
	aiString Name;
};

// ���ϸ��̼�
struct asClip
{
	string Name;

	UINT FrameCount;  // �ִ� 250������ ���� (����)
	float FrameRate;  // 30�����ӷ���Ʈ���� ��ȯ
	float Duration;  // ����ð�

	vector<asKeyframe*> Keyframes;
};


// ����
// 1. ���ʹϿ�
// w�� Ÿ��ȸ��
// 2. �׶�������(���� 1���� �ุ���)
// ��������ȭ
// 3. 
// D3DXMatrixDecompose
// LU Decompose