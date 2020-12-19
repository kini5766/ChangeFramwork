//--
// Global CBuffer < 전역 변수가 c버퍼
//--

cbuffer CB_PerFrame
{
	matrix View;
	matrix ViewInverse;
	matrix Projection;
	matrix VP;

	float4 Culling[4];
	float4 Clipping;

	float Time;
};

cbuffer CB_World
{
	matrix World;
};


// --
// Mesh
// --

// output
struct MeshOutput
{
	float4 Position : SV_Position0;
	float3 oPosition : Position1;
	float3 wPosition : Position2;

	float3 Normal : Normal0;
	float3 Tangent : Tangent0;
	float2 Uv : Uv0;
	float4 Color : Color0;
};

//--
// Functions
//--

float4 WorldPosition(float4 position)
{
	return mul(position, World);
}

float4 ViewProjection(float4 position)
{
	position = mul(position, View);
	position = mul(position, Projection);
	return position;
}

float3 WorldNormal(float3 normal)
{
	return mul(normal, (float3x3) World);
}

float3 WorldTangent(float3 tangent)
{
	return mul(tangent, (float3x3) World);
}

float3 ViewPosition()
{
	return ViewInverse._41_42_43;
}

void Decompose(out float3 s, out float4 r, out float3 t, matrix w)
{
	t = w._41_42_43;

	float3 x = float3(w._11_12_13);
	float3 y = float3(w._21_22_23);
	float3 z = float3(w._31_32_33);

	s.x = sqrt(x.x * x.x + x.y * x.y + x.z * x.z);
	s.y = sqrt(y.x * y.x + y.y * y.y + y.z * y.z);
	s.z = sqrt(z.x * z.x + z.y * z.y + z.z * z.z);

	x /= s.x;
	y /= s.y;
	z /= s.z;

	r.w = sqrt(1 + x.x + y.y + z.z) / 2.0f;
	float w4_1 = 1 / (4 * r.w);
	r.x = (z.y - y.z) * w4_1;
	r.y = (x.z - z.x) * w4_1;
	r.z = (y.x - x.y) * w4_1;
}

void Combine(out matrix w, float3 s, float4 r, float3 t)
{
	float xy2 = 2 * r.x * r.y;
	float xz2 = 2 * r.x * r.z;
	float yz2 = 2 * r.y * r.z;
	float xx2 = 2 * r.x * r.x;
	float yy2 = 2 * r.y * r.y;
	float zz2 = 2 * r.z * r.z;
	float xw2 = 2 * r.x * r.w;
	float yw2 = 2 * r.y * r.w;
	float zw2 = 2 * r.z * r.w;

	w._11 = s.x * (1 - yy2 - zz2);
	w._22 = s.y * (1 - xx2 - zz2);
	w._33 = s.z * (1 - xx2 - yy2);

	w._13 = s.x * (xz2 + yw2);
	w._21 = s.y * (xy2 + zw2);
	w._32 = s.z * (yz2 + xw2);

	w._12 = s.x * (xy2 - zw2);
	w._23 = s.y * (yz2 - xw2);
	w._31 = s.z * (xz2 - yw2);

	w._41 = t.x; w._42 = t.y; w._43 = t.z;
	w._14 = w._24 = w._34 = 0.0f; w._44 = 1.0f;
}

float4 Slerp(float4 q1, float4 q2, float t)
{
	float dotQ = dot(q1, q2);
	float rad = acos(dotQ);
	float sinRad = sin(rad);

	if (sinRad < 0.00001f)
	{
		if (t <= 0.5f)
			return q1;
		return q2;
	}

	return (sin(rad * (1 - t)) * q1 + sin(rad * t) * q2) / sinRad;
}

//--
// States
//--

SamplerState PointSampler
{
	Filter = MIN_MAG_MIP_POINT;
	AddressU = WRAP;
	AddressV = WRAP;
};

SamplerState LinearSampler
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = WRAP;
	AddressV = WRAP;
};


RasterizerState FillMode_WireFrame
{
	FillMode = WireFrame;
};

RasterizerState FrontCounterClockwise_True
{
	FrontCounterClockwise = true;
};

RasterizerState CullMode_None
{
	CullMode = None;
	//FillMode = 2;  // 숫자 2도 가능
};


DepthStencilState DepthEnable_False
{
	DepthEnable = false;
};


//--
// VS -> PS
//--
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

#define P_BS_VP(name, bs, vs, ps) \
pass name \
{ \
    SetBlendState(bs, float4(0, 0, 0, 0), 0xFF); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_DSS_VP(name, dss, vs, ps) \
pass name \
{ \
    SetDepthStencilState(dss, 1); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_RS_DSS_VP(name, rs, dss, vs, ps) \
pass name \
{ \
    SetRasterizerState(rs); \
    SetDepthStencilState(dss, 1); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_RS_BS_VP(name, rs, bs, vs, ps) \
pass name \
{ \
    SetRasterizerState(rs); \
    SetBlendState(bs, float4(0, 0, 0, 0), 0xFF); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_DSS_BS_VP(name, dss, bs, vs, ps) \
pass name \
{ \
    SetDepthStencilState(dss, 1); \
    SetBlendState(bs, float4(0, 0, 0, 0), 0xFF); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

//--
// + GS
//--
#define P_VGP(name, vs, gs, ps) \
pass name \
{ \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetGeometryShader(CompileShader(gs_5_0, gs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_RS_VGP(name, rs, vs, gs, ps) \
pass name \
{ \
    SetRasterizerState(rs); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetGeometryShader(CompileShader(gs_5_0, gs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_BS_VGP(name, bs, vs, gs, ps) \
pass name \
{ \
    SetBlendState(bs, float4(0, 0, 0, 0), 0xFF); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetGeometryShader(CompileShader(gs_5_0, gs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_DSS_VGP(name, dss, vs, gs, ps) \
pass name \
{ \
    SetDepthStencilState(dss, 1); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetGeometryShader(CompileShader(gs_5_0, gs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_RS_DSS_VGP(name, rs, dss, vs, gs, ps) \
pass name \
{ \
    SetRasterizerState(rs); \
    SetDepthStencilState(dss, 1); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetGeometryShader(CompileShader(gs_5_0, gs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_RS_BS_VGP(name, rs, bs, vs, gs, ps) \
pass name \
{ \
    SetRasterizerState(rs); \
    SetBlendState(bs, float4(0, 0, 0, 0), 0xFF); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetGeometryShader(CompileShader(gs_5_0, gs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_DSS_BS_VGP(name, dss, bs, vs, gs, ps) \
pass name \
{ \
    SetDepthStencilState(dss, 1); \
    SetBlendState(bs, float4(0, 0, 0, 0), 0xFF); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetGeometryShader(CompileShader(gs_5_0, gs())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}


//--
// + TS
//--
#define P_VTP(name, vs, hs, ds, ps) \
pass name \
{ \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetHullShader(CompileShader(hs_5_0, hs())); \
    SetDomainShader(CompileShader(ds_5_0, ds())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_RS_VTP(name, rs, vs, hs, ds, ps) \
pass name \
{ \
    SetRasterizerState(rs); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetHullShader(CompileShader(hs_5_0, hs())); \
    SetDomainShader(CompileShader(ds_5_0, ds())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_BS_VTP(name, bs, vs, hs, ds, ps) \
pass name \
{ \
    SetBlendState(bs, float4(0, 0, 0, 0), 0xFF); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetHullShader(CompileShader(hs_5_0, hs())); \
    SetDomainShader(CompileShader(ds_5_0, ds())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_DSS_VTP(name, dss, vs, hs, ds, ps) \
pass name \
{ \
    SetDepthStencilState(dss, 1); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetHullShader(CompileShader(hs_5_0, hs())); \
    SetDomainShader(CompileShader(ds_5_0, ds())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_RS_DSS_VTP(name, rs, dss, vs, hs, ds, ps) \
pass name \
{ \
    SetRasterizerState(rs); \
    SetDepthStencilState(dss, 1); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetHullShader(CompileShader(hs_5_0, hs())); \
    SetDomainShader(CompileShader(ds_5_0, ds())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_RS_BS_VTP(name, rs, bs, vs, hs, ds, ps) \
pass name \
{ \
    SetRasterizerState(rs); \
    SetBlendState(bs, float4(0, 0, 0, 0), 0xFF); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetHullShader(CompileShader(hs_5_0, hs())); \
    SetDomainShader(CompileShader(ds_5_0, ds())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_DSS_BS_VTP(name, dss, bs, vs, hs, ds, ps) \
pass name \
{ \
    SetDepthStencilState(dss, 1); \
    SetBlendState(bs, float4(0, 0, 0, 0), 0xFF); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetHullShader(CompileShader(hs_5_0, hs())); \
    SetDomainShader(CompileShader(ds_5_0, ds())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}

#define P_RS_DSS_BS_VTP(name, rs, dss, bs, vs, hs, ds, ps) \
pass name \
{ \
    SetRasterizerState(rs); \
    SetDepthStencilState(dss, 1); \
    SetBlendState(bs, float4(0, 0, 0, 0), 0xFF); \
    SetVertexShader(CompileShader(vs_5_0, vs())); \
    SetHullShader(CompileShader(hs_5_0, hs())); \
    SetDomainShader(CompileShader(ds_5_0, ds())); \
    SetPixelShader(CompileShader(ps_5_0, ps())); \
}