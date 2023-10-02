/*
 * XPX implementation of blinn-phong shading
 * (c) XPX Corp.
 * Written by Amlal El Mahrouss.
 */

cbuffer CBUFFER
{
    matrix WORLD;
    matrix VIEW;
    matrix PROJECTION;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 ambient : COLOR;
    float4 diffuse : COLOR1;
    float4 specular : COLOR2;
};

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    float3 normal = normalize(float3(0.5, 0.5, 0.5));
    float4 lightColor = float4(0.5, 0.5, 0.5, 1);
    float4 lightSource = PROJECTION._11_11_11_11.xyzw;
    float diffuseStrength = max(0.1, dot(lightSource.xyz, normal));
    float4 diffuse = diffuseStrength * lightColor;
    
    float4 cameraSource = input.position;
    float4 viewSource = normalize(cameraSource);
    
    float3 reflectSource = normalize(reflect(-lightSource.xyz, normal));
    float specularStrength = max(0.01, dot(viewSource.xyz, reflectSource));
    
    specularStrength = pow(specularStrength, 0.5);
    float4 specular = specularStrength * lightColor;
    
    float3 lighting = lightColor * .5 + ((diffuse / 6.0) * (diffuse / 4.0) + (diffuse / 2.0)) + input.specular * 0.5;
    
    float3 modelColor = float3(0.94, 0.82, 0.38);
    float3 color = modelColor * lighting;
    
    return float4(color, 1.0);
}