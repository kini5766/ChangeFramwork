// ���� �ʿ����
//#define MAX_MODEL_TRANSFORMS 250
//#define MAX_MODEL_KEYFRAMES 500

Texture2DArray TransformsMap;

// �����Ͷ����� ���� VertexMesh������ ���������� �Ǿ� VertexOutput�� �������
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

// ���α׷��Ӱ� ��Ʈ������ �ٲ����� ���ٴ� ���� ����

// inout : �� by ���۷����� ����
void SetModelWorld(inout matrix world)
{
	// Ʈ������world * ������
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
    
    // any ���� ��� 0�̸� false �ϳ��� 0�� �ƴϸ� true
    if (any(input.BlendIndices) == false)
    {
        // �߰��� �θ� ��ŭ�� ������
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

	// ���̴��� �÷� ��Ʈ�� (�ݺ���)
	// unroll�� �����Ϸ��ܿ� �ڵ带 �ø�
	[unroll(4)]
	for (int i = 0; i < 4; i++)  // int�� ��������
	{
        // ���� ������
        
		// ���ø� �� �� mipmap�� �߻����� �ʴ� �Լ�
		// int4.w == �Ӹ� ���� (0�� ����)
		// 1D : int2 = x, w, 2D : int3 = x, y, w
        c0 = TransformsMap.Load(int4(indices[i] * 4 + 0, currentFrame[0], clip[0], 0));
        c1 = TransformsMap.Load(int4(indices[i] * 4 + 1, currentFrame[0], clip[0], 0));
        c2 = TransformsMap.Load(int4(indices[i] * 4 + 2, currentFrame[0], clip[0], 0));
        c3 = TransformsMap.Load(int4(indices[i] * 4 + 3, currentFrame[0], clip[0], 0));

		curr = matrix(c0, c1, c2, c3);

		// ����ġ ū ������� �����ؼ� ����
		// transform += mul(weights[i], curr);
        
        
        // ���� ������
        n0 = TransformsMap.Load(int4(indices[i] * 4 + 0, nextFrame[0], clip[0], 0));
        n1 = TransformsMap.Load(int4(indices[i] * 4 + 1, nextFrame[0], clip[0], 0));
        n2 = TransformsMap.Load(int4(indices[i] * 4 + 2, nextFrame[0], clip[0], 0));
        n3 = TransformsMap.Load(int4(indices[i] * 4 + 3, nextFrame[0], clip[0], 0));
        next = matrix(n0, n1, n2, n3);
        
        // ���� ����
        currAnim = lerp(curr, next, time[0]);
        
        //-> ���� ���� ����
        [flatten]
        if (clip[1] >= 0)
        {
            // ���� Ŭ���� �ִٸ�
            
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