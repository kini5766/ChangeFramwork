
// --
// VS_GENERATE
// --

// input -> output
#define VS_GENERATE \
output.oPosition = input.Position.xyz; \
\
output.Position = WorldPosition(input.Position); \
output.wPosition = output.Position.xyz; \
\
output.Position = ViewProjection(output.Position); \
\
output.Normal = WorldNormal(input.Normal); \
output.Tangent = WorldTangent(input.Tangent); \
output.Uv = input.Uv; \
output.Color = input.Color; \


// --
// Mesh
// --

// Input
struct VertexMesh
{
	float4 Position : Position0;
	float2 Uv : Uv0;
	float3 Normal : Normal0;
	float3 Tangent : Tangent0;

	matrix Transform : Inst1_Transform0;
	float4 Color : Inst2_Color0;
};

// function
void SetMeshWorld(inout matrix world, VertexMesh input)
{
	world = input.Transform;
}

// VS
MeshOutput VS_Mesh(VertexMesh input)
{
	MeshOutput output;

	SetMeshWorld(World, input);
	VS_GENERATE// input -> output

	return output;
}

// --
// Model
// --

// Input
struct VertexModel
{
	float4 Position : Position0;
	float2 Uv : Uv0;
	float3 Normal : Normal0;
	float3 Tangent : Tangent0;
	float4 BlendIndices : BlendIndices0;
	float4 BlendWeights : BlendWeights0;

	matrix Transform : Inst1_Transform0;
	float4 Color : Inst2_Color0;

	uint InstanceID : SV_InstanceID0;
};

Texture2D InvBindPose;
Texture2DArray<float4> BonesMap;

void SetSkinnedModelWorld(inout matrix world, in VertexModel input)
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

void SetModelWorld(inout matrix world, in VertexModel input)
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

MeshOutput VS_Model(VertexModel input)
{
	MeshOutput output;

	World = input.Transform;

	if (any(input.BlendWeights))
		SetSkinnedModelWorld(World, input);
	else 
		SetModelWorld(World, input);

	VS_GENERATE// input -> output

	return output;
}


// --
// SkyCube
// --

float4 PS_Sky(MeshOutput input) : SV_Target0
{
	return SkyCubeMap.Sample(LinearSampler, input.oPosition);
}
