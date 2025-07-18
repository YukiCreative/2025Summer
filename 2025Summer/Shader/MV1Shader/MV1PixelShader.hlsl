#include "PixelShader.h"


// �s�N�Z���V�F�[�_�[�̓���
struct PS_INPUT
{
	float4 Diffuse         : COLOR0 ;		// �f�B�t���[�Y�J���[
#if USESPC
    float4 Specular : COLOR1; // �X�y�L�����J���[
#endif
	float4 TexCoords0_1    : TEXCOORD0 ;	// xy:�e�N�X�`�����W zw:�T�u�e�N�X�`�����W
	float3 VPosition       : TEXCOORD1 ;	// ���W( �r���[��� )
	float3 VNormal         : TEXCOORD2 ;	// �@��( �r���[��� )

	float2 Fog             : TEXCOORD5 ;	// �t�H�O�p�����[�^( x )   �����t�H�O�p�����[�^( y )

    float4 Position : SV_POSITION;
    float3 WorldPosition : POSITION0;
    float4 PositionD : POSITION1; //�f�v�X���ؗp
} ;

// �s�N�Z���V�F�[�_�[�̏o��
struct PS_OUTPUT
{
	float4 Color0          : SV_TARGET0;    // �F
	float4 Normal          : SV_TARGET1;	// �@��( �r���[��� )
	float4 Depth           : SV_TARGET2;	// �[�x
    float4 Distance        : SV_TARGET3;    // ����
    float4 MaterialID      : SV_TARGET4;    //�}�e���A��ID
    
} ;

#if SUBTEXTUREMODE != 0
SamplerState g_SubSampler                   : register( s7 ) ;		// �T�u�e�N�X�`��
Texture2D    g_SubTexture                   : register( t7 ) ;		// �T�u�e�N�X�`��
#endif // SUBTEXTUREMODE != 0

Texture2D tex : register(t0);
SamplerState smp : register(s0);

// main�֐�
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
    
    //���X�^���C�U������Ă�悤�ɂ��Ƃ�Z�l�Ŋ����Ă����܂��B
    //�����Ă����Ȃ���
    //far(z-near)/(far-near)�ɂȂ��Ă��邽�߁A�͈͂�0.0�`1.0�ɂȂ�Ȃ��Ȃ��Ă��܂��B
    //���̂��߁A�{����Z�Ŋ��邽�߂ɖ{����Z�������Ă���w�Ŋ����Ă����܂�
    PSOutput.Depth.r = PSInput.PositionD.z / PSInput.PositionD.w; //0�`1
    PSOutput.Depth.gb = 0.0;
    PSOutput.Depth.a = 1.0; //a=0.0�ɂ���ƁA���e�X�g�ł͂������̂ŁA1.0�����Ă���
	
    float bright = max(saturate(dot(normalize(-g_Common.Light[0].Direction), PSOutput.Normal.xyz)),
				g_Common.Material.Ambient_Emissive + g_Common.Light[0].Ambient);
	
    float3 eyeRay = normalize(-PSInput.VPosition);
    float3 ref = reflect(g_Common.Light[0].Direction, PSOutput.Normal.xyz);
    float spec = pow(saturate(dot(eyeRay, ref)), g_Common.Material.Power);
	
    float4 texCol = tex.Sample(smp, PSInput.TexCoords0_1.xy);
    float4 color = g_Common.Material.Diffuse * texCol * bright + spec;

    PSOutput.MaterialID = float4(0, 0, 1, 1);
    PSOutput.Color0 = float4(color.rgb, g_Common.Material.Diffuse.a);
    return PSOutput;
}