#pragma pack_matrix( row_major )
#include "defines.hlsl"

//note some of these helper functions were not completely written by me, or otherwise converted from another source.

float3 ExpandNormal(float3 n)
{
	return n * 2.0f - 1.0f;
};

float Remap(float x, float input_min, float input_max, float output_min, float output_max)
{
	float n = (x - output_min) / (output_max - output_min);
	return input_min + ((input_max - input_min)*n);
}
float3 Remap(float3 x, float3 input_min, float3 input_max, float3 output_min, float3 output_max)
{
	return float3(
		Remap(x.x, input_min.x, input_max.x, output_min.x, output_max.x),
		Remap(x.y, input_min.y, input_max.y, output_min.y, output_max.y),
		Remap(x.z, input_min.z, input_max.z, output_min.z, output_max.z)
		);
}
