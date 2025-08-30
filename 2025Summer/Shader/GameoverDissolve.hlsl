#include "MV1Shader/PixelShader.h"
#define USESPC false

// ピクセルシェーダーの入力
struct PS_INPUT
{
    float4 Position : SV_POSITION;
    float4 DiffuseColor : COLOR0;
    //float4 SpecularColor : COLOR1;
    float2 TextureCoord0 : TEXCOORD0;
    float2 TextureCoord1 : TEXCOORD1;
};

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
    float4 Color0 : SV_TARGET0; // 色
};

cbuffer BoundingBox : register(b4)
{
    // 0~1
    float time;
    float3 dammy;
};

Texture2D tex : register(t0);
Texture2D dissolveTex : register(t1);
SamplerState smp : register(s0);

// main関数
PS_OUTPUT main(PS_INPUT PSInput)
{
    PS_OUTPUT PSOutput;
	
    float dissolve = dissolveTex.Sample(smp, PSInput.TextureCoord0.xy).r;
    
    float threshold = dissolve - time;
    
    if (threshold > 0)
    {
        discard;
    }
	
    float4 texCol = tex.Sample(smp, PSInput.TextureCoord0.xy);
	
    float edge = pow(1.0 - saturate(abs(threshold)), 20);
    float4 edgeColor = float4(0, 1, 1, 1); // 青
    PSOutput.Color0 = lerp(texCol, edgeColor, edge);
    return PSOutput;
}