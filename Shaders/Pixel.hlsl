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
    float4 ambient : COLOR;
    float4 diffuse : COLOR1;
    float4 specular : COLOR2;
};

float4 PS(VS_OUTPUT input) : SV_TARGET
{
    return input.ambient;
}