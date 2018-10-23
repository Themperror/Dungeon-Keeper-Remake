#include "defines.hlsl"
#include "functions.hlsl"
#include "structs.hlsl"

//Texture2D shaderTexture[4];
Texture2D tex : register(t0);

SamplerState diffSampler : register(s0);

float4 PShader(VS_OUTPUT input) : SV_TARGET
{
    float4 col = tex.Sample(diffSampler, float2(input.uv.x, input.uv.y)).xyzw;
	if(col.a <= 0.1)
    {
        discard;
    }
    return col;
}