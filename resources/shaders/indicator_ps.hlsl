#include "defines.hlsl"
#include "functions.hlsl"

cbuffer ConstantBuffer : register(b2)
{
    float _screenWidth;
    float _screenHeight;
    float _visualType;
    float _time;
};
cbuffer ObjectBuffer : register(b0)
{
    matrix modelmatrix;
    int _misc0, _misc1, _misc2, _misc3;
}
struct VS_OUTPUT
{
    float4 position : SV_POSITION;
    float4 localPos : POSITION;
    float3 normal : NORMAL;
    float2 uv : UV;
};
float4 PShader(VS_OUTPUT input) : SV_TARGET
{
    float4 lineColor = float4(0, 1, 0, 1);
    if (_misc0 > 0)
    {
        lineColor = float4(1, 0, 0, 1);
    }
    float4 spaceColor = float4(0, 0, 0, 0);

    float4 outColor = spaceColor;
    float range = 5.0;
    float spacing = 40;
    float sinVal = sin(_time * range + input.localPos.x * spacing);
    float sinYVal = sin(_time * range + input.localPos.y * spacing);
    float cosVal = cos(_time * range + input.localPos.z * spacing);
	
    float thickness = 0.02;
	//constant
    float vertexPos = 0.5;

    if (input.localPos.z < -vertexPos + thickness)
    {
        if (input.localPos.y < -vertexPos + thickness)
        {
            outColor = lerp(lineColor, spaceColor, saturate(sinVal));
        }
        else if (input.localPos.y > vertexPos - thickness)
        {
            outColor = lerp(lineColor, spaceColor, saturate(sinVal));
        }
    }
    else if (input.localPos.z > vertexPos - thickness)
    {
        if (input.localPos.y < -vertexPos + thickness)
        {
            outColor = lerp(lineColor, spaceColor, saturate(sinVal));
        }
        else if (input.localPos.y > vertexPos - thickness)
        {
            outColor = lerp(lineColor, spaceColor, saturate(sinVal));
        }
    }
    if (input.localPos.x < -vertexPos + thickness)
    {
        if (input.localPos.y < -vertexPos + thickness)
        {
            outColor = lerp(lineColor, spaceColor, saturate(cosVal));
        }
        else if (input.localPos.y > vertexPos - thickness)
        {
            outColor = lerp(lineColor, spaceColor, saturate(cosVal));
        }
        if (input.localPos.z < -vertexPos + thickness)
        {
            outColor = lerp(lineColor, spaceColor, saturate(sinYVal));
        }
        else if (input.localPos.z > vertexPos - thickness)
        {
            outColor = lerp(lineColor, spaceColor, saturate(sinYVal));
        }
    }
    else if (input.localPos.x > vertexPos - thickness)
    {
        if (input.localPos.y < -vertexPos + thickness)
        {
            outColor = lerp(lineColor, spaceColor, saturate(cosVal));
        }
        else if (input.localPos.y > vertexPos - thickness)
        {
            outColor = lerp(lineColor, spaceColor, saturate(cosVal));
        }
		
        if (input.localPos.z < -vertexPos + thickness)
        {
            outColor = lerp(lineColor, spaceColor, saturate(sinYVal));
        }
        else if (input.localPos.z > vertexPos - thickness)
        {
            outColor = lerp(lineColor, spaceColor, saturate(sinYVal));
        }
    }
    if (outColor.a <= 0.01f)
    {
        discard;
    }
    return outColor;
}