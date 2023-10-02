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
    float4 DIFFUSE : COLOR1; // Diffuse color
    float4 SPECULAR : COLOR2; // Specular color
};

PIXEL PS(PIXEL input_data) : SV_TARGET
{
    float3 normal = normalize(NORMAL.xyz - input_data.POSITION.xyz);
    float3 lightColor = COLOUR.xyz;
    float3 lightSource = SOURCE.xyz;
    float diffuseStrength = max(0.1, dot(lightSource, normal));
    float3 diffuse = diffuseStrength * lightColor;
    
    float3 cameraSource = input_data.POSITION.xyz;
    float3 viewSource = normalize(cameraSource);
    
    float3 reflectSource = normalize(reflect(-lightSource, normal));
    float specularStrength = max(0.01, dot(viewSource, reflectSource));
    
    specularStrength = pow(specularStrength, 0.5);
    float3 specular = specularStrength * lightColor;
    
    //! phong equation.
    float4 lighting = float4(0.6, 0.6, 0.6, 1.0);
    lighting = float4(input_data.AMBIENT.xyz - 0.5 + (diffuse / 24.0) * (specular * 0.5), 1.0);
    
    input_data.AMBIENT = lighting;
    
    return input_data;
}