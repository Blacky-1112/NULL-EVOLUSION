#include "common.hlsl"

StructuredBuffer<float3> Position : register(t2);
StructuredBuffer<float3> Scale : register(t3);

void main(in VS_IN In, out PS_IN Out)
{
    float3 localPos = In.Position.xyz * Scale[In.InstanceId];

    float heightFactor = saturate(localPos.y);

    float3 worldBase = Position[In.InstanceId];

    float wave = sin(
        Time * WindSpeed +
        worldBase.x * 0.2 +
        worldBase.z * 0.2
    );

    float3 windOffset =
        float3(WindDir.x, 0.0f, WindDir.y) *
        wave *
        WindStrength *
        heightFactor;

    localPos += windOffset;
    
    //ëêÇì•ÇÒÇ≈Ç¢ÇÈÇ©
    float dist = distance(PlayerPos, worldBase);
    
    if(dist<PlayerRadius)
    {
        //float ratio = saturate(1.0 -dist / PlayerRadius);
        float ratio = 1.0 - smoothstep(0.0f, PlayerRadius, dist);
        
        //ëêÇí·Ç≠Ç∑ÇÈ
        localPos.y *= (1.0 - ratio * 0.6);
        
        //ì|ÇÍÇÈï˚å¸
        float lean = ratio * ratio;
        float3 dir = normalize(worldBase - PlayerPos);
        localPos.x += dir.x * lean * 0.7;
        localPos.z += dir.z * lean * 0.7;
        
        //è≠ÇµïÇÇ©ÇπÇÈ
        localPos.y += 0.02;
        
        //êFÇè≠Çµà√Ç≠ÇµÇƒéãäoìIÇ…ÇÌÇ©ÇËÇ‚Ç∑Ç≠
        float Shade = lerp(1.0, 0.7, ratio);
        Out.Diffuse.rgb *= Shade;
    }

    float4 worldPos = mul(float4(localPos, 1.0f), World);

    worldPos.xyz += worldBase;

    float4 viewPos = mul(worldPos, View);
    Out.Position = mul(viewPos, Projection);

    Out.TexCoord = In.TexCoord;
    Out.Diffuse = In.Diffuse * Material.Diffuse;

    Out.Depth = Out.Position.z;

    Out.Normal = In.Normal.xyz;
    Out.WorldPosition = worldPos;
}

