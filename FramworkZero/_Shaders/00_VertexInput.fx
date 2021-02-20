
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
\
output.Culling.x = dot(float4(output.wPosition, 1), Culling[0]);\
output.Culling.y = dot(float4(output.wPosition, 1), Culling[1]);\
output.Culling.z = dot(float4(output.wPosition, 1), Culling[2]);\
output.Culling.w = dot(float4(output.wPosition, 1), Culling[3]);\
\
output.Clipping = float4(0, 0, 0, 0); \
output.Clipping.x = dot(float4(output.wPosition, 1), Clipping); \


// --
// Mesh
// --
struct VertexMesh_Inst
{
	float4 Position : Position0;
	float2 Uv : Uv0;
	float3 Normal : Normal0;
	float3 Tangent : Tangent0;

	matrix Transform : Inst1_Transform0;
};

struct VertexMesh
{
	float4 Position : Position0;
	float2 Uv : Uv0;
	float3 Normal : Normal0;
	float3 Tangent : Tangent0;
};


// --
// Model
// --
struct VertexModel_Inst
{
	float4 Position : Position0;
	float2 Uv : Uv0;
	float3 Normal : Normal0;
	float3 Tangent : Tangent0;
	float4 BlendIndices : BlendIndices0;
	float4 BlendWeights : BlendWeights0;

	matrix Transform : Inst1_Transform0;
	uint InstanceID : SV_InstanceID0;
};

struct VertexModel
{
	float4 Position : Position0;
	float2 Uv : Uv0;
	float3 Normal : Normal0;
	float3 Tangent : Tangent0;
	float4 BlendIndices : BlendIndices0;
	float4 BlendWeights : BlendWeights0;
};


// --
// Model World
// --
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
