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
    float4 PHONG : COLOR; // Ambient color
    float4 PBR : COLOR1; // Diffuse color
    float4 RESERVED : COLOR2; // Specular color
};

struct VERTEX
{
    float4 POSITION : POSITION;
    float4 AMBIENT : COLOR; // Ambient color
    float4 DIFFUSE : COLOR1; // Diffuse color
    float4 SPECULAR : COLOR2; // Specular color
};

PIXEL VS(VERTEX input_data)
{
    float4 normal = normalize(NORMAL);
    float4 lightColor = COLOUR;
    float4 lightSource = SOURCE;
    float diffuseStrength = max(0.1, dot(lightSource, normal));
    float4 diffuse = diffuseStrength * lightColor;
    
    float4 cameraSource = input_data.POSITION;
    float4 viewSource = normalize(cameraSource);
    
    float4 reflectSource = normalize(reflect(-lightSource, normal));
    float specularStrength = max(0.01, dot(viewSource, reflectSource));
    
    specularStrength = pow(specularStrength, 0.5);
    float4 specular = specularStrength * lightColor;
    
    //! phong equation.
    float4 lighting = float4(0.6, 0.6, 0.6, 1.0);
    lighting = input_data.AMBIENT - .1 + ((diffuse / 6.0) * (diffuse / 4.0) + (diffuse / 2.0)) + specular * 0.5;
    
    float4 color = input_data.DIFFUSE * lighting;

    PIXEL output;
    
    output.POSITION = mul(input_data.POSITION, WORLD);
    output.POSITION = mul(output.POSITION, VIEW);
    output.POSITION = mul(output.POSITION, PROJECTION);
    
    output.PHONG = color;
    
    return output;
}