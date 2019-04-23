#include "CMeshCollider.h"
#include "CCollider.h"
#include "CModel.h"

CLASSDEFINITION(IComponent, CMeshCollider)

void CMeshCollider::Init()
{ 
	if (generateFromModel)
	{
		const CModel *model = GetOwner().GetComponent<CModel>();

		if (model == nullptr)
		{
			Logger::Print("MeshCollider::Can't generate mesh collider from attached CModel component. Also, CModel component must be declared before CMeshCollider");
		}

		const StaticArray<MeshResource*> &meshes = model->GetMeshes();

		// get transforms in global space
		const StaticArray<Matrix4> &transforms = model->GetTranforms();

		// for each mesh
		for (UINT i = 0; i < meshes.GetSize(); i++)
		{
			// add triangles to this mesh collider all trangles with transformation
			AddTriangles(meshes[i]->GetTriangles(), transforms[i]);
		}

		// first, recalculate bounding box
		RecalculateBoundingBox();
		// then sphere
		RecalculateBoundingSphere();
		// then create bounding parts
		RecalculateBoundingParts();
	}
}

void CMeshCollider::Update()
{ }

#define PROPERTY_KEY_GENERATE_FROM_MODEL "GenerateFromModel"

void CMeshCollider::SetProperty(const String & key, const String & value)
{
	if (key == PROPERTY_KEY_GENERATE_FROM_MODEL)
	{
		generateFromModel = value.ToBool();
	}
	else if (CCollider::SetProperty(this, key, value))
	{
		// if was set in CCollider
		return;
	}
}
