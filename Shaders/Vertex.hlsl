struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VS_OUTPUT VS(float4 pos : POSITION, float4 clr : COLOR)
{
    VS_OUTPUT outVert;
    outVert.position = float4(pos);
    outVert.color = clr;

    return outVert;
}