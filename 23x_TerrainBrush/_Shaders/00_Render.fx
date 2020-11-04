// 이제 필요없음
//#define MAX_MODEL_TRANSFORMS 250
//#define MAX_MODEL_KEYFRAMES 500

Texture2DArray TransformsMap;

// 레스터라이저 이후 VertexMesh값들이 선형보간이 되어 VertexOutput로 만들어짐
struct VertexMesh
{
	float4 Position : POSITION0;
	float2 Uv : UV0;
	float3 Normal : NORMAL0;
};

MeshOutput VS_Mesh(VertexMesh input)
{
	MeshOutput output;
	VS_GENERATE
	return output;
}

// cbuffer /////////////////////////////////////////////////////////////////////////////

cbuffer CB_Bone
{
	Matrix BoneTransform;
    uint BoneIndex;
};

struct VertexModel
{
	float4 Position : POSITION;
	float2 Uv : UV;
	float3 Normal : NORMAL;

	float3 Tangent : TANGENT0;
	float4 BlendIndices : BLENDINDICES0;
	float4 BlendWeights : BLENDWEIGHTS0;
};

// 프로그래머가 본트렌스폼 바꿀일이 없다는 것을 간주

// inout : 콜 by 레퍼런스랑 같음
void SetModelWorld(inout matrix world)
{
	// 트렌스폼world * 본월드
	world = mul(BoneTransform, world);
}

MeshOutput VS_Model(VertexModel input)
{
	MeshOutput output;
	SetModelWorld(World);
	VS_GENERATE
	return output;
}

// Animation /////////////////////////////////////////////////////////////////////////////

struct AnimationFrame
{
	int Clip;

	uint CurrFrame;
	uint NextFrame;

	float Time;
	float RunningTime;

	float Speed;
	// float2 Padding;
};

struct TweenFrame
{
    float TakeTime;
    float TweenTime;
    float RunningTime;
    float Padding;
    
    AnimationFrame Curr;
    AnimationFrame Next;
};

cbuffer CB_AnimationFrame
{
    TweenFrame TweenFrames;
};

void SetAnimationWorld(inout matrix world, VertexModel input)
{
	matrix transform = 0;

	matrix curr = 0, currAnim = 0;
	matrix next = 0, nextAnim = 0;

	float indices[4] =
	{
		input.BlendIndices.x,
		input.BlendIndices.y,
		input.BlendIndices.z,
		input.BlendIndices.w
	};
	float weights[4] =
	{
		input.BlendWeights.x,
		input.BlendWeights.y,
		input.BlendWeights.z,
		input.BlendWeights.w
	};
    
    // any 값이 모두 0이면 false 하나라도 0이 아니면 true
    if (any(input.BlendIndices) == false)
    {
        // 추가된 부모 만큼만 움직임
        indices[0] = BoneIndex;
        weights[0] = 1.0f;
    }

	uint clip[2];
	uint currentFrame[2];
	uint nextFrame[2];
	float time[2];

    clip[0] = TweenFrames.Curr.Clip;
    currentFrame[0] = TweenFrames.Curr.CurrFrame;
    nextFrame[0] = TweenFrames.Curr.NextFrame;
    time[0] = TweenFrames.Curr.Time;
    
    clip[1] = TweenFrames.Next.Clip;
    currentFrame[1] = TweenFrames.Next.CurrFrame;
    nextFrame[1] = TweenFrames.Next.NextFrame;
    time[1] = TweenFrames.Next.Time;

	float4 c0, c1, c2, c3;
	float4 n0, n1, n2, n3;

	// 셰이더의 플롯 컨트롤 (반복문)
	// unroll은 컴파일러단에 코드를 늘림
	[unroll(4)]
	for (int i = 0; i < 4; i++)  // int는 전역변수
	{
        // 현재 프레임
        
		// 샘플링 할 떄 mipmap이 발생하지 않는 함수
		// int4.w == 밉맵 레벨 (0은 원본)
		// 1D : int2 = x, w, 2D : int3 = x, y, w
        c0 = TransformsMap.Load(int4(indices[i] * 4 + 0, currentFrame[0], clip[0], 0));
        c1 = TransformsMap.Load(int4(indices[i] * 4 + 1, currentFrame[0], clip[0], 0));
        c2 = TransformsMap.Load(int4(indices[i] * 4 + 2, currentFrame[0], clip[0], 0));
        c3 = TransformsMap.Load(int4(indices[i] * 4 + 3, currentFrame[0], clip[0], 0));

		curr = matrix(c0, c1, c2, c3);

		// 가중치 큰 순서대로 누적해서 적용
		// transform += mul(weights[i], curr);
        
        
        // 다음 프레임
        n0 = TransformsMap.Load(int4(indices[i] * 4 + 0, nextFrame[0], clip[0], 0));
        n1 = TransformsMap.Load(int4(indices[i] * 4 + 1, nextFrame[0], clip[0], 0));
        n2 = TransformsMap.Load(int4(indices[i] * 4 + 2, nextFrame[0], clip[0], 0));
        n3 = TransformsMap.Load(int4(indices[i] * 4 + 3, nextFrame[0], clip[0], 0));
        next = matrix(n0, n1, n2, n3);
        
        // 선형 보간
        currAnim = lerp(curr, next, time[0]);
        
        //-> 동작 선형 보간
        [flatten]
        if (clip[1] >= 0)
        {
            // 다음 클립이 있다면
            
            c0 = TransformsMap.Load(int4(indices[i] * 4 + 0, currentFrame[1], clip[1], 0));
            c1 = TransformsMap.Load(int4(indices[i] * 4 + 1, currentFrame[1], clip[1], 0));
            c2 = TransformsMap.Load(int4(indices[i] * 4 + 2, currentFrame[1], clip[1], 0));
            c3 = TransformsMap.Load(int4(indices[i] * 4 + 3, currentFrame[1], clip[1], 0));
            curr = matrix(c0, c1, c2, c3);

            n0 = TransformsMap.Load(int4(indices[i] * 4 + 0, nextFrame[1], clip[1], 0));
            n1 = TransformsMap.Load(int4(indices[i] * 4 + 1, nextFrame[1], clip[1], 0));
            n2 = TransformsMap.Load(int4(indices[i] * 4 + 2, nextFrame[1], clip[1], 0));
            n3 = TransformsMap.Load(int4(indices[i] * 4 + 3, nextFrame[1], clip[1], 0));
            next = matrix(n0, n1, n2, n3);
            
            nextAnim = lerp(curr, next, time[1]);
            currAnim = lerp(currAnim, nextAnim, TweenFrames.TweenTime);
        }
        //<-/
        
        transform += mul(weights[i], currAnim);
    }

	world = mul(transform, world);

}

MeshOutput VS_Animation(VertexModel input)
{
	MeshOutput output;

	SetAnimationWorld(World, input);

	VS_GENERATE

	return output;
}