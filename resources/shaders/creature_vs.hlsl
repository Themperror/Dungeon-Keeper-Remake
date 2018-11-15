#include "defines.hlsl"
#include "structs.hlsl"

VS_OUTPUT VShader(VS_INPUT input)
{
    VS_OUTPUT output;
    float4 pos = float4(input.position, 1.0);
	
    float4 mPos = float4(_modelMatrix._41_42_43_44);
    matrix T = mul(_modelMatrix, _invViewMatrix);
    T._41_42_43 = mPos.xyz;
    //T._21_23 = float2(0, 0);

    output.position = mul(pos, mul(T, mul(_viewMatrix, _projectionMatrix)));
    float u = 0;
	if(_isFlipped)
    {
        float uvx = 1.0 - input.uv.x;
        u = (1.0 / _SpriteWidth) * (_SpriteWidth / _NumAnim) * ((_AnimIndex + uvx));
    }
	else
    {
        u = (1.0 / _SpriteWidth) * (_SpriteWidth / _NumAnim) * (_AnimIndex + input.uv.x);
    }
    output.uv = float2(u, 1.0-input.uv.y);
    output.normal = input.normal;
    return output;
}