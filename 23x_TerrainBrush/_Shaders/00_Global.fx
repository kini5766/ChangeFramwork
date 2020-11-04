cbuffer CB_PerFrame
{
    matrix View;
    matrix ViewInverse;
    matrix Projection;
    matrix VP;

    float Time;
    // float3 Padding;  // 쉐이더에서는 맨마지막 패딩 생략 가능
};

cbuffer CB_World
{
    matrix World;
};

Texture2D DiffuseMap;
Texture2D SpecularMap;
Texture2D NormalMap;

TextureCube SkyCubeMap;

// Function /////////////////////////////////////////////////////////////////////////////

float4 WorldPosition(float4 position)
{
    return mul(position, World);
}

float4 ViewProjection(float4 position)
{
    return mul(position, VP);
    
    //position = mul(position, View);
    //return mul(position, Projection);
}

float3 WorldNormal(float3 normal)
{
    return mul(normal, (float3x3) World);
}

float3 ViewPosition()
{
	return float3(ViewInverse._41_42_43);
}

// Mesh /////////////////////////////////////////////////////////////////////////////

struct MeshOutput
{
    float4 Position : SV_Position0; // sv가 픽셀화 됨
    float4 wvpPosition : Position1; //WVP Position
    float3 oPosition : Position2; //NDC Position
    float3 wPosition : Position3; //W Position

    float2 Uv : Uv;
    float3 Normal : NORMAL0;
};

// Types /////////////////////////////////////////////////////////////////////////////

struct Vertex
{
    float4 Position : POSITION0;
};

struct VertexColor
{
    float4 Position : POSITION0;
    float4 Color : COLOR0;
};

struct VertexNormal
{
    float4 Position : POSITION0;
    float3 Normal : NORMAL0;
};

struct VertexColorNormal
{
    float4 Position : POSITION0;
    float4 Color : COLOR0;
    float3 Normal : NORMAL0;
};

struct VertexTexture
{
    float4 Position : POSITION0;
    float2 Uv : UV0;
};

struct VertexTextureNormal
{
    float4 Position : POSITION0;
    float2 Uv : UV0;
    float3 Normal : NORMAL0;
};

// States /////////////////////////////////////////////////////////////////////////////

SamplerState LinearSampler
{
    Filter = MIN_MAG_MIP_LINEAR;
    
    AddressU = Wrap;
    AddressV = Wrap;
};

RasterizerState FillMode_WireFrame
{
    FillMode = WIREFRAME;
};

RasterizerState FrontCounterClockwise_True
{
    FrontCounterClockwise = true;
};

DepthStencilState DepthEnable_False
{
    DepthEnable = false;
};

// BlendState : 겹친 픽셀처리
// Src : 전면
// Dest : 후면
BlendState AlphaBlend
{
	BlendEnable[0] = true;
	DestBlend[0] = INV_SRC_ALPHA;
	SrcBlend[0] = SRC_ALPHA;
	BlendOp[0] = Add;

	SrcBlendAlpha[0] = ZERO;
	DestBlendAlpha[0] = ZERO;
	BlendOpAlpha[0] = Add;

	RenderTargetWriteMask[0] = 15;
};

BlendState AlphaBlend_AlphaToCoverage
{
	AlphaToCoverageEnable = true;
	BlendEnable[0] = true;
	DestBlend[0] = INV_SRC_ALPHA;
	SrcBlend[0] = SRC_ALPHA;
	BlendOp[0] = Add;

	SrcBlendAlpha[0] = ZERO;
	DestBlendAlpha[0] = ZERO;
	BlendOpAlpha[0] = Add;

	RenderTargetWriteMask[0] = 15;
};

// Generate Macro /////////////////////////////////////////////////////////////////////////////
#define VS_GENERATE \
    output.oPosition = input.Position.xyz;\
    \
    output.Position = WorldPosition(input.Position);\
    output.wPosition = output.Position.xyz;\
    \
    output.Position = ViewProjection(output.Position);\
    output.wvpPosition = output.Position;\
    \
    output.Normal = WorldNormal(input.Normal);\
    output.Uv = input.Uv;

// PipeLine /////////////////////////////////////////////////////////////////////////////

#define P_VP(name, vs, ps) \
pass name \
{ \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_RS_VP(name, rs, vs, ps) \
pass name \
{ \
    SetRasterizerState(rs); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_RS_DSS_VP(name, rs, dss, vs, ps) \
pass name \
{ \
    SetRasterizerState(rs); \
    SetDepthStencilState(dss, 0); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_BS_VP(name, bs, vs, ps) \
pass name \
{ \
    SetBlendState(bs, float4(0, 0, 0, 0), 0xFF); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}