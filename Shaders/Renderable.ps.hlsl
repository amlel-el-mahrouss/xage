/*
 * XPX implementation of blinn-phong shading
 * (c) XPX Corp.
 * Written by Amlal El Mahrouss.
 */

struct PIXEL
{
    float4 POSITION : POSITION;
    float2 TEXTURE : TEXCOORD0;
    float3 AMBIENT : COLOR0;
};

SamplerState SAMPLE_TYPE : register(s0);

Texture2D gShaderTexture1 : register(t0);
Texture2D gShaderTexture2 : register(t1);
Texture2D gShaderTexture3 : register(t2);

float4 PS(PIXEL input) : SV_TARGET
{
    float4 color1 = gShaderTexture1.Sample(SAMPLE_TYPE, input.TEXTURE);
    float4 color2 = gShaderTexture2.Sample(SAMPLE_TYPE, input.TEXTURE);
    float4 color3 = gShaderTexture3.Sample(SAMPLE_TYPE, input.TEXTURE);
    
    return saturate(color1 * color2 * color3 * 2.0);
}