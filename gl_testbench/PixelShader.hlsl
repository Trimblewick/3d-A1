Texture2D t1 : register(t0);
SamplerState s1 : register(s0);



struct VS_OUT
{
	float4 pos : SV_POSITION;
	float2 uv : TEXCOORD;
};

float4 main(VS_OUT inputblargh) : SV_TARGET
{

	return t1.Sample(s1, inputblargh.uv); //tex.Sample(s, input.uv);//
}
