#include "MV1Shader/PixelShader.h"
#define USESPC false

// ピクセルシェーダーの入力
struct PS_INPUT
{
    float4 Diffuse : COLOR0; // ディフューズカラー
#if USESPC
    float4 Specular : COLOR1; // スペキュラカラー
#endif
    float4 TexCoords0_1 : TEXCOORD0; // xy:テクスチャ座標 zw:サブテクスチャ座標
    float3 VPosition : TEXCOORD1; // 座標( ビュー空間 )
    float3 VNormal : TEXCOORD2; // 法線( ビュー空間 )

    float2 Fog : TEXCOORD5; // フォグパラメータ( x )   高さフォグパラメータ( y )

    float4 Position : SV_POSITION;
    float3 WorldPosition : POSITION0;
    float4 PositionD : POSITION1; //デプス検証用
};

// ピクセルシェーダーの出力
struct PS_OUTPUT
{
    float4 Color0 : SV_TARGET0; // 色
    float4 Normal : SV_TARGET1; // 法線( ビュー空間 )
    float4 Depth : SV_TARGET2; // 深度
    float4 Distance : SV_TARGET3; // 距離
};

cbuffer TimeBuff : register(b4)
{
    float time;
    float3 dammy;
    float4 dammy2;
};

Texture2D tex : register(t0);
Texture2D dissolveTex : register(t4);
SamplerState smp : register(s0);

// main関数
PS_OUTPUT main(PS_INPUT PSInput)
{
    PS_OUTPUT PSOutput;
    
    PSOutput.Normal.x = (PSInput.VNormal.x);

    PSOutput.Normal.y = (PSInput.VNormal.y);

    PSOutput.Normal.z = (PSInput.VNormal.z);
    PSOutput.Normal.xyz = normalize(PSOutput.Normal.xyz);
    PSOutput.Normal.w = 1.0;
    
    PSOutput.Distance.x = length(PSInput.VPosition) / 1000.0f;
    PSOutput.Distance.yz = 0.0;
    PSOutput.Distance.w = 1.0;
    
    //ラスタライザがやってるようにもとのZ値で割ってあげます。
    //割ってあげないと
    //far(z-near)/(far-near)になっているため、範囲が0.0～1.0にならなくなっています。
    //このため、本来のZで割るために本来のZが入っているwで割ってあげます
    PSOutput.Depth.r = PSInput.PositionD.z / PSInput.PositionD.w; //0～1
    PSOutput.Depth.gb = 0.0;
    PSOutput.Depth.a = 1.0; //a=0.0にすると、αテストではじかれるので、1.0を入れておく
    
    PSOutput.Color0 = float4(1,0,1,1);
    
    return PSOutput;
	
    float dissolve = dissolveTex.Sample(smp, PSInput.TexCoords0_1.xy).r;
    
    float threshold = dissolve - time;
	
    const float amb = 0.2;
    
    float bright = max(saturate(dot(normalize(-g_Common.Light[0].Direction), PSInput.VNormal.xyz)), amb);
	
    float3 eyeRay = normalize(-PSInput.VPosition);
    float3 ref = reflect(g_Common.Light[0].Direction, PSInput.VNormal.xyz);
    float spec = pow(saturate(dot(eyeRay, ref)), g_Common.Material.Power);
	
    float4 texCol = tex.Sample(smp, PSInput.TexCoords0_1.xy);
    float4 color = g_Common.Material.Diffuse * texCol * bright + spec;
	
    if (threshold > 0)
    {
        // 白を乗算
        texCol = lerp(texCol, float4(1, 1, 1, 1), 0.8);
    }
    
    float edge = pow(1.0 - saturate(abs(threshold)), 20);
    float4 edgeColor = float4(0, 0, 0, 1); // 端黒
    PSOutput.Color0 = lerp(texCol, edgeColor, edge);
    return PSOutput;
}