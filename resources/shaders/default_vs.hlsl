#include "Defines.hlsl"
#include "Structs.hlsl"

VS_OUTPUT VShader(VS_INPUT input)
{
    VS_OUTPUT output;
    float4 pos = float4(input.position, 1.0f);
    output.positionWS = mul(pos, modelMatrix).xyz;
    pos = mul(pos, mul(modelMatrix, mul(viewMatrix, projectionMatrix)));
	//pos = mul(pos, projectionMatrix);
    

    output.tangentWS = mul(input.tangent, (float3x3) modelMatrix);
    output.bitangentWS = mul(input.bitangent, (float3x3) modelMatrix);
    output.normalWS = mul(input.normal, (float3x3) modelMatrix);

    output.uv = input.uv;
    output.positionVS = pos;

    return output;
}