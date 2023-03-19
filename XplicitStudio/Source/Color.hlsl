struct PixelIn
{
	float4 pos : SV_POSITION;
	float4 clr : COLOR;
};

float4 main(PixelIn input) : SV_TARGET
{
	return input.clr;
}