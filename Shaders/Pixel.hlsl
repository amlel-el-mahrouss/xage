struct VS_INPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

float4 PS(VS_INPUT input) : SV_TARGET
{
    return input.color;
}