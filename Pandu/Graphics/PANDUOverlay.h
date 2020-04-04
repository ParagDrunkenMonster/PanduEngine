/********************************************************************
	filename: 	PANDUOverlay
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUOverlay_h__
#define __PANDUOverlay_h__

#include "PANDURenderable.h"
#include "PANDUMesh.h"
#include "PANDUMaterial.h"
#include "../boost/smart_ptr.hpp"

namespace Pandu
{
	class Overlay;

	typedef boost::shared_ptr<Overlay> TOverlaySharedPtr;
	typedef std::auto_ptr<Overlay> TOverlayAutoPtr;

	class Overlay : public Renderable
	{
	private:

		const String				m_Name;
		bool						m_EnablePreRenderUpdate;
		int							m_ZOrder;
		bool						m_Visible;

	protected:

		TSharedMaterialPtr			m_OverlayMaterial;
		TSharedMeshPtr				m_OverlayMesh;
		TSharedMeshInstancePtr		m_OverlayMeshInstance;

	public:

		Overlay(const String& _name);
		virtual ~Overlay();
	
		inline void SetMaterial(TSharedMaterialPtr _material)	
		{	
			m_OverlayMaterial = _material;		
			m_OverlayMeshInstance->SetMaterial(m_OverlayMaterial);
		}
		
		inline void SetZOrder(int _zOrder)						{	m_ZOrder = _zOrder;					}
	
		inline int GetZOrder()									{	return m_ZOrder;					}
		inline const String& GetName() const					{	return m_Name;						}
	
		inline void SetEnablePreRenderUpdate(bool _enable)		{	m_EnablePreRenderUpdate = _enable;	}
	
		void Render(const Matrix44& _transform, unsigned int _techniqueID, bool _useFixedFunctionIfNoTechniqueAvailable);

	private:

		virtual void PreRenderUpdate() = 0;

	};
}

#endif