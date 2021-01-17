#include "00_Global.fx"
#include "00_Light.fx"
#include "00_VertexInput.fx"

Texture2D InvBindPose;
Texture2DArray<float4> BonesMap;


// --
// Function
// --

void SetSkinnedModelWorld_Inst(inout matrix world, in VertexModel_Inst input)
{
	int indices[4] = {
		input.BlendIndices.x,
		input.BlendIndices.y,
		input.BlendIndices.z,
		input.BlendIndices.w
	};

	float weights[4] = {
		input.BlendWeights.x,
		input.BlendWeights.y,
		input.BlendWeights.z,
		input.BlendWeights.w
	};

	uint instance = input.InstanceID;

	// 가중치가 누적된 정점의 위치
	float4 c0, c1, c2, c3;
	float4 n0, n1, n2, n3;
	matrix transform = 0;
	matrix curr = 0; matrix inv = 0;

	[unroll(4)]
	for (int i = 0; i < 4; i++)
	{
		c0 = BonesMap[int3(indices[i] * 4 + 0, instance, 0)];
		c1 = BonesMap[int3(indices[i] * 4 + 1, instance, 0)];
		c2 = BonesMap[int3(indices[i] * 4 + 2, instance, 0)];
		c3 = BonesMap[int3(indices[i] * 4 + 3, instance, 0)];
		curr = matrix(c0, c1, c2, c3);

		n0 = InvBindPose.Load(int3(indices[i] * 2 * 4 + 0, 0, 0));
		n1 = InvBindPose.Load(int3(indices[i] * 2 * 4 + 1, 0, 0));
		n2 = InvBindPose.Load(int3(indices[i] * 2 * 4 + 2, 0, 0));
		n3 = InvBindPose.Load(int3(indices[i] * 2 * 4 + 3, 0, 0));
		inv = matrix(n0, n1, n2, n3);

		curr = mul(inv, curr);

		transform += mul(weights[i], curr);
	}

	world = mul(transform, world);
}

void SetModelWorld(inout matrix world, in VertexModel_Inst input)
{
	float4 n0, n1, n2, n3;
	int index = input.BlendIndices.x;
	n0 = InvBindPose.Load(int3((index * 2 + 1) * 4 + 0, 0, 0));
	n1 = InvBindPose.Load(int3((index * 2 + 1) * 4 + 1, 0, 0));
	n2 = InvBindPose.Load(int3((index * 2 + 1) * 4 + 2, 0, 0));
	n3 = InvBindPose.Load(int3((index * 2 + 1) * 4 + 3, 0, 0));
	matrix transform = matrix(n0, n1, n2, n3);
	world = mul(transform, world);
}


// --
// VS
// --

MeshOutput VS_Model_Inst(VertexModel_Inst input)
{
	MeshOutput output;

	World = input.Transform;

	if (any(input.BlendWeights))
		SetSkinnedModelWorld_Inst(World, input);
	else
		SetModelWorld(World, input);

	// input -> output
	VS_GENERATE

	return output;
}


// --
// PS
// --

float4 PS(MeshOutput input) : SV_Target0
{
	return Lighting_MeshOutput(input);
}


technique11 T0
{
	P_VP(P0, VS_Model_Inst, PS)
}