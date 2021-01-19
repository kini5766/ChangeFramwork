#include "00_Global.fx"
#include "00_PixelOutput.fx"
#include "00_VertexInput.fx"


Texture2D InvBindPose;
// Skinned 컴퓨트 쉐이더 처리 된 결과
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

void SetModelWorld(inout matrix world, int index)
{
	float4 n0, n1, n2, n3;
	n0 = InvBindPose.Load(int3((index * 2 + 1) * 4 + 0, 0, 0));
	n1 = InvBindPose.Load(int3((index * 2 + 1) * 4 + 1, 0, 0));
	n2 = InvBindPose.Load(int3((index * 2 + 1) * 4 + 2, 0, 0));
	n3 = InvBindPose.Load(int3((index * 2 + 1) * 4 + 3, 0, 0));
	matrix transform = matrix(n0, n1, n2, n3);
	world = mul(transform, world);
}

void SetModelWorld_All_Inst(inout matrix world, in VertexModel_Inst input)
{
	world = input.Transform;

	if (any(input.BlendWeights))
		SetSkinnedModelWorld_Inst(world, input);
	else
		SetModelWorld(world, input.BlendIndices.x);
}


// >-- None --< //

// --
// VS
// --

MeshOutput VS_Model_Inst(VertexModel_Inst input)
{
	MeshOutput output;

	SetModelWorld_All_Inst(World, input);

	// input -> output
	VS_GENERATE

	return output;
}


// >-- Shadow + ProjectionTexture --< //

// --
// VS_Shadow
// --
// 1pass Depth
MeshDepthOutput VS_Model_Inst_Depth(VertexModel_Inst input)
{
	MeshDepthOutput output = (MeshDepthOutput)0;

	SetModelWorld_All_Inst(World, input);

	// input -> output
	VS_DEPTH_GENERATE

	return output;
}
// 2pass Shadow
MeshOutput_Shadow VS_Model_Inst_Shadow(VertexModel_Inst input)
{
	MeshOutput_Shadow output = (MeshOutput_Shadow) 0;

	SetModelWorld_All_Inst(World, input);

	// input -> output
	VS_GENERATE

	VSSet_Shadow(output.sPosition, output.wPosition);

	return output;
}


technique11 T0
{
	// None
	P_VP(P0, VS_Model_Inst, PS_MeshOutput)

	// ProjectionTexture
	P_VP(P1, VS_Model_Inst, PS_MeshOutput_ProjT)

	// Shadow + ProjectionTexture
	P_VP(P2, VS_Model_Inst_Depth, PS_Shadow_Depth)
	P_VP(P3, VS_Model_Inst_Shadow, PS_MeshOutput_Shadow)
}