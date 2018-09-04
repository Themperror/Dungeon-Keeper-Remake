#include "Defines.hlsl"
#include "Functions.hlsl"
#include "Structs.hlsl"

//Texture2D shaderTexture[4];
Texture2D diff : register(t0);

SamplerState diffSampler : register(s0);

float4 PShader(VS_OUTPUT input) : SV_TARGET
{
    float4 output = diff.Sample(diffSampler, float2(input.uv.x, 1.0 - input.uv.y));
    return output;
}