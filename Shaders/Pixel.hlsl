/*
 * XPX implementation of blinn-phong shading
 * (c) XPX Corp.
 * Written by Amlal El Mahrouss.
 */

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 ambient : COLOR0;
    float4 diffuse : COLOR1;
};

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    float3 normal = normalize(float3(0.5, 0.5, 0.5));
    float4 lightColor = input.ambient;
    float4 lightSource = input.position;
    float diffuseStrength = max(0.1, dot(lightSource.xyz, normal));
    float4 diffuse = diffuseStrength * lightColor;
    
    float4 cameraSource = input.position;
    float4 viewSource = normalize(cameraSource);
    
    float3 reflectSource = normalize(reflect(-lightSource.xyz, normal));
    float specularStrength = max(0.01, dot(viewSource.xyz, reflectSource));
    
    specularStrength = pow(specularStrength, 0.5);
    float4 specular = specularStrength * lightColor;
    
    float4 lighting = lightColor * .5 + ((diffuse / 6.0) * (diffuse / 4.0) + (diffuse / 2.0)) + specular * 0.5;
    
    float4 color = input.ambient * lighting;
    return color;
}