// ADSE
// Ambient
// Diffuse + 램버트
// Specular
// Emissive

struct LightDesc // <- 메쉬
{
    float4 Ambient;
    float4 Specular;
    //float4 Emissive;
    
    // Diffuse
    float3 Direction; float Padding_L1;
    float3 Position; // float Padding2;
};


struct MaterialDesc // <- 텍스쳐
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