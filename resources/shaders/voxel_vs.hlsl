#include "defines.hlsl"

struct VS_OUTPUT
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL;
	float2 uv : UV;
	float visible : VISIBLE;
    float lightIntensity : LIGHTINTENSITY;
};
struct VS_INPUT
{
	float3 position : POSITION;
	float3 normal : NORMAL;
	float2 uv : UV;
	float visible : VISIBLE;
	uint doAnimate : ANIMATE;
	uint4 lightIndices : LIGHTINDICES;
};
cbuffer ObjectBuffer : register(b0)
{
	float4x4 _modelMatrix; //64
};
cbuffer CameraBuffer : register(b1)
{
	float4x4 _viewMatrix;
	float4x4 _projectionMatrix;
	float4x4 _invProjectionMatrix;
	float4x4 _invViewMatrix;
	float4 _cameraPosition;
	float4 _cameraDir;
};
cbuffer ConstantBuffer : register(b2)
{
	float _screenWidth;
	float _screenHeight;
	float _visualType;
	float _time;
};

struct Light
{
	uint range;
	uint lightIntensity;
	uint x;
	uint y;
	uint z;
	uint lightIndex;
};
StructuredBuffer<Light> lights : register(t8);

float GetLightStrength(VS_INPUT input, uint index)
{
	if(index == -1)
    {
        return 0.0f;
    }

    Light light = lights[index];
    float xPos = input.position.x;
    float yPos = input.position.z;
    //float distance = sqrt(pow(light.x - xPos, 2) + pow(light.y - yPos, 2));
    float dist = distance(float2(light.x, light.y), float2(xPos, yPos));
    return 1.0 / ((dist * dist) / (light.lightIntensity / 3.0));
}

VS_OUTPUT VShader(VS_INPUT input)
{
	VS_OUTPUT output;
	float4 pos = float4(input.position, 1.0);

	if (input.doAnimate > 0)
	{
		pos.y += sin(_time*2.0 + pos.x * 1.33) * 0.25;
	}

	output.position = mul(pos, mul(_modelMatrix, mul(_viewMatrix, _projectionMatrix)));
	output.uv = input.uv;
	output.normal = input.normal;
	output.visible = input.visible;
	
    output.lightIntensity = 0.0;

    output.lightIntensity += GetLightStrength(input,input.lightIndices.x);
    output.lightIntensity += GetLightStrength(input,input.lightIndices.y);
    output.lightIntensity += GetLightStrength(input,input.lightIndices.z);
    output.lightIntensity += GetLightStrength(input,input.lightIndices.w);

    output.lightIntensity = clamp(output.lightIntensity, 0.3f, 1.0f);
	return output;
}


