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
    float4 textureColor;
    float3 lightDir;
    float lightIntensity;
    float4 color;
    float3 reflection;
    float4 specular;

    textureColor = gShaderTexture.Sample(SAMPLE_TYPE, input.TEXTURE);
    textureColor *= gShaderTexture2.Sample(SAMPLE_TYPE, input.TEXTURE);
    
    color = AMBIENT_COLOR;
    specular = SPECULAR_COLOR;
    
    lightDir = -DIRECTION;
    lightIntensity = saturate(dot(input.NORMAL, lightDir));

    if (lightIntensity > 0.0f)
    {
        color += (DIFFUSE_COLOR * lightIntensity);
        color = saturate(color);
        reflection = normalize(2.0f * lightIntensity * input.NORMAL - lightDir);
        specular = pow(saturate(dot(reflection, input.VIEW_DIR)), SPECULAR_POWER);
    }

    color = color * textureColor;
    color = saturate(color * specular);

    return color;
}