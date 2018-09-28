#include "defines.hlsl"

cbuffer CameraBuffer : register(b1)
{
    float4x4 viewMatrix;
    float4x4 projectionMatrix;
    float4x4 invProjectionMatrix;
    float4x4 invViewMatrix;
    float4 cameraPosition;
    float4 cameraDir;
};

struct VS_INPUT
{
    float3 position : POSITION;
    float3 color : COLOR;
};
struct VS_OUTPUT
{
    float4 positionVS : SV_POSITION;
    float3 positionWS : POSITION;
    float3 color : COLOR;
};

VS_OUTPUT VShader(VS_INPUT input)
{
	VS_OUTPUT output;
	float4 pos = float4(input.position, 1.0f);
    output.positionWS = input.position.xyz;
    pos = mul(pos, mul(viewMatrix, projectionMatrix));
	output.positionVS = pos;
    output.color = input.color;
	return output;
}