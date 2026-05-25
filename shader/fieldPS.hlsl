
#include "common.hlsl"


Texture2D		g_Texture : register(t0);
Texture2D		g_Texture1 : register(t1);
SamplerState	g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{

    if (Material.TextureEnable)
    {
        outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord) * In.Diffuse.a;
        outDiffuse += g_Texture1.Sample(g_SamplerState, In.TexCoord) * (1.0 - In.Diffuse.a);
        outDiffuse *= In.Diffuse;
    }
    else
    {
        outDiffuse = In.Diffuse;
    }

    //çÇÇ≥ÉtÉHÉO
    float3 heightFogColor = float3(0.0, 0.1, 0.2);
    float heightFog = saturate(-In.WorldPosition.y * 0.5f);
    outDiffuse.rgb = (1 - heightFog) * outDiffuse.rgb + heightFogColor * heightFog;
    
    outDiffuse.a = 1.0;

}
