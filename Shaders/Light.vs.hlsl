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
};

struct VERTEX
{
    float4 POSITION : POSITION;
    float2 TEXTURE : TEXCOORD0;
    float3 NORMAL : NORMAL;
};

struct PIXEL
{
    float4 POSITION : SV_POSITION;
    float2 TEXTURE : TEXCOORD0;
    float3 NORMAL : NORMAL;
};

PIXEL VS(VERTEX input)
{    
    PIXEL output;
    
    input.POSITION.w = 1.0;

    output.POSITION = mul(input.POSITION, WORLD);
    output.POSITION = mul(output.POSITION, VIEW);
    output.POSITION = mul(output.POSITION, PROJECTION);
    
    output.TEXTURE = input.TEXTURE;
    
    output.NORMAL = mul(input.NORMAL, (float3x3)WORLD);
    output.NORMAL = normalize(output.NORMAL);

    return output;
}