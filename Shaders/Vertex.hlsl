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
    float2 TEXTURE : TEXCOORD0;
};

struct VERTEX
{
    float4 POSITION : POSITION;
    float2 TEXTURE : TEXCOORD0;
};

PIXEL VS(VERTEX input)
{    
    PIXEL output;
    
    output.POSITION = mul(input.POSITION, WORLD);
    output.POSITION = mul(output.POSITION, VIEW);
    output.POSITION = mul(output.POSITION, PROJECTION);
    
    output.TEXTURE = input.TEXTURE;
    
    return output;
}