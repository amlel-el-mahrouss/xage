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
    float3 viewDir;
    float lightIntensity;
    float4 color;
    
    textureColor = gShaderTexture.Sample(SAMPLE_TYPE, input.TEXTURE);
    
    float3 halfwayDir = normalize(input.POSITION.xyz + DIR);
    
    lightIntensity = pow(max(dot(input.NORMAL, halfwayDir), 0.0), 0.5);
    
    color = saturate(COLOR * lightIntensity);
    color = color * textureColor;
    
    return color;
}