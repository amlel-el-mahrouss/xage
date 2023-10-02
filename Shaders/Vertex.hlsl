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

struct VERTEX
{
    float4 POSITION : POSITION;
    float4 AMBIENT : COLOR; // Ambient color
    float4 DIFFUSE : COLOR1; // Diffuse color
    float4 SPECULAR : COLOR2; // Specular color
};

float4 PS(VERTEX input_data) : SV_TARGET
{
    float3 normal = normalize(NORMAL.xyz);
    float3 lightColor = COLOUR;
    float3 lightSource = SOURCE.xyz;
    float diffuseStrength = max(0.1, dot(lightSource, normal));
    float3 diffuse = diffuseStrength * lightColor;
    
    float3 cameraSource = input_data.POSITION;
    float3 viewSource = normalize(cameraSource);
    
    float3 reflectSource = normalize(reflect(-lightSource, normal));
    float specularStrength = max(0.01, dot(viewSource, reflectSource));
    
    specularStrength = pow(specularStrength, 0.5);
    float3 specular = specularStrength * lightColor;
    
    //! phong equation.
    float3 lighting = float3(0.6, 0.6, 0.6);
    lighting = input_data.AMBIENT.xyz - .1 + ((diffuse / 6.0) * (diffuse / 4.0) + (diffuse / 2.0)) + specular * 0.5;
    
    float3 color = input_data.DIFFUSE.xyz * lighting;

    return float4(color, 1.0);
}