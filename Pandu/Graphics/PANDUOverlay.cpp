#include "PANDUOverlay.h"

namespace Pandu
{
	Overlay::Overlay(const String& _name)
			: Renderable()
			, m_Name(_name)
			, m_EnablePreRenderUpdate(false)
			, m_ZOrder(0)
			, m_Visible(true)
			, m_OverlayMesh( new Mesh(_name + "_Mesh"))
			, m_OverlayMeshInstance(m_OverlayMesh->CreateMeshInstance())
	{
	}

	Overlay::~Overlay()
	{
		m_OverlayMesh.reset();
		m_OverlayMaterial.reset();
	}

	void Overlay::Render(const Matrix44& _transform, unsigned int _techniqueID, bool _useFixedFunctionIfNoTechniqueAvailable)
	{
		if(m_EnablePreRenderUpdate)
		{
			PreRenderUpdate();
		}
		
		if(m_Visible == true)
		{			
			m_OverlayMeshInstance->Render(_transform, _techniqueID,_useFixedFunctionIfNoTechniqueAvailable);
		}
	}

}