#include "VertexShader.h"

#define USESPC false

// ���_�V�F�[�_�[�̓���
struct VS_INPUT
{
	float3 Position        : POSITION ;			// ���W( ���[�J����� )
	float3 Normal          : NORMAL0 ;			// �@��( ���[�J����� )
	float4 Diffuse         : COLOR0 ;			// �f�B�t���[�Y�J���[
    float3 Binormal		   : BINORMAL;
    float3 Tan			   : TANGENT;
#if USESPC
	float4 Specular        : COLOR1 ;			// �X�y�L�����J���[
#endif
	float4 TexCoords0      : TEXCOORD0 ;		// �e�N�X�`�����W
	float4 TexCoords1      : TEXCOORD1 ;		// �T�u�e�N�X�`�����W
};

// ���_�V�F�[�_�[�̏o��
struct VS_OUTPUT
{
	float4 Diffuse         : COLOR0 ;		// �f�B�t���[�Y�J���[
#if USESPC
	float4 Specular        : COLOR1 ;		// �X�y�L�����J���[
#endif
	float4 TexCoords0_1    : TEXCOORD0 ;	// xy:�e�N�X�`�����W zw:�T�u�e�N�X�`�����W
	float3 VPosition       : TEXCOORD1 ;	// ���W( �r���[��� )
	float3 VNormal         : TEXCOORD2 ;	// �@��( �r���[��� )

	float2 Fog             : TEXCOORD5 ;	// �t�H�O�p�����[�^( x )  �����t�H�O�p�����[�^( y )

	float4 Position        : SV_POSITION ;	// ���W( �v���W�F�N�V������� )
    float3 WorldPosition	: POSITION0;//���Ƃ̃��[���h���W
    float4 PositionD		: POSITION1; //�f�v�X�p
} ;



#ifdef SKINMESH
#define LOCAL_WORLD_MAT         lLocalWorldMatrix
#else
#define LOCAL_WORLD_MAT         g_Base.LocalWorldMatrix
#endif

#define VERTICAL_FOG_OUT        VSOutput.VerticalFog





// main�֐�
VS_OUTPUT main( VS_INPUT VSInput )
{
	VS_OUTPUT	VSOutput ;
	int4		lBoneFloatIndex ;
	float4		lLocalWorldMatrix[ 3 ] ;
	float4		lLocalPosition ;
	float4		lWorldPosition ;
	float4		lViewPosition ;
	float3		lWorldNrm ;
	float3		lWorldTan ;
	float3		lWorldBin ;
	float3		lViewNrm ;
	float3		lViewTan ;
	float3		lViewBin ;
	float		lVerticalFogY ;
	float		lFogDensity ;

	// ���[�J�����W�̃Z�b�g
	lLocalPosition.xyz = VSInput.Position ;
	lLocalPosition.w = 1.0f ;
	
	// ���W�v�Z( ���[�J�����r���[���v���W�F�N�V���� )
	lWorldPosition.x = dot( lLocalPosition, LOCAL_WORLD_MAT[ 0 ] ) ;
	lWorldPosition.y = dot( lLocalPosition, LOCAL_WORLD_MAT[ 1 ] ) ;
	lWorldPosition.z = dot( lLocalPosition, LOCAL_WORLD_MAT[ 2 ] ) ;
	lWorldPosition.w = 1.0f ;
    VSOutput.WorldPosition = lWorldPosition.xyz;

	lViewPosition.x = dot( lWorldPosition, g_Base.ViewMatrix[ 0 ] ) ;
	lViewPosition.y = dot( lWorldPosition, g_Base.ViewMatrix[ 1 ] ) ;
	lViewPosition.z = dot( lWorldPosition, g_Base.ViewMatrix[ 2 ] ) ;
	lViewPosition.w = 1.0f ;

	VSOutput.Position.x = dot( lViewPosition, g_Base.ProjectionMatrix[ 0 ] ) ;
	VSOutput.Position.y = dot( lViewPosition, g_Base.ProjectionMatrix[ 1 ] ) ;
	VSOutput.Position.z = dot( lViewPosition, g_Base.ProjectionMatrix[ 2 ] ) ;
	VSOutput.Position.w = dot( lViewPosition, g_Base.ProjectionMatrix[ 3 ] ) ;
    VSOutput.PositionD = VSOutput.Position;
   
	
	
	// ���W( �r���[��� )��ۑ�
	VSOutput.VPosition = lViewPosition.xyz;
	
	// �@�����v�Z
	lWorldNrm.x = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 0 ].xyz ) ;
	lWorldNrm.y = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 1 ].xyz ) ;
	lWorldNrm.z = dot( VSInput.Normal, LOCAL_WORLD_MAT[ 2 ].xyz ) ;

	lViewNrm.x = dot( lWorldNrm, g_Base.ViewMatrix[ 0 ].xyz ) ;
	lViewNrm.y = dot( lWorldNrm, g_Base.ViewMatrix[ 1 ].xyz ) ;
	lViewNrm.z = dot( lWorldNrm, g_Base.ViewMatrix[ 2 ].xyz ) ;

	// �@��( �r���[��� )��ۑ�
	VSOutput.VNormal = lViewNrm;

	// �f�B�t���[�Y�J���[���Z�b�g
	VSOutput.Diffuse  = g_Base.DiffuseSource  > 0.5f ? VSInput.Diffuse  : g_Common.Material.Diffuse ;
	
	// �X�y�L�����J���[���Z�b�g
#if USESPC
	VSOutput.Specular = ( g_Base.SpecularSource > 0.5f ? VSInput.Specular : g_Common.Material.Specular ) * g_Base.MulSpecularColor ;
#endif
	// ���_���W�ϊ� ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++( �I�� )

	VSOutput.Fog.x = 1.0f;

	VSOutput.Fog.y = 1.0f ;
	if( g_Common.VerticalFog.Mode == 1 || g_Common.VerticalFog.Mode == 2 )
	{
		if( g_Common.VerticalFog.Density < 0.0 )
		{
			lVerticalFogY = lWorldPosition.y - g_Common.VerticalFog.DensityStart;
			lFogDensity = -g_Common.VerticalFog.Density ;
		}
		else
		{
			lVerticalFogY = g_Common.VerticalFog.DensityStart - lWorldPosition.y ;
			lFogDensity = g_Common.VerticalFog.Density ;
		}
		if( lVerticalFogY > 0.0f )
		{
			if( g_Common.VerticalFog.Mode == 1 )
			{
				// �w���t�H�O�v�Z 1.0f / pow( e, ���� * density )
				VSOutput.Fog.y = 1.0f / pow( abs( g_Common.VerticalFog.E ), lVerticalFogY * lFogDensity ) ;
			}
			else
			if( g_Common.VerticalFog.Mode == 2 )
			{
				// �w���t�H�O�Q�v�Z 1.0f / pow( e, ( ���� * density ) * ( ���� * density ) )
				VSOutput.Fog.y = 1.0f / pow( abs( g_Common.VerticalFog.E ), ( lVerticalFogY * lFogDensity ) * ( lVerticalFogY * lFogDensity ) ) ;
			}
		}
	}
	else
	if( g_Common.VerticalFog.Mode == 3 )
	{
		// ���`�t�H�O�v�Z
		VSOutput.Fog.y = lWorldPosition.y * g_Common.VerticalFog.LinearDiv + g_Common.VerticalFog.LinearAdd ;
	}

	// �e�N�X�`�����W�̃Z�b�g
    VSOutput.TexCoords0_1.x = VSInput.TexCoords0.x;
    VSOutput.TexCoords0_1.y = VSInput.TexCoords0.y;

    VSOutput.TexCoords0_1.z = VSInput.TexCoords1.z;
    VSOutput.TexCoords0_1.w = VSInput.TexCoords1.w;

	return VSOutput ;
}

