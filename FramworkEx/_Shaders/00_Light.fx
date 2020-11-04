// ADSE
// Ambient
// Diffuse + ����Ʈ
// Specular
// Emissive

struct LightDesc // <- �޽�
{
    float4 Ambient;
    float4 Specular;
    //float4 Emissive;
    
    // Diffuse
    float3 Direction; float Padding_L1;
    float3 Position; // float Padding2;
};


struct MaterialDesc // <- �ؽ���
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
};

cbuffer CB_Light
{
    LightDesc GlobalLight;
};

cbuffer CB_Material
{
    MaterialDesc Material;
};