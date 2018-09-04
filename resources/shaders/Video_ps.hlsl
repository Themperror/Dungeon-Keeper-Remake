#include "Defines.hlsl"

Texture2D video : register(t0);
SamplerState videoSampler : register(s1);

cbuffer ConstantBuffer : register(b2)
{
    float _screenWidth;
    float _screenHeight;
    float _visualType;
    float _SkyboxLerp;

    float _shadowType;
    float _globalRoughness;
    float _globalMetallic;
    float _MSAAValue;

    float _time;
    float _F0x;
    float _F0y;
    float _F0z;

    float _numCascades;
    float _shadow_atlas_size;
    float _kernel_size;
    float _d7;
};
struct VS_OUTPUT
{
    float4 positionVS : SV_POSITION;
    float2 uv : UV;
};
struct PS_OUTPUT
{
    float4 col : SV_Target;
    //float depth : SV_Depth;
};


PS_OUTPUT PShader(VS_OUTPUT input)
{
    PS_OUTPUT output;
    input.uv = float2(input.uv.x, 1.0 - input.uv.y);
    //output.col = BlurFunction3x3(input); // video.Sample(videoSampler, float2(input.uv.x, 1.0 - input.uv.y));
    //output.col = video.Sample(videoSampler, float2(input.uv.x, 1.0 - input.uv.y));
    int width, height;
    video.GetDimensions(width, height);

    output.col = video.Load(int3(input.uv.x * width, input.uv.y * height, 0));
    //output.depth = 1.0;
    return output;
}