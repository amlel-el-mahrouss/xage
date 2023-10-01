/*
 * XPX implementation of blinn-phong shading
 * (c) XPX Corp.
 * Written by Amlal El Mahrouss.
 */

cbuffer CBUFFER
{
    float4 normal;
    float3 ambient;
    float3 lighting;
    float3 model_color;
};

struct VS_INPUT
{
    float3 clr;
    float3 src;
    float3 cam_src;
};

float4 PS(VS_INPUT input_data) : SV_TARGET
{
    float3 normal = normalize(normal.xyz);
    float3 lightColor = input_data.clr;
    float3 lightSource = input_data.src;
    float diffuseStrength = max(0.1, dot(lightSource, normal));
    float3 diffuse = diffuseStrength * lightColor;
    
    float3 cameraSource = input_data.cam_src;
    float3 viewSource = normalize(cameraSource);
    
    float3 reflectSource = normalize(reflect(-lightSource, normal));
    float specularStrength = max(0.01, dot(viewSource, reflectSource));
    
    specularStrength = pow(specularStrength, 0.5);
    float3 specular = specularStrength * lightColor;
    
    //! phong equation.
    lighting = ambient - .1 + ((diffuse / 6.0) * (diffuse / 4.0) + (diffuse / 2.0)) + specular * 0.5;
    
    float3 color = model_color * lighting;

    return float4(color, 1.0);
}