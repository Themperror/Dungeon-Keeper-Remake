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

float4 ClipToView(float4x4 invProjectionMatrix, float4 clip)
{
	// View space position.
	float4 view = mul(invProjectionMatrix, clip);
	// Perspective projection.
	view = view / view.w;

	return view;
}

// Convert screen space coordinates to view space.
float4 ScreenToView(float screenWidth,float screenHeight, float4 screen, float4x4 invProjectionMatrix)
{
	// Convert to normalized texture coordinates
    float2 texCoord = screen.xy / float2(screenWidth, screenHeight);

	// Convert to clip space
	float4 clip = float4(float2(texCoord.x, 1.0f - texCoord.y) * 2.0f - 1.0f, screen.z, screen.w);

    return ClipToView(invProjectionMatrix, clip);
}
float4 WorldToScreen(float3 worldPos, float4x4 viewMatrix, float4x4 projectionMatrix)
{
    float4 pos = float4(worldPos, 1.0f);
    pos = mul(pos, viewMatrix);
    pos = mul(pos, projectionMatrix);

    return pos;
}


float3 GammaCorrect(float3 col,float scale)
{
    col = col / (col + float3(1.0, 1.0, 1.0));
   // col = pow(col, float3(0.454545455f, 0.454545455f, 0.454545455f));
    col = pow(abs(col), float3(scale, scale, scale));
    return col;
}

//not written by me
// Hammersley function (return random low-discrepency points)
float2 Hammersley(uint i, uint N)
{
    return float2(
    float(i) / float(N),
    float(reversebits(i)) * 2.3283064365386963e-10);
}

//not written by me
// Random rotation based on the current pixel coord (Only use with Fullscreen quad!)
float randAngle(uint2 texCoord)
{
    uint x = texCoord.x;
    uint y = texCoord.y;
    return float(30u * x ^ y + 10u * x * y);
}
//not written by me
// Helper function (make skewed point into vec3 direction)
float3 MakeSample(float Theta, float Phi, uint2 texCoord)
{
    Phi += randAngle(texCoord);
    float SineTheta = sin(Theta);

    float x = cos(Phi) * SineTheta;
    float y = sin(Phi) * SineTheta;
    float z = cos(Theta);

    return float3(x, y, z);
}

//not written by me
//k = roughness
float DGgx(float NoH, float k)
{
  // Note: Generally sin2 + cos2 = 1
  // Also: Dgtr = c / (a * cos2 + sin2)
  // So...
    float Krough2 = k * k;
    float NoH2 = NoH * NoH;
    float denom = 1.0 + NoH2 * (Krough2 - 1.0);
    return Krough2 / (PI * denom * denom);
}

//not written by me
// Computes the exact mip-map to reference for the specular contribution.
// Accessing the proper mip-map allows us to approximate the integral for this
// angle of incidence on the current object.
//k = roughness
float compute_lod(uint NumSamples, float width, float height, float NoH, float k)
{
    float dist = DGgx(NoH, k); // Defined elsewhere as subroutine
    return 0.5 * (log2(float(width * height) / NumSamples) - log2(dist));
}


//not written by me
//GGxSample (S)
//E = output from Hammersley()
//k = roughness
float3 S(float2 E, float k, uint2 texCoord)
{
    float a = k * k;
    float Theta = atan(sqrt((a * E.x) / (1.0 - E.x)));
    float Phi = PI2 * E.y;
    return MakeSample(Theta, Phi, texCoord);
}
float FSchlick(float VoH, float metallic)
{
    float Kmetallic = metallic;
    return Kmetallic + (1.0 - Kmetallic) * pow(1.0 - VoH, 5.0);
}

//k = roughness
float GSmithSchlickBeckmann_(float NoV, float k)
{
    float r = k * k * 2.50662827f; // the constant is sqrt(PI2)
    return NoV / (NoV * (1.0 - r) + r);
}

float GSmithSchlickBeckmann(float NoL, float NoV, float k)
{
    return GSmithSchlickBeckmann_(NoL, k) * GSmithSchlickBeckmann_(NoV, k);
}
//not written by me
float2 SphereMap(float3 N)
{
    return float2(0.5 - atan2(N.y, N.x) / PI2, acos(N.z) / PI);
}
float4 textureSphereLod(TextureCube tex,SamplerState s, float3 N, float lod)
{
    return tex.SampleLevel(s, N, lod);
}
float3 BlendDielectric(float3 Kdiff, float3 Kspec, float3 Kbase)
{
    return Kdiff + Kspec;
}

float3 BlendMetal(float3 Kdiff, float3 Kspec, float3 Kbase)
{
    return Kspec * Kbase;
}

float3 BlendMaterialStepped(float3 albedo, float3 Kdiff, float3 Kspec, float metallic)
{
    float3 Kbase = albedo;
    float scRange = smoothstep(0.2, 0.45, metallic);
    float3 dielectric = BlendDielectric(Kdiff, Kspec, Kbase);
    float3 metal = BlendMetal(Kdiff, Kspec, Kbase);
    return lerp(dielectric, metal, scRange); 
}
float3 BlendMaterial(float3 albedo, float3 Kdiff, float3 Kspec, float metallic)
{
    float3 Kbase = albedo;
    float scRange = metallic;
    float3 dielectric = BlendDielectric(Kdiff, Kspec, Kbase);
    float3 metal = BlendMetal(Kdiff, Kspec, Kbase);
    return lerp(dielectric, metal, scRange);
}
float GeometryGGXSchlick(float val, float roughness)
{
    float nom = val;
    float denom = val * (1.0 - roughness) + roughness;
	
    return nom / denom;
}
float GeometryGGX(float maxNdotV, float maxNdotL, float roughness)
{
    float ggx2 = GeometryGGXSchlick(maxNdotV, roughness);
    float ggx1 = GeometryGGXSchlick(maxNdotL, roughness);
	
    return ggx1 * ggx2;
}


// GGX/Throwbridge-Reitz
float DistributionGGX(float3 N, float3 H, float roughness)
{
    float a = roughness * roughness;
    float a2 = a * a;
    float NdotH = max(dot(N, H),0);
    float NdotH2 = NdotH * NdotH;
	
    float nom = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return nom / denom;
}

float3 FresnelSchlick(float cosTheta, float3 F0)
{
    return F0 + (float3(1.0, 1.0, 1.0) - F0) * pow(1.0 - cosTheta, 5.0);
}

// ----------------------------------------------------------------------------
float3 FresnelSphericalGaussian(float3 F0, float cosTheta)
{
    return F0 + (float3(1.0, 1.0, 1.0) - F0) * pow(2.0, (-5.55473 * cosTheta - 6.98316 * cosTheta));
}



//not entirely written by me
//adjusted several parts
float3 SpecBRDF(float3 normal, float3 V, float2 texCoord, float roughness, float metallic, float3 F0v, TextureCube SkyTex1, TextureCube SkyTex2, SamplerState samplerType, float skyboxLerp, float f0x)
{
    float3 UpVector = abs(normal.z) < 0.9999 ? float3(0, 0, 1) : float3(1, 0, 0);
    float3 TangentX = normalize(cross(UpVector, normal));
    float3 TangentY = normalize(cross(normal, TangentX));
	
    uint texWidth = 0;
    uint texHeight = 0;
    SkyTex1.GetDimensions(texWidth, texHeight);
	
    float NdotV = max(dot(normal, V), 0.0);

    const uint numSamples = 50;
    float3 sum = float3(0, 0, 0);
    for (uint i = 0; i < numSamples; ++i)
    {
        float2 Xi = Hammersley(i, numSamples);
        float3 Li = S(Xi, roughness, texCoord);
        float3 H = normalize(Li.x * TangentX + Li.y * TangentY + Li.z * normal);
        float3 L = normalize(-reflect(V, H));
		
        float NdotL = max(dot(normal, L), 0.0);
        float NdotH = max(dot(normal, H), 0.0);
        float VdotH = max(dot(V, H), 0.0);

        float lod = compute_lod(numSamples, texWidth, texHeight, NdotH, roughness);

        float3 F_ = FresnelSchlick(NdotH, F0v);
        float G_ = GeometryGGX(NdotV, NdotL, roughness);
        float3 LColor = GammaCorrect(lerp(textureSphereLod(SkyTex1, samplerType, L, lod).rgb, textureSphereLod(SkyTex2, samplerType, L, lod).rgb, skyboxLerp), f0x);
       
        sum += F_ * G_ * LColor * VdotH / (NdotH * NdotV + 0.001);
    }
    return sum / (float) numSamples;
}
float3 BRDF(float3 albedo, float3 F0v, float3 V, float3 normal, float3 lightDir, float4 lightCol, float roughness, float metallic , float visualType, float f0z)
{
    float3 H = normalize(lightDir + V); //L V
    float NdotL = max(dot(normal, lightDir), 0.0); // L N cosTheta
   // cosTheta = max(cosTheta, 0.0);
    float NdotV = max(dot(normal, V), 0); // N V
   // float NdotL = dot(normal, lightDir); // N L cosTheta

    float D = DistributionGGX(normal, H, roughness);
    float3 F = FresnelSchlick(max(dot(normal, H), 0.0), F0v);
    float G = GeometryGGX(NdotV, NdotL, roughness); //    GSmithSchlickBeckmann(NdotL, NdotV, max(dot(normal, H), 0.0), 0, roughness);

    float3 nominator = D * G * F;
    float denominator = 4 * NdotV * NdotL;
    float3 specular = nominator / max(denominator, 0.001);
        
	//direct diffuse part:
    float3 kS = F;
    float3 kD = float3(1.0, 1.0, 1.0) - kS;
    kD *= 1.0 - metallic;
	
	// add to outgoing radiance Lo
    float3 Lo = (kD * albedo / PI + specular) * (lightCol.xyz * f0z * lightCol.w) * NdotL;
    return Lo;
}
float3 DebugBRDF(float3 albedo, float3 F0v, float3 V, float3 normal, float3 lightDir, float4 lightCol, float roughness, float metallic, float visualType, float f0z)
{
    float3 H = normalize(lightDir + V); //L V
    float NdotL = max(dot(lightDir, normal), 0.0); // L N cosTheta
   // cosTheta = max(cosTheta, 0.0);
    float NdotV = max(dot(normal, V), 0); // N V
   // float NdotL = dot(normal, lightDir); // N L cosTheta

    float D = DistributionGGX(normal, H, roughness);
    if (visualType == 1.0)
    {
        return float3(D, D, D);
    }

    float3 F = FresnelSchlick(max(dot(normal, H), 0.0), F0v);
    if (visualType == 2.0)
    {
        return F;
    }

    float G = GeometryGGX(NdotV, NdotL, roughness); //    GSmithSchlickBeckmann(NdotL, NdotV, max(dot(normal, H), 0.0), 0, roughness);
    if (visualType == 3.0)
    {
        return float3(G, G, G);
    }
   
    float3 nominator = D * G * F;
    float denominator = 4 * NdotV * NdotL;
    float3 specular = nominator / max(denominator, 0.001);
        
	//direct diffuse part:
    float3 kS = F;
    float3 kD = float3(1.0, 1.0, 1.0) - kS;
    kD *= 1.0 - metallic;

	// add to outgoing radiance Lo
    float3 Lo = (kD * albedo / PI + specular) * (lightCol.xyz * f0z * lightCol.w) * NdotL;

    return Lo;
}