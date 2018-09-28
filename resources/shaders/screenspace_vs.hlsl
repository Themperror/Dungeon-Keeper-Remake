#include "defines.hlsl"

cbuffer ObjectBuffer : register(b0)
{
    float4x4 _modelMatrix; //64
};
struct VS_OUTPUT
{
    float4 positionVS : SV_POSITION;
    float2 uv : UV;
};
struct VS_INPUT
{
    float3 position : POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};
VS_OUTPUT VShader(VS_INPUT input)
{
    VS_OUTPUT output;

    float4 mPos = float4(_modelMatrix._41_42_43_44);
    float4x4 scaleMatrix = _modelMatrix;
    scaleMatrix._41_42_43_44 = float4(0, 0, 0, 1);

    output.positionVS = mul(scaleMatrix, float4(input.position, 1.0f)); //scaling
    output.positionVS += mPos; //positioning

    output.uv = float2(input.uv);
    return output;
}
