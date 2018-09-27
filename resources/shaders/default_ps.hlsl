#include "Defines.hlsl"
#include "Functions.hlsl"
#include "Structs.hlsl"

//Texture2D shaderTexture[4];
Texture2D TextureAtlas : register(t0);

SamplerState diffSampler : register(s0);

float4 PShader(VS_OUTPUT input) : SV_TARGET
{
   // if (input.normal.x < -0.5 || input.normal.x > 0.5)
   // {
   //     float4 face1 = float4(XTexture.Sample(diffSampler, float2(input.uv.x, input.uv.y)).xyz, 1.0);
   //     return float4(1, 0, 0, 1);
   //     return face1;
   // }
   // if (input.normal.y < -0.5 || input.normal.y > 0.5)
   // {
   //     float4 face2 = float4(YTexture.Sample(diffSampler, float2(input.uv.x, input.uv.y)).xyz, 1.0);
   //    return float4(0, 1, 0, 1);
   //     return face2;
   // }
   // if (input.normal.z < -0.5 || input.normal.z > 0.5)
   // {
   //     float4 face3 = float4(ZTexture.Sample(diffSampler, float2(input.uv.x, input.uv.y)).xyz, 1.0);
	//	
   //     return float4(0,0,1, 1);
   //     return face3;
   // }

    float4 face = float4(TextureAtlas.Sample(diffSampler, float2(input.uv.x, input.uv.y)).xyz, 1.0);
    return face;
    //return float4(input.uv.x*3.0, input.uv.y*3.0, 0.5, 1);
}