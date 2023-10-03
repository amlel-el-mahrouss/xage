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

Texture2D gShaderTexture : register(t0);
SamplerState SAMPLE_TYPE : register(s0);

float4 PS(PIXEL input) : SV_TARGET
{
    // color = modelColor * lighting
    float4 color = gShaderTexture.Sample(SAMPLE_TYPE, input.TEXTURE);    
    
    bool scalar_bool = color.xyzw == float4(0, 0, 0, 0).xyzw;
    
    if (scalar_bool)
    {
        color = float4(input.AMBIENT, 1.0);
    }
    
    return color;
}