
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
    
    //アルファクリップ
    clip(outDiffuse.a - 0.01);
    
    ////線形フォグ(霧の表現)
    //float3 fogColor = float3(0.2, 0.1, 0.1);//徐々に黒くなる
    //float fog = In.Depth * 0.05;//霧の濃さを調整

    //outDiffuse.rgb = (1 - fog) * outDiffuse.rgb + fog * fogColor;
}
