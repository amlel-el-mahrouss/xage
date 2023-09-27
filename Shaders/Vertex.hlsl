cbuffer CBUFFER
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

struct VS_INPUT
{
    float4 position : POSITION;
    float4 color : COLOR;
};

VS_OUTPUT VS(VS_INPUT input)
{
    VS_OUTPUT outVert;
    
    input.position.w = 1.0f;
    
    outVert.position = mul(input.position, worldMatrix);
    outVert.position = mul(outVert.position, viewMatrix);
    outVert.position = mul(outVert.position, projectionMatrix);
    
    outVert.color = input.color;

    return outVert;
}