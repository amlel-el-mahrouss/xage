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
    float4 TRANSLATE;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 ambient : COLOR;
    float4 diffuse : COLOR1;
};

struct VS_INPUT
{
    float4 position : POSITION;
    float4 ambient : COLOR;
    float4 diffuse : COLOR1;
};


VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT output;
    
    input.position.w = 1.0f;
    
    output.position = mul(input.position, WORLD);
    output.position = mul(output.position, VIEW);
    output.position = mul(output.position, PROJECTION);
    output.position += normalize(TRANSLATE);
    
    output.ambient = input.ambient;
    output.diffuse = input.diffuse;

    return output;
}