#include "common.hlsl"

PS_IN main(VS_IN In)
{
    PS_IN Out;

    float4 wp = mul(In.Position, World);
    float4 vp = mul(wp, View);
    float4 pp = mul(vp, Projection);

    Out.Position = pp;
    Out.WorldPosition = wp;
    Out.TexCoord = In.TexCoord;
    Out.Diffuse = float4(1, 1, 1, 1);
    Out.Normal = float3(0, 1, 0);
    Out.Depth = pp.z / pp.w;

    return Out;
}