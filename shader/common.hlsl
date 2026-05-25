


cbuffer WorldBuffer : register(b0)
{
	matrix World;
}
cbuffer ViewBuffer : register(b1)
{
	matrix View;
}
cbuffer ProjectionBuffer : register(b2)
{
	matrix Projection;
}
cbuffer CameraBuffer : register(b5)
{
    float4 CameraPosition;
}
cbuffer WindParam : register(b6)
{
    float Time;			//経過時間
    float WindStrength;	//揺れの強さ
    float WindSpeed;	//アニメーション速度
    float2 WindDir;		//風向き(x,z)
}
cbuffer PlayerPos : register(b7)
{
	float3 PlayerPos;
	float padding0;
	float PlayerRadius;	//草が潰れる範囲
	float3 padding1;
}
cbuffer DiagonalGlowBuffer : register(b8)
{
    float GlowPower;
    float GlowWidth;
    float time;
    float enable;	//1.0がON
};
cbuffer BeamParam : register(b9)
{
    float BeamLength;
    float BeamWidth;
    float Frame;
    float Padding;
}

struct MATERIAL
{
	float4 Ambient;
	float4 Diffuse;
	float4 Specular;
	float4 Emission;
	float Shininess;
	bool TextureEnable;
	float2 Dummy;
};

cbuffer MaterialBuffer : register(b3)
{
	MATERIAL Material;
}




struct LIGHT
{
	bool Enable;
	bool3 Dummy;
	float4 Direction;
	float4 Diffuse;
	float4 Ambient;
};

cbuffer LightBuffer : register(b4)
{
	LIGHT Light;
}





struct VS_IN
{
	float4 Position		: POSITION0;
	float4 Normal		: NORMAL0;
	float4 Diffuse		: COLOR0;
	float2 TexCoord		: TEXCOORD0;
    uint InstanceId		: SV_InstanceID;
};


struct PS_IN
{
	float4 Position		: SV_POSITION;
	float4 Diffuse		: COLOR0;
	float2 TexCoord		: TEXCOORD0;
    float Depth			: DEPTH0;
	
    float3 Normal		: NORMAL0;
    float4 WorldPosition : POSITION0;
};