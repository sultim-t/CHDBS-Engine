#pragma once

#include <Engine/Math/Matrix.h>
#include <Engine/Rendering/Materials/StandardMaterial.h>
#include <Engine/ResourceManager/MeshResource.h>

class IModel
{
public:
	virtual const StaticArray<MeshResource*>		&GetMeshes()	const = 0;
	virtual const StaticArray<StandardMaterial*>	&GetMaterials()	const = 0;
	virtual const StaticArray<UINT>					&GetVAO()		const = 0;
	virtual const StaticArray<Matrix4>				&GetTranforms() const = 0;
	virtual const StaticArray<Matrix4>				&GetTranforms(const Matrix4 &global) const = 0;
};