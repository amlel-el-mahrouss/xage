/*
 * XPX implementation of blinn-phong shading
 * (c) XPX Corp.
 * Written by Amlal El Mahrouss.
 */

cbuffer CBUFFER
{
    matrix VIEW;
    matrix WORLD;
    matrix PROJECTION;
    
    float4 NORMAL;
    float4 COLOUR; // Light Colour
    float4 SOURCE; // Light source
};

struct PIXEL
{
    float4 POSITION : SV_POSITION;
    float4 AMBIENT : COLOR; // Ambient color
    float4 DIFFUSE : COLOR; // Diffuse color
    float4 SPECULAR : COLOR; // Specular color
};

float4 PS(PIXEL input_data) : SV_TARGET0
{
    float3 normal = normalize(NORMAL.xyz);
    float3 lightColor = COLOUR.xyz;
    float3 lightSource = SOURCE.xyz;
    float lightDistance = length(lightSource);
    float diffuseStrength = max(0.1, dot(lightSource, normal));

    float3 cameraSource = input_data.POSITION.xyz;
    float3 viewSource = normalize(cameraSource);
    
    float3 reflectSource = normalize(reflect(-lightSource, normal));
    float specularStrength = max(0.01, dot(viewSource, reflectSource));
    
    specularStrength = pow(specularStrength, 0.5);
    float3 specular = specularStrength * lightColor;
    
    float3 diffuse = (diffuseStrength + lightColor / lightDistance);
    
    diffuse += specular * input_data.SPECULAR.xyz;
    
    return float4(diffuse, 1.0);
}