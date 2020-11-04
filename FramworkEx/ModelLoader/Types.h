#pragma once

#include "stdafx.h"

struct asMaterial
{
	string Name;  // 한글 안 됨

	// 조명을 받았을 때 흡수할 색깔
	Color Ambient;
	Color Diffuse;
	Color Specular;  // .a : 샤이니스 강도

	string DiffuseFile;  // albedo
	string SpecularFile;  // phong
	string NormalFile;  // tangentMap
};

struct asBone
{
	int Index;
	string Name;

	// 인버스 키네스틱
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
	// 영향 받을 가중치 번호
	Vector4 Indices = Vector4(0, 0, 0, 0);

	// 영향 받을 가중치 크기
	Vector4 Weights = Vector4(0, 0, 0, 0);

	void Set(UINT index, UINT boneIndex, float weight)
	{
		float i = (float)boneIndex;
		float w = weight;

		// 가중치합이 1이 넘으면 안 된다.
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
	// 인덱스, 가중치
	typedef pair<int, float> Pair;
	vector<Pair> BoneWeights;

public:
	// 가중치 최고 순서대로 추가
	void AddWeights(UINT boneIndex, float boneWeights)
	{
		// 음수는 잘못된 정보, 0은 필요없음
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

	// 최고 가중치 4개까지만 저장하는 함수
	void GetBlendWeights(asBlendWeight& blendWeight)
	{
		for (UINT i = 0; i < BoneWeights.size(); i++)
		{
			if (i >= 4) return;

			blendWeight.Set(i, BoneWeights[i].first, BoneWeights[i].second);
		}
	}

	// 가중치 합을 1이하의 값으로 정규화
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
				// 4개 이상은 삭제
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

// 채널 분리
struct asClipNode
{
	vector<asKeyframeData> Keyframe;
	aiString Name;
};

// 에니메이션
struct asClip
{
	string Name;

	UINT FrameCount;  // 최대 250프레임 제한 (해제)
	float FrameRate;  // 30프레임레이트으로 변환
	float Duration;  // 재생시간

	vector<asKeyframe*> Keyframes;
};


// 조사
// 1. 쿼터니온
// w는 타축회전
// 2. 그람슈미츠(방향 1개로 축만들기)
// 정규직교화
// 3. 
// D3DXMatrixDecompose
// LU Decompose