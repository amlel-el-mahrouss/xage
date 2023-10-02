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
    float4 NORMAL : NORMAL; // Specular color
};

float4 PS(PIXEL input_vs) : SV_TARGET0
{
    // ambient lighting (global illuminance)
    float3 ambient = float3(0.7, 0.7, 0.7); // color - grey

    // diffuse (lambertian) lighting
    // lightColor, lightSource, normal, diffuseStrength
    float3 normal = normalize(input_vs.NORMAL.xyz);
    float3 lightColor = float3(1.0, 1.0, 1.0); // color - white
    float3 lightSource = SOURCE.xyz; // coord - (1, 0, 0)
    float diffuseStrength = max(0.1, dot(lightSource, normal));
    float3 diffuse = diffuseStrength * lightColor;

    // specular light
    // lightColor, lightSource, normal, specularStrength, viewSource
    float3 cameraSource = float3(1.1, 1.1, 1.0);
    float3 viewSource = normalize(cameraSource);
    float3 reflectSource = normalize(reflect(-lightSource, normal));
    float specularStrength = max(0.01, dot(viewSource, reflectSource));
    specularStrength = pow(specularStrength, 0.5);
    float3 specular = specularStrength * lightColor;

    // lighting = ambient + diffuse + specular
    float3 lighting = float3(1, 1, 1); // color - black
    // lighting = ambient;
    // lighting = ambient * 0.0 + diffuse;
    // lighting = ambient * 0.0 + diffuse * 0.0 + specular;
    lighting = ambient - .5 + ((diffuse / 6.0) * (diffuse / 4.0) + (diffuse / 2.0)) + specular * 0.5;

    // color = modelColor * lighting
    float3 modelColor = float3(1.0, 1.0, 1.0);
    float3 color = modelColor * lighting;

    return float4(color, 1.0);
}