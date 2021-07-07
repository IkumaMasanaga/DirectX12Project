cbuffer VSMatrix: register(b0) {
	float4x4 WorldViewProj;
	float4x4 World;
};

// VSInput structure
struct VSInput {
	float3 Position:	POSITION;
	float3 Normal:		NORMAL;
	float2 Texture:     TEXCOORD0;
};

struct PSInput {
	float4	Position	: SV_POSITION;
	float3  Normal		: NORMAL;
	float2	Texture		: TEXCOORD0;
};


Texture2D txDiffuse : register(t0);
SamplerState samLinear : register(s0);



//PSInput VSMain(float4 position : POSITION, float2 tex : TEXCOORD0)
PSInput VSMain(VSInput input)
{
	PSInput	result = (PSInput)0 ;
	result.Position = mul( float4( input.Position, 1.0f ), WorldViewProj) ;
	//result.Position = mul(float4(input.Position, 1.0f), WorldViewProj);
	//result.Normal = normalize(mul(input.Normal, (float3x3)World));
	result.Texture = input.Texture;

	//result.Position = position;
	//result.color = color;

	return result;
}

float4 PSMain(PSInput input) : SV_TARGET
{
	return txDiffuse.Sample(samLinear, input.Texture);
	//return float4(1, 1, 1, 1);
}
