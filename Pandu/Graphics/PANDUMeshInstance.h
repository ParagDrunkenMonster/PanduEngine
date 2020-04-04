/********************************************************************
	filename: 	PANDUMeshInstance
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUMeshInstance_h__
#define __PANDUMeshInstance_h__

#include "PANDURenderable.h"
#include <list>
#include "PANDUMeshModifier.h"
#include "PANDUMaterial.h"

namespace Pandu
{
	class MeshInstance;

	typedef boost::shared_ptr<MeshInstance> TSharedMeshInstancePtr;
	typedef std::list<TSharedMeshInstancePtr> TMeshInstancesList;

	class MeshInstance : public Renderable
	{
	protected:

		TSharedMaterialPtr m_Material;

		inline MeshInstance(){}
		virtual ~MeshInstance()
		{
			m_Material.reset();
		}

	public:

		virtual void AddModifier(TMeshModifierAutoPtr _modifier) = 0;
		virtual void Render(const Matrix44& _transform, unsigned int _techniqueID, bool _useFixedFunctionIfNoTechniqueAvailable) = 0;

		inline void SetMaterial(TSharedMaterialPtr _material)		{	m_Material = _material;		}
	};
}

#endif