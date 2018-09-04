struct GS_INPUT
{
    float4 pos : SV_POSITION;
	uint texID : TEX_ID0;
};
struct GSPS_OUTPUT
{
    float4 pos : SV_POSITION;
	float2 uv : UV0;
};

[maxvertexcount(4)]
void GShader( point  GS_INPUT input[1], inout TriangleStream<GSPS_OUTPUT> TriStream )
{
    
}