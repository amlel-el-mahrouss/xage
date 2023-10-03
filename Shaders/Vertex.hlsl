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
    
    float4 COLOUR; // Light Colour
    float4 SOURCE; // Light source
};

struct PIXEL
{
    float4 POSITION : SV_POSITION;
    float4 AMBIENT : COLOR; // Ambient color
    float4 DIFFUSE : COLOR1; // Diffuse color
    float4 SPECULAR : COLOR2; // Specular color
    float4 NORMAL : NORMAL;
    float2 TEXTURE : TEXCOORD0;
};

struct VERTEX
{
    float4 POSITION : POSITION;
    float4 AMBIENT : COLOR; // Ambient color
    float4 DIFFUSE : COLOR1; // Diffuse color
    float4 SPECULAR : COLOR2; // Specular color
    float4 NORMAL : NORMAL;
    float2 TEXTURE : TEXCOORD0;
};

PIXEL VS(VERTEX input)
{    
    PIXEL output;
    
    output.POSITION = mul(input.POSITION, WORLD);
    output.POSITION = mul(output.POSITION, VIEW);
    output.POSITION = mul(output.POSITION, PROJECTION);
    
    output.AMBIENT = input.AMBIENT;
    output.DIFFUSE = input.DIFFUSE;
    output.NORMAL = input.NORMAL;
    output.SPECULAR = input.SPECULAR;
    output.TEXTURE = input.TEXTURE;
    
    return output;
}