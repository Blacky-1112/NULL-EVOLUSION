
#include "common.hlsl"


Texture2D		g_Texture : register(t0);
SamplerState	g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    //輪郭抽出(グラディエント)
    
    //float offset = 1.0 / 300.0;
    
    //float4 colorX0 = g_Texture.Sample(g_SamplerState, In.TexCoord);
    //float4 colorX1 = g_Texture.Sample(g_SamplerState, In.TexCoord + float2(offset, 0.0));

    //float4 colorDX = abs(colorX1 - colorX0);
    
    //float4 colorY0 = g_Texture.Sample(g_SamplerState, In.TexCoord);
    //float4 colorY1 = g_Texture.Sample(g_SamplerState, In.TexCoord + float2(0, offset));

    //float4 colorDY = abs(colorY1 - colorY0);
    
    //float4 colorDXY = sqrt(colorDX * colorDX + colorDY * colorDY);
    
    //輪郭抽出(ラプラシアン)
    //float offset = 1.0 / 500.0;
    //float4 colorX0 = g_Texture.Sample(g_SamplerState, In.TexCoord + float2(-offset, 0.0));
    //float4 colorX1 = g_Texture.Sample(g_SamplerState, In.TexCoord + float2(offset, 0.0));
    //float4 colorY0 = g_Texture.Sample(g_SamplerState, In.TexCoord + float2(0.0, -offset));
    //float4 colorY1 = g_Texture.Sample(g_SamplerState, In.TexCoord + float2(0.0, offset));
    //float4 color = g_Texture.Sample(g_SamplerState, In.TexCoord);
    
    //float4 colorDXY = (colorX0 + colorX1 + colorY0 + colorY1) - 4.0 * color;

    //outDiffuse = color - abs(colorDXY) * 2.0;
    //outDiffuse.a = 1.0;
    
    //00 10 20
    //01 11 21
    //02 12 22
    
    //フィルタ
    float offset = 1.0 / 500.0;
    
    float4 color[3][3];
    
    //平均化フィルタのカーネル
    //1/9 1/9  1/9
    //1/9 1/9  1/9
    //1/9 1/9  1/9
    //float filter[3][3] =
    //{
    //    { 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0 },
    //    { 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0 },
    //    { 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0 },
    //};
    
    //ラプラシアン
    //float filter[3][3] =
    //{
    //    { 0.0, 1.0, 0.0 },
    //    { 1.0, -4.0, 1.0 },
    //    { 0.0, 1.0, 0.0 },
    //};
    
    //先鋭化
    //float filter[3][3] =
    //{
    //    { 0.0, -1.0, 0.0 },
    //    { -1.0, 5.0, -1.0 },
    //    { 0.0, -1.0, 0.0 },
    //};
    
    //エンボス
    //float filter[3][3] =
    //{
    //    { 1.0, 0.0, 0.0 },
    //    { 0.0, 0.0, 0.0 },
    //    { 0.0, 0.0, -1.0 },
    //};
    
    //ガウシアン
    //float filter[3][3] =
    //{
    //    { 1.0, 2.0, 1.0 },
    //    { 2.0, 4.0, 2.0 },
    //    { 1.0, 2.0, 1.0 },
    //};
    
    float weight = 0.0;
    for (int y = -5; y <= 5; y++)
    {
        for (int x = -5; x <= 5; x++)
        {
            float lenSq = x * x + y * y;
            float gauss = exp(-lenSq * 0.01);    //小さくするとよりボケる
            outDiffuse += g_Texture.Sample(g_SamplerState, In.TexCoord + float2(x, y) * offset) * gauss;
            //outDiffuse += g_Texture.Sample(g_SamplerState, In.TexCoord + float2(x, y) * offset) * filter[y + 1][x + 1];
            
            weight += gauss;
        }
    }
    
    //outDiffuse /= 16.0;
    outDiffuse /= weight;
    
    outDiffuse.a = 1.0;
}
