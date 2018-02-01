

float4 main(uint id : SV_VertexID) : SV_POSITION//float4 pos : POSITION ) : SV_POSITION
{
	float4 pos = float4(0, 0, 0, 1);

	if (id == 0)
	{
		pos = float4(-0.5f, -0.5f, 0, 1);
	}
	if (id == 1)
	{
		pos = float4(0.0f, 0.5f, 0, 1);
	}
	if (id == 2)
	{
		pos = float4(0.5f, -0.5f, 0, 1);
	}

	return pos;//float4((id == 2 || id == 3) * 2 - 1, (id == 1 || id == 3) * 2 - 1, 0, 1);
}