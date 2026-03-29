#include "defines.hlsl"
#include "functions.hlsl"
#include "structs.hlsl"

//Texture2D shaderTexture[4];
Texture2D TextureAtlas : register(t0);

SamplerState diffSampler : register(s0);


struct Light
{
	uint range;
	uint lightIntensity;
	uint x;
	uint y;
	uint z;
};
StructuredBuffer<Light> lights : register(t8);

struct VOXEL_OUTPUT
{
    float4 position : SV_POSITION;
    float3 localPos : LOCALPOSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
    float visible : VISIBLE;
	//uint4 lightIndices : LIGHTINDICES;
	//float lightIntensity : LIGHTINTENSITY;
	uint tileIndex : TILEINDEX;
	uint numLights : LIGHTNUM;
};

float GetLightStrength(VOXEL_OUTPUT input, float falloff)
{
	float lightIntensity = 0;
	for(uint i = 0; i < input.numLights; i++)
	{
		Light light = lights[input.tileIndex * 4 + i];
		float xPos = input.localPos.x;
		float yPos = input.localPos.z;
		//float distance = sqrt(pow(light.x - xPos, 2) + pow(light.y - yPos, 2));
		float dist = distance(float2(light.x, light.y), float2(xPos, yPos));
		float s = (dist / light.range);
		s *= s;
		if (s >= 1.0)
			continue;
		
		float s2 = sqrt(s);
		lightIntensity += light.lightIntensity * sqrt(1.0 - s2) / (1.0 + falloff * s2);
	}
	return lightIntensity;
	//return 1.0 / ((dist * dist) / max((light.lightIntensity / 3.0), 0.01));
}


//original
//float GetLightStrength(VOXEL_OUTPUT input, uint index)
//{
//	if (index == 0xFFFFFFFF)
//	{
//		return 0.0f;
//	}
//
//	Light light = lights[index];
//	float xPos = input.localPos.x;
//	float yPos = input.localPos.z;
//    //float distance = sqrt(pow(light.x - xPos, 2) + pow(light.y - yPos, 2));
//	float dist = max(distance(float2(light.x, light.y), float2(xPos, yPos)), 0.01);
//	
//	return 1.0 / ((dist * dist) / max((light.lightIntensity / 3.0), 0.01));
//}


float4 PShader(VOXEL_OUTPUT input) : SV_TARGET
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
   // return float4(face.xyz * input.visible * input.lightIntensity, 1.0);
    
	
	float lightIntensity = 0.6;

	float falloff = 27.0;
	
	lightIntensity += GetLightStrength(input, falloff);
	lightIntensity = clamp(lightIntensity, 0.5f, 1.0f);
	
	
    return float4(face.xyz * lightIntensity * input.visible, 1.0);
   // return float4(input.uv.x*3.0, input.uv.y*3.0, 0.5, 1);
}