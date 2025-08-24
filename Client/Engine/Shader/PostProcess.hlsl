Texture2D sceneTex : register(t0);
SamplerState samLinear : register(s0);

cbuffer constant : register(b0)
{
    float2 resolution; // (width, height)
    float blurRadius; // 0.5 ~ 2.0 (픽셀 단위)
    float vignetteStrength; // 0 ~ 1
    float gamma; // >1 = 어둡게, <1 = 밝게

    float grayscale;
    float2 padding;
}

static const float3 LUMA = float3(0.2126, 0.7152, 0.0722);

float3 SampleColor(float2 uv)
{
    return sceneTex.Sample(samLinear, uv).rgb; // sRGB 리소스면 자동 선형화
}

struct PS_INPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 texcoord : TEXCOORD;
};

float4 main(PS_INPUT input) : SV_Target
{
    float2 texel = rcp(resolution);
    float2 step = texel * blurRadius;
    float2 uv = input.texcoord;

    // 3x3 가우시안 유사 커널: [1 2 1; 2 5 2; 1 2 1] / 17
    float3 acc = SampleColor(uv) * 5.0;
    acc += SampleColor(uv + float2(step.x, 0.0)) * 2.0;
    acc += SampleColor(uv + float2(-step.x, 0.0)) * 2.0;
    acc += SampleColor(uv + float2(0.0, step.y)) * 2.0;
    acc += SampleColor(uv + float2(0.0, -step.y)) * 2.0;
    acc += SampleColor(uv + float2(step.x, step.y)) * 1.0;
    acc += SampleColor(uv + float2(-step.x, step.y)) * 1.0;
    acc += SampleColor(uv + float2(step.x, -step.y)) * 1.0;
    acc += SampleColor(uv + float2(-step.x, -step.y)) * 1.0;
    float3 col = acc * (1.0 / 17.0);

    // 비네트 (블러 후 적용 권장)
    float2 p = uv * 2.0 - 1.0; // [-1,1]^2
    float v = saturate(1.0 - dot(p, p) * vignetteStrength);
    col *= v;

    float gray = dot(col, LUMA);
    col = lerp(col, gray.xxx, grayscale);

    // 감마/노출: 선형 공간에서 마지막에
    col = pow(saturate(col), gamma); // gamma>1이면 어두워짐

    return float4(col, 1.0);
}
