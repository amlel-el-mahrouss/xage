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


struct VERTEX
{
    float4 POSITION : POSITION;
    float4 AMBIENT : COLOR; // Ambient color
    float4 DIFFUSE : COLOR1; // Diffuse color
    float4 SPECULAR : COLOR2; // Specular color
};

PIXEL VS(VERTEX input_data)
{
    PIXEL output;
    
    output.POSITION = mul(input_data.POSITION, WORLD);
    output.POSITION = mul(output.POSITION, VIEW);
    output.POSITION = mul(output.POSITION, PROJECTION);
    
    output.AMBIENT = input_data.AMBIENT;
    output.DIFFUSE = input_data.DIFFUSE;
    output.SPECULAR = input_data.SPECULAR;
   
    return output;
}