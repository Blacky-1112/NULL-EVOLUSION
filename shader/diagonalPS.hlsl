
#include "common.hlsl"


Texture2D		g_Texture : register(t0);
SamplerState	g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{

    if (Material.TextureEnable)
    {
        outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
        outDiffuse *= In.Diffuse;
    }
    else
    {
        outDiffuse = In.Diffuse;
    }
    
    float speed = 0.5f;
    
    // 斜め発光処理
    if (enable > 0.5)
    {
        // 斜め方向（左下 → 右上）
        float diagonal = In.TexCoord.x + In.TexCoord.y;

        // 時間で流す
        float scan = frac(diagonal - time * speed);

        // 光帯マスク
        float glow =
            smoothstep(0.0, GlowWidth, scan) *
            (1.0 - smoothstep(GlowWidth, GlowWidth * 2.0, scan));
        
        float3 glowColor = float3(1.0, 1.0, 1.0);

        // 白色ハイライト的に加算
        outDiffuse.rgb += glow * GlowPower * glowColor;
    }
    
    //アルファクリップ
    clip(outDiffuse.a - 0.01);
}
