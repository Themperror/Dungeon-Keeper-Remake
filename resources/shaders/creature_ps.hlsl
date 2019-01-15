#include "defines.hlsl"
#include "functions.hlsl"
#include "structs.hlsl"

//Texture2D shaderTexture[4];
Texture2D tex : register(t0);

SamplerState diffSampler : register(s0);

float4 PShader(VS_OUTPUT input) : SV_TARGET
{
    float4 col = tex.Sample(diffSampler, input.uv).xyzw;
	if(col.a <= 0.1)
    {
        discard;
    }
    if (_isFrozen) //add lightblue color
    {
        col += float4(0.447, 0.761, 1, 0.0);
    }
    if (_isFighting && fmod(_time, 0.15) > 0.075) //add flashing lightred color
    {
        col += float4(0.8, 0.1, 0.15, 0.0);
    }
    if (_isHovered && fmod(_time, 0.15) > 0.075) //add flashing white color
    {
        col += float4(0.4, 0.4, 0.4, 0.0);
    }
    return col;
}