#include "PANDUMeshInstance.h"
#include "PANDUMaterial.h"

namespace Pandu
{
	MeshInstance::MeshInstance(TSharedMeshPtr _shrdMesh)
	: m_MeshPtr(_shrdMesh)
	{

	}

	MeshInstance::~MeshInstance()
	{
		m_MeshPtr.reset();
	}

	void MeshInstance::Render()
	{
		Material::defaultMaterial.ApplyMaterial();
		m_MeshPtr->Render();
	}
}