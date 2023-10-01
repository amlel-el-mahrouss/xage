/*
 * XPX implementation of blinn-phong shading
 * (c) XPX Corp.
 * Written by Amlal El Mahrouss.
 */

cbuffer CBUFFER
{
    matrix WORLD;
    matrix VIEW;
    matrix PROJECTION;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 ambient : SV_COLOR0;
    float4 diffuse : SV_COLOR1;
};

struct VS_INPUT
{
    float4 position : POSITION;
    float4 ambient : COLOR0;
    float4 diffuse : COLOR1;
};


VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    input.position.w = 1.0f;
    
    output.position = mul(input.position, WORLD);
    output.position = mul(output.position, VIEW);
    output.position = mul(output.position, PROJECTION);
    
    output.ambient = input.ambient;
    output.diffuse = input.diffuse;

    return output;
}