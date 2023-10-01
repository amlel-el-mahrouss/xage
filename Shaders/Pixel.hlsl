/*
 * XPX implementation of blinn-phong shading
 * (c) XPX Corp.
 * Written by Amlal El Mahrouss.
 */

struct VS_INPUT
{
    float4 position : SV_POSITION;
    float4 ambient : COLOR;
    float4 diffuse : COLOR;
    float4 specular : COLOR;
    float4 normal : COLOR;
};

float4 PS(VS_INPUT input) : SV_TARGET
{
    float3 normal = normalize(input.normal.xyz);
    float3 lightColor = input.ambient;
    float3 lightSource = input.position;
    float diffuseStrength = max(0.1, dot(lightSource, normal));
    float3 diffuse = diffuseStrength * lightColor;
    
    float3 cameraSource = input.position;
    float3 viewSource = normalize(cameraSource);
    
    float3 reflectSource = normalize(reflect(-lightSource, normal));
    float specularStrength = max(0.01, dot(viewSource, reflectSource));
    
    specularStrength = pow(specularStrength, 0.5);
    float3 specular = specularStrength * lightColor;
    
    float4 lighting = ((diffuse / 6.0) * (diffuse / 4.0) + (diffuse / 2.0)) + specular * 0.5;
    
    float3 color = input.ambient * lighting;

    return float4(color, 1.0);
}