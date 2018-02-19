StructuredBuffer <float4> pos : register(t1);
StructuredBuffer <float4> nor : register(t2);
StructuredBuffer <float2> uv : register(t3);


cbuffer tBuffer : register(b4)
{
	float4 translation;
};

struct VS_OUT
{
	float4 pos : SV_POSITION;
	float4 nor : NORMAL;
	float2 uv : TEXCOORD;
};

VS_OUT main(uint id : SV_VertexID)
{
	VS_OUT output;
	output.pos = pos[id] + translation;// float4(translation.x, translation.y, 1, 1);
	output.nor = nor[id];
	output.uv = uv[id];

	return output;
}
