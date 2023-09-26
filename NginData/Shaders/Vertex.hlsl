struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

VS_OUTPUT VS(float3 pos : POSITION)
{
	VS_OUTPUT outVert;
	outVert.position = float4(pos, 0.f);
	outVert.color = float4(1.f, 1.f, 1.f, 1.f);

	return outVert;
}