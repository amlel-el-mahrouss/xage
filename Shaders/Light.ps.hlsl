/*
 * XPX implementation of blinn-phong shading
 * (c) XPX Corp.
 * Written by Amlal El Mahrouss.
 */

Texture2D gShaderTexture : register(t0);
Texture2D gShaderTexture2 : register(t1);

SamplerState SAMPLE_TYPE : register(s0);

struct PIXEL
{
    float4 POSITION : SV_POSITION;
    float2 TEXTURE : TEXCOORD0;
    float3 NORMAL : NORMAL;
    float3 VIEW_DIR : TEXCOORD0;
};

cbuffer LIGHT
{
    float4 COLOR;
    float3 DIR;
    float PADDING;
    float SPECULAR_POWER;
    float4 SPECULAR_COLOR;
};

float4 PS(PIXEL input) : SV_TARGET
{
    float4 textureColor;
    float3 lightDir;
    float lightIntensity;

    textureColor = gShaderTexture.Sample(SAMPLE_TYPE, input.TEXTURE);
    textureColor *= gShaderTexture2.Sample(SAMPLE_TYPE, input.TEXTURE);
    
    float3 halfwayDir = normalize(input.VIEW_DIR);
    
    float4 spec = pow(max(dot(input.NORMAL, halfwayDir), 0.0), 28.0);
    float4 specular = textureColor * spec;
    
    return specular;
}