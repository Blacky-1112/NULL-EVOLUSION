
#include "common.hlsl"


void main(in VS_IN In, out PS_IN Out)
{

    float4 pos = float4(In.Position.xyz, 1.0f);

    pos = mul(pos, World);
    pos = mul(pos, View);
    pos = mul(pos, Projection);

    Out.Position = pos;
    Out.TexCoord = In.TexCoord;
    Out.Diffuse = In.Diffuse * Material.Diffuse;
}

