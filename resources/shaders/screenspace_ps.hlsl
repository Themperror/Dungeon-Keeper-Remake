#include "defines.hlsl"

Texture2D diff : register(t0);
SamplerState diffSampler : register(s0);

struct VS_OUTPUT
{
    float4 positionVS : SV_POSITION;
    float2 uv : UV;
};
struct PS_OUTPUT
{
    float4 col : SV_Target;
    float depth : SV_Depth;
};
PS_OUTPUT PShader(VS_OUTPUT input)
{
    PS_OUTPUT output;
    output.col = diff.Sample(diffSampler, float2(input.uv.x, 1.0 - input.uv.y));
	
    if (output.col.a <= 0.01f)
    {
        discard;
    }
    output.depth = input.positionVS.z;
    return output;
}
