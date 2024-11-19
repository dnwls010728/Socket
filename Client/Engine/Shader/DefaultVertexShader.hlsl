#pragma pack_matrix(row_major)

cbuffer constant : register(b0)
{
	float4x4 mat;
	float2 uv_offset;
	float2 uv_scale;
};

struct VS_INPUT
{
	float3 position : POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	output.position = mul(float4(input.position, 1.f), mat);
	output.color = input.color;
	output.texcoord = (input.texcoord * uv_scale) + uv_offset;
	return output;
}