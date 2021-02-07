
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