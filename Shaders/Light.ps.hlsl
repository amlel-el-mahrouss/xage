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
};

cbuffer LIGHT
{
    float4 COLOR;
    float3 DIR;
    float PADDING;
};

float4 PS(PIXEL input) : SV_TARGET
{
    float4 textureColor;
    float3 lightDir;
    float lightIntensity;

    textureColor = gShaderTexture.Sample(SAMPLE_TYPE, input.TEXTURE);
    textureColor *= gShaderTexture2.Sample(SAMPLE_TYPE, input.TEXTURE);
    
    lightDir = normalize(DIR - input.POSITION.xyz);
    float3 viewDir = normalize(input.POSITION.xyz - DIR);
    float3 halfwayDir = normalize(lightDir + viewDir);
    
    float spec = pow(max(dot(normalize(input.NORMAL), halfwayDir), 0.0), 16.0);
    float3 specular = textureColor * spec;
    
    return float4(specular, 1.0);
}