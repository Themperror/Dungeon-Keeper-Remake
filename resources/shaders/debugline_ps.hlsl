#include "defines.hlsl"

struct VS_OUTPUT
{
    float4 positionVS : SV_POSITION;
    float3 positionWS : POSITION;
    float3 color : COLOR;
};

float4 PShader(VS_OUTPUT input) : SV_Target
{
	float4 output = float4(input.color,1.0);
    return output;
}