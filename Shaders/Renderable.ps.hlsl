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

float4 PS(PIXEL input) : SV_TARGET
{
    float4 color1 = gShaderTexture1.Sample(SAMPLE_TYPE, input.TEXTURE);
    
    float result = saturate(color1 * 2.0).xyzw;
    
    float4 ret = saturate(color1 * 2.0);

    return result > 0.0 ? ret : float4(input.AMBIENT, 1.0);
}