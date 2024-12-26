cbuffer constant : register(b0)
{
	// 16 bytes
	float r;
	float g;
	float b;
	float a;

	// 16 bytes
	bool has_texture;
	bool padding[3];
};

struct PS_INPUT
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	float2 texcoord : TEXCOORD;
};

Texture2D texture_ : register(t0);
SamplerState sampler_ : register(s0);

float4 main(PS_INPUT input) : SV_TARGET
{
	float4 color = input.color;
	if (has_texture)
	{
		color *= texture_.Sample(sampler_, input.texcoord);
	}
	
	color.r *= r;
	color.g *= g;
	color.b *= b;
	color.a *= a;

	float gamma = 2.2f;
	color.rgb = pow(color.rgb, 1. / gamma);
	
	return color;
}