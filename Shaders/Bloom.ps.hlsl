/*
 * XPX implementation of blinn-phong shading
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

#define XPX_FIXED_GAMMA    2.2

float4 PS(PIXEL input) : SV_TARGET
{
    float4 texture = gShaderTexture.Sample(SAMPLE_TYPE, input.TEXTURE);

    float4 result = pow(texture, float4(1.0, 1.0, 1.0, 1.0) / XPX_FIXED_GAMMA);
    
    result += texture;
    result *= texture;
    
    return result;
}