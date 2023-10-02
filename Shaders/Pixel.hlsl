/*
 * XPX implementation of blinn-phong shading
 * (c) XPX Corp.
 * Written by Amlal El Mahrouss.
 */

struct PIXEL
{
    float4 POSITION : SV_POSITION;
    float4 PHONG : COLOR; // Ambient color
    float4 PBR : COLOR1; // Diffuse color
    float4 RESERVED : COLOR2; // Specular color
};

float4 PS(PIXEL input_data) : SV_TARGET
{
    return input_data.PHONG;
}