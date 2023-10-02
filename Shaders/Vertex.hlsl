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

volatile float4 NORMAL;

struct VS_OUTPUT
{
    float4 position : POSITION;
    float4 ambient : COLOR;
    float4 diffuse : COLOR1;
    float4 specular : COLOR2;
    float4 normal : POSITION1;
};

struct VS_INPUT
{
    float4 position : POSITION;
    float4 ambient : COLOR;
    float4 diffuse : COLOR1;
    float4 specular : COLOR2;
};


VS_OUTPUT VS(VS_INPUT input)
{
    NORMAL = normalize(input.position);
    
    VS_OUTPUT output;
    
    input.position.w = 1.0f;
    
    output.position = mul(input.position, WORLD);
    output.position = mul(output.position, VIEW);
    output.position = mul(output.position, PROJECTION);
    
    output.ambient = input.ambient;
    output.diffuse = input.diffuse;
    output.specular = input.specular;
    output.normal = NORMAL;

    return output;
}