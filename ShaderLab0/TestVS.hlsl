struct PosNormalTex
{
    float3 PosL     : POSITION;
    float3 NormalL  : NORMAL;
    float2 Tex      : TEXCOORD;
};

struct VertOut
{
    float4 PosH    : SV_POSITION;
    float3 PosW    : POSITION;
    float3 NormalW : NORMAL;
    float2 Tex     : TEXCOORD;
};

struct DirectionalLight
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular;
    float3 Direction;
    float pad;
};

struct Material
{
    float4 Ambient;
    float4 Diffuse;
    float4 Specular; // w = SpecPower
    float4 Reflect;
};

cbuffer PerFrame : register(cb0)
{
    float3 EyePosW;
    DirectionalLight DirLight;
};

cbuffer PerMesh: register(cb1)
{
    float4x4 World;
    float4x4 View;
    float4x4 Proj;
    Material Mat;
};

VertOut main( PosNormalTex vin ) 
{
    VertOut vout;

    vout.PosW = mul(World, float4(vin.PosL, 1.0f)).xyz;
    //vout.NormalW = mul(vin.NormalL, (float3x3)gWorldInvTranspose);

    // Transform to homogeneous clip space.
    float4x4 wvp = mul(Proj, mul(View, World));

    vout.PosH = mul(float4(vin.PosL, 1.0f), wvp );
    vout.Tex = vin.Tex;

    return vout;
}