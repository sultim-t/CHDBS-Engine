#pragma once

struct VertexWeight
{
	UINT VertexID;
	float Weight;

	inline VertexWeight();
	inline VertexWeight(UINT vertId, float weight);
};

inline VertexWeight::VertexWeight() 
	: VertexID(0), Weight(0.0f)
{ }

inline VertexWeight::VertexWeight(UINT vertId, float weight) 
	: VertexID(vertId), Weight(weight)
{ }
