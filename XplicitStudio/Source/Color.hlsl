float4 main(float3 clr : COLOR) : SV_TARGET
{
	return float4(clr.r, clr.g, clr.b, 1.0f);
}