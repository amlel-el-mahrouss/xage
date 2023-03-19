cbuffer MatrixBuffer
{
	matrix world_matrix;
	matrix view_matrix;
	matrix projection_matrix;
};

struct VertexInput
{
	float4 position : POSITION;
	float4 color : COLOR;
};

struct PixelOutput
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
};

PixelOutput main(VertexInput vertex_input)
{
	PixelOutput pixel_output;
	vertex_input.position.w = 1.0f;

	pixel_output.position = mul(vertex_input.position, world_matrix);
	pixel_output.position = mul(pixel_output.position, view_matrix);
	pixel_output.position = mul(pixel_output.position, projection_matrix);

	pixel_output.color = vertex_input.color;

	return pixel_output;
}