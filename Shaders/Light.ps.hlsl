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
    float3 VIEW_DIR : TEXCOORD1;
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
    float4 tex = gShaderTexture.Sample(SAMPLE_TYPE, input.TEXTURE);
    tex *= gShaderTexture2.Sample(SAMPLE_TYPE, input.TEXTURE);
    
    float4 ambient = 0.5 * tex;
    
    float3 lightDir = normalize(DIRECTION - input.POSITION.xyz);
    float3 viewDir = normalize(input.VIEW_DIR - input.POSITION.xyz);
    float3 halfwayDir = normalize(lightDir + input.VIEW_DIR);
    
    float4 spec = pow(max(dot(input.NORMAL, halfwayDir), 0.0), SPECULAR_POWER);
    
    float diff = max(dot(input.VIEW_DIR, input.NORMAL), 0.0);

    float4 diffuse = tex * diff;
    
    return spec * ambient * diffuse;
}