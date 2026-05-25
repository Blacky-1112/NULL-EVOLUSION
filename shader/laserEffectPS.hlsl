#include "common.hlsl"
float4 main(PS_IN In) : SV_TARGET
{
    float2 uv = In.TexCoord;

    // 横方向フェード（中心明るく）
    float center = 1.0 - abs(uv.x - 0.5) * 2.0;

    // 先端フェード
    float tip = saturate(1.0 - uv.y);

    // 波打ちアニメ
    float wave = sin(uv.y * 20.0 + Frame * 10.0) * 0.1;

    float intensity = center * tip + wave;

    intensity = saturate(intensity);

    float3 color = float3(1.0, 0.2, 1.0);

    return float4(color * intensity * 3.0, intensity);
}