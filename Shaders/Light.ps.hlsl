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
 // ambient lighting (global illuminance)
    float3 ambient = float3(0.6, 0.6, 0.6); // color - grey

    // diffuse (lambertian) lighting
    // lightColor, lightSource, normal, diffuseStrength
    float3 normal = input.NORMAL;
    float3 lightColor = DIFFUSE_COLOR; // color - white
    float3 lightSource = input.POSITION.xyz - DIRECTION; // coord - (1, 0, 0)
    float diffuseStrength = max(0.1, dot(lightSource, normal));
    float3 diffuse = diffuseStrength * lightColor;

    // specular light
    // lightColor, lightSource, normal, specularStrength, viewSource
    float3 cameraSource = input.POSITION.xyz;
    float3 viewSource = normalize(cameraSource);
    float3 reflectSource = normalize(reflect(-lightSource, normal));
    float specularStrength = max(0.01, dot(viewSource, reflectSource));
    specularStrength = pow(specularStrength, SPECULAR_POWER);
    float3 specular = specularStrength * lightColor;

    // lighting = ambient + diffuse + specular
    float3 lighting = SPECULAR_COLOR.xyz; // color - black
    // lighting = ambient;
    // lighting = ambient * 0.0 + diffuse;
    // lighting = ambient * 0.0 + diffuse * 0.0 + specular;
    lighting = ambient * .5 + ((diffuse / 6.0) * (diffuse / 4.0) + (diffuse / 2.0)) + specular * 0.5;

    // color = modelColor * lighting
    float3 modelColor = gShaderTexture.Sample(SAMPLE_TYPE, input.TEXTURE);
    modelColor *= gShaderTexture2.Sample(SAMPLE_TYPE, input.TEXTURE);
    
    float3 color = modelColor * lighting;

    return float4(color, 1.0);
}