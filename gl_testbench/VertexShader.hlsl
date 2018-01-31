





float4 main(uint id : SV_VertexID) : SV_POSITION//float4 pos : POSITION ) : SV_POSITION
{
    return float4((id == 2 || id == 3) * 2 - 1, (id == 1 || id == 3) * 2 - 1, 0, 1);
	
}