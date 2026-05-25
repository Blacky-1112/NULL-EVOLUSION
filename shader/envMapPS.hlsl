
#include "common.hlsl"


Texture2D g_Texture : register(t0);
Texture2D g_EnvTexture : register(t1);
SamplerState g_SamplerState : register(s0);

static float PI = 3.141592653589;

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    //視線ベクトル
    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    eyev = normalize(eyev);
    
    //法線ベクトル
    float3 normal = normalize(In.Normal.xyz);
    
    //反射ベクトル
    float3 refv = reflect(eyev, normal);
    refv = normalize(refv);
    
    //屈折ベクトル            //視線、法線、屈折率の順番
    float3 reflactv = refract(eyev, normal, 0.5);
    reflactv = normalize(reflactv);
    
    if (Material.TextureEnable)
    {
        outDiffuse = g_Texture.Sample(g_SamplerState, In.TexCoord);
        outDiffuse *= In.Diffuse;
    }
    else
    {
        outDiffuse = In.Diffuse;
    }
    
    //環境マッピングテクスチャ座標
    float2 envTexCoord;
    envTexCoord.x = atan2(refv.x, refv.z) / (PI * 2) + 0.5f;
    envTexCoord.y = acos(refv.y) / PI;
    
    //環境マッピング
    float3 envMap = g_EnvTexture.SampleLevel(g_SamplerState, envTexCoord, 6);
    
    //屈折マッピングテクスチャ座標
    float2 refTexCoord;
    refTexCoord.x = atan2(reflactv.x, reflactv.z) / (PI * 2) + 0.5f;
    refTexCoord.y = acos(reflactv.y) / PI;
    
    //屈折マッピング
    float3 refMap = g_EnvTexture.SampleLevel(g_SamplerState, refTexCoord, 6);
    
    //フレネル反射
    float f0 = 0.05;
    float d = dot(-eyev, normal);
    float fresnel = f0 + (1.0 - f0) * pow((1.0 - d), 4.0);
    
    //合成
    //outDiffuse.rgb = lerp(refMap, envMap, fresnel);
    outDiffuse.rgb = refMap;
    
    outDiffuse.a = 1.0;
    
}
