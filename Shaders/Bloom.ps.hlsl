/*
 * XPX implementation of bloom shading
 * (c) XPX Corp.
 * Written by Amlal El Mahrouss.
 */

Texture2D gShaderTexture : register(t0);

SamplerState SAMPLE_TYPE : register(s0);

struct PIXEL
{
    float4 POSITION : SV_POSITION;
    float2 TEXTURE : TEXCOORD0;
    float3 NORMAL : NORMAL;
};

cbuffer LIGHT
{
    float4 AMBIENT_COLOR;
    float4 DIFFUSE_COLOR;
    float3 DIRECTION;
    float PADDING;
    float SPECULAR_POWER;
    float4 SPECULAR_COLOR;
};

float4 PS(PIXEL input) : SV_TARGET
{
    float4 hdr_tex = gShaderTexture.Sample(SAMPLE_TYPE, input.TEXTURE);
    
    float4 result = float4(1.0, 1.0, 1.0, 1.0) - exp(-hdr_tex * SPECULAR_COLOR);
    result = pow(hdr_tex, 2.2);
    
    result.w = 1.0;
    
    return result;
}