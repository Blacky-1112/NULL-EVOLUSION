#include "common.hlsl"

void main(in VS_IN In, out PS_IN Out)
{
    matrix wvp;
    wvp = mul(World, View);
    wvp = mul(wvp, Projection);
    
    float4 worldNormal, normal;
    normal = float4(In.Normal.xyz, 0.0);
    worldNormal = mul(normal, World);
    worldNormal = normalize(worldNormal);
    
    float3 lightDirection = normalize(Light.Direction.xyz);
    float light = 0.5 - -dot(lightDirection, worldNormal.xyz) * 0.5;
    light = saturate(light);

    
    Out.Diffuse = In.Diffuse * Material.Diffuse * light * Light.Diffuse;
    Out.Diffuse += In.Diffuse * Material.Ambient * Light.Ambient;
    Out.Diffuse += Material.Emission;
    Out.Diffuse.a = In.Diffuse.a * Material.Diffuse.a;
    
    Out.Position = mul(In.Position, wvp);
    Out.TexCoord = In.TexCoord;
    Out.Depth = Out.Position.z;
    
    Out.Normal = worldNormal.xyz;
    Out.WorldPosition = mul(In.Position, World);
}