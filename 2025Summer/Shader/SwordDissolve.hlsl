#include "MV1Shader/PixelShader.h"
#define USESPC false

// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
    float4 Diffuse : COLOR0; // �f�B�t���[�Y�J���[
#if USESPC
    float4 Specular : COLOR1; // �X�y�L�����J���[
#endif
    float4 TexCoords0_1 : TEXCOORD0; // xy:�e�N�X�`�����W zw:�T�u�e�N�X�`�����W
    float3 VPosition : TEXCOORD1; // ���W( �r���[��� )
    float3 VNormal : TEXCOORD2; // �@��( �r���[��� )

    float2 Fog : TEXCOORD5; // �t�H�O�p�����[�^( x )   �����t�H�O�p�����[�^( y )

    float4 Position : SV_POSITION;
    float3 WorldPosition : POSITION0;
    float4 PositionD : POSITION1; //�f�v�X���ؗp
};

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
    float4 Color0 : SV_TARGET0; // �F
};

cbuffer BoundingBox : register(b4)
{
    float3 vmin;
    float dammy;
    float3 vmax;
    float  time;
};

Texture2D tex : register(t0);
Texture2D dissolveTex : register(t4);
SamplerState smp : register(s0);

// main�֐�
PS_OUTPUT main(PS_INPUT PSInput)
{
    PS_OUTPUT PSOutput;
    
    float3 bbUVW0_1 = (PSInput.WorldPosition - vmin.xyz) / (vmax.xyz - vmin.xyz);
	
    float dissolve = dissolveTex.Sample(smp, PSInput.TexCoords0_1.xy).r;
    
    float threshold = dissolve - time;
    
    if (threshold > 0)
    {
        discard;
    }
	
    const float amb = 0.2;
    
    float bright = max(saturate(dot(normalize(-g_Common.Light[0].Direction), PSInput.VNormal.xyz)), amb);
	
    float3 eyeRay = normalize(-PSInput.VPosition);
    float3 ref = reflect(g_Common.Light[0].Direction, PSInput.VNormal.xyz);
    float spec = pow(saturate(dot(eyeRay, ref)), g_Common.Material.Power);
	
    float4 texCol = tex.Sample(smp, PSInput.TexCoords0_1.xy);
    float4 color = g_Common.Material.Diffuse * texCol * bright + spec;
	
    float edge = pow(1.0 - saturate(abs(threshold)), 20);
    float4 edgeColor = float4(1, 0, 1, 1); // ��
    PSOutput.Color0 = lerp(texCol, edgeColor, edge);
    return PSOutput;
}