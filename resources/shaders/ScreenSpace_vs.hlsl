#include "Defines.hlsl"

struct VS_OUTPUT
{
    float4 positionVS : SV_POSITION;
    float2 uv : UV;
};
struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 bitangent : BITANGENT;
    float2 uv : UV;
};
VS_OUTPUT VShader(VS_INPUT input)
{
    VS_OUTPUT output;
    output.positionVS = float4(input.position, 1.0f);
    output.uv = float2(input.uv);
    return output;
}
