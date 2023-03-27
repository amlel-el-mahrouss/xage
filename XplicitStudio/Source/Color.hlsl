struct VS_INPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

struct PS_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

cbuffer MATRIX_BUFFER
{
	matrix view;
	matrix world;
	matrix projection;
};

PS_OUTPUT PS(VS_INPUT input) : SV_TARGET
{
	PS_OUTPUT output;
	input.position.w = 1.0f;

	output.position = mul(input.position, world);
	output.position = mul(output.position, view);
	output.position = mul(output.position, projection);

	output.color = input.color;

	return output;
}