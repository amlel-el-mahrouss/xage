struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VS_OUTPUT VS(float2 pos : POSITION) : SV_Position
{
    return float4(pos, 0, 1);
}