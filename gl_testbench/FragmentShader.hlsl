Texture2D t1 : register(t0);
SamplerState s1 : register(s0);

cbuffer colorBuffer : register(b0)
{
	float3 color;
}

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

float4 main(VS_OUT input) : SV_TARGET
{
	return float4(color,1);//t1.Sample(s1, input.uv);
}
