Texture2D sceneTex : register(t0);
SamplerState samLinear : register(s0);

cbuffer PostCB : register(b0)
{
	float2 resolution;
	float   blurRadius;        // 0.5~1.5
	float   vignetteStrength;  // 0~1
	float   gamma;             // <1 for dimming
};

float4 main(float4 pos : SV_POSITION, float2 uv : TEXCOORD) : SV_Target
{
	// 1) Rec.709 그레이스케일
	float3 col = sceneTex.Sample(samLinear, uv).rgb;
	float gray = dot(col, float3(0.2126, 0.7152, 0.0722));
	col = float3(gray, gray, gray);

	// 2) 비네팅
	float2 centered = uv * 2.0 - 1.0;
	float vignette = saturate(1.0 - dot(centered, centered) * vignetteStrength);
	col *= vignette;

	// 3) 간단한 가우시안 블러 (9-tap)
	float2 texel = 1.0 / resolution;
	float3 blur = col * 0.2941176;                  // center weight
	blur += sceneTex.Sample(samLinear, uv + texel * float2( 1, 0)).rgb * 0.117647;
	blur += sceneTex.Sample(samLinear, uv + texel * float2(-1, 0)).rgb * 0.117647;
	blur += sceneTex.Sample(samLinear, uv + texel * float2( 0, 1)).rgb * 0.117647;
	blur += sceneTex.Sample(samLinear, uv + texel * float2( 0,-1)).rgb * 0.117647;
	blur += sceneTex.Sample(samLinear, uv + texel * float2( 1, 1)).rgb * 0.0588235;
	blur += sceneTex.Sample(samLinear, uv + texel * float2(-1, 1)).rgb * 0.0588235;
	blur += sceneTex.Sample(samLinear, uv + texel * float2( 1,-1)).rgb * 0.0588235;
	blur += sceneTex.Sample(samLinear, uv + texel * float2(-1,-1)).rgb * 0.0588235;
	col = blur;

	// 4) 감마 낮춰서 디밍
	col = pow(col, gamma);      // gamma < 1.0 → 어두워짐

	return float4(col, 1.0);
}
