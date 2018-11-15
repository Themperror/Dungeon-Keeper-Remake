#include "defines.hlsl"
#pragma pack_matrix( row_major )

struct Light
{
    int enabled, ldummy0, ldummy1, ldummy2;
    float4 position;
    float4 color;
};
struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};
struct VS_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
    float2 uv : UV;
};
cbuffer ObjectBuffer : register(b0)
{
	float4x4 _modelMatrix; //64
};
cbuffer CameraBuffer : register(b1)
{
	float4x4 _viewMatrix;
	float4x4 _projectionMatrix;
	float4x4 _invProjectionMatrix;
    float4x4 _invViewMatrix;
    float4 _cameraPosition;
    float4 _cameraDir;
};
cbuffer ConstantBuffer : register(b2)
{
    float _screenWidth;
    float _screenHeight;
    float _visualType;
    float _time;
};
cbuffer CreatureDataBuffer : register(b3) 
{
	float _AnimIndex;
	float _NumAnim;
	float _SpriteWidth;
	bool _isFrozen; 
    bool _isFlipped;
    float _cdummy0; 
    float _cdummy1; 
    float _cdummy2;   
};
cbuffer LightConstantBuffer : register(b4)
{
    uint _numPoint, _ldummy0, _ldummy1, _ldummy2;
};