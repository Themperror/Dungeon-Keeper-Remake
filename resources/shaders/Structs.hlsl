#include "Defines.hlsl"
#pragma pack_matrix( row_major )

struct Light
{
    int enabled, ldummy0, ldummy1, ldummy2;
    float4 position;
    float4 color;
    float4x4 lightprojmatrix;
};
struct DirectionalLight : Light
{
    float4 direction;
    float4x4 lightviewmatrix;
    float4 texOffset;
};
struct SpotLight : Light
{
    float4 direction;
    float angleMin, angleMax, range, dummyRW; 
    float4x4 lightviewmatrix;
    float4 texOffset;
};
struct PointLight : Light
{
    float4x4 lightviewmatrix[6]; //128
    float4 texOffset[6];
};


struct VS_SHADOW_OUTPUT
{
    float4 positionVS : SV_POSITION;
    float3 positionWS : POSITION;
    //uint rti : SV_RenderTargetArrayIndex;
    uint vpi : SV_ViewportArrayIndex;
};


struct VS_OUTPUT
{
	float4 positionVS : SV_POSITION;
	float3 normalWS : NORMAL0;
	float3 tangentWS : TANGENT;
	float3 bitangentWS : BITANGENT;
	float3 positionWS : POSITION;
	float2 uv : UV0;
};
struct VS_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float3 bitangent : BITANGENT;
	float2 uv : UV;
};
cbuffer ObjectBuffer : register(b0)
{
	float4x4 modelMatrix; //64
};
cbuffer CameraBuffer : register(b1)
{
	float4x4 viewMatrix;
	float4x4 projectionMatrix;
	float4x4 invProjectionMatrix;
    float4x4 invViewMatrix;
    float4 cameraPosition;
    float4 cameraDir;
};
cbuffer ConstantBuffer : register(b2)
{
    float _screenWidth;
    float _screenHeight;
    float _visualType;
    float _SkyboxLerp;

    float _shadowType;
    float _globalRoughness;
    float _globalMetallic;
    float _MSAAValue;

    float _time;
    float _F0x;
    float _F0y;
    float _F0z;

    float _numCascades;
    float _shadow_atlas_size;
    float _kernel_size;
    float _d7;
};
cbuffer MeshBuffer : register(b3)
{
	bool _hasNormal; //4
	bool _hasRoughness; //8
	bool _hasMisc; //12
	bool _isEmissive; //16
    float _Metallic;
    float _Roughness;
    float _EmissiveStrength;
    float _F0;
};
cbuffer LightConstantBuffer : register(b4)
{
    uint _numDir, _numSpot, _numPoint, _dummy;
    DirectionalLight _dirLights[NUM_LIGHTS];
    PointLight _pointLights[NUM_LIGHTS];
    SpotLight _spotLights[NUM_LIGHTS];
};