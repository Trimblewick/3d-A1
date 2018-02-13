Texture2D t1 : register(t0);
SamplerState s1 : register(s0);

cbuffer colorBuffer : register(b0)
{
	float4 color;
}

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 nor : NORMAL;
	float2 uv : TEXCOORD;
};

float4 main(VS_OUT input) : SV_TARGET
{
	if(color.w > 0.5)
		return color;
	else
		return t1.Sample(s1, input.uv);
}
