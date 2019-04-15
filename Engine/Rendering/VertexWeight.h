#pragma once

//struct VertexWeight
//{
//	UINT VertexID;
//	float Weight;
//
//	inline VertexWeight();
//	inline VertexWeight(UINT vertId, float weight);
//};
//
//inline VertexWeight::VertexWeight() 
//	: VertexID(0), Weight(0.0f)
//{ }
//
//inline VertexWeight::VertexWeight(UINT vertId, float weight) 
//	: VertexID(vertId), Weight(weight)
//{ }

#define BONE_MAX_WEIGHTS 4

struct BoneWeight
{
	int		BoneIndex;
	float	Weight;
};

struct VertexWeight
{
	// All weights
	BoneWeight	Weights[BONE_MAX_WEIGHTS];
	// Current amount of weights
	int			WeightCount;

	VertexWeight() : WeightCount(0) { }

	// Add weight to this vertex
	// If there will be added a weight, but the array is full,
	// it will be ignored
	void AddWeight(int boneIndex, float weight)
	{
		// ASSERT(WeightCount >= 0 && WeightCount < BONE_MAX_WEIGHTS);

		if (WeightCount >= BONE_MAX_WEIGHTS)
		{
			for (int i = 0; i < BONE_MAX_WEIGHTS; i++)
			{
				if (Weights[i].Weight < weight)
				{
					Weights[i].BoneIndex = boneIndex;
					Weights[i].Weight = weight;
				}
			}

			return;
		}

		Weights[WeightCount].BoneIndex = boneIndex;
		Weights[WeightCount].Weight = weight;

		WeightCount++;
	}

	// Normalize weights to make their sum equal to 1.0f
	void Normalize()
	{
		if (WeightCount == 0)
		{
			return;
		}

		float sum = 0.0f;

		// get sum
		for (int i = 0; i < WeightCount; i++)
		{
			sum += Weights[i].Weight;
		}

		// normalize
		for (int i = 0; i < WeightCount; i++)
		{
			Weights[i].Weight /= sum;
		}
	}
};
