#include "defines.hlsl"

struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 localPos : POSITION;
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

VS_OUTPUT VShader(VS_INPUT input)
{
    VS_OUTPUT output;
    float4 pos = float4(input.position, 1.0);
    output.localPos = pos;
    output.position = mul(mul(pos, _modelMatrix), mul(_viewMatrix, _projectionMatrix));
    output.uv = input.uv;
    output.normal = input.normal;
    return output;
}