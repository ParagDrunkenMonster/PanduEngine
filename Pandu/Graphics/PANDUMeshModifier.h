/********************************************************************
	filename: 	PANDUMeshModifier
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUMeshModifier_h__
#define __PANDUMeshModifier_h__

#include "../boost/smart_ptr.hpp"
#include "PANDUVertexBuffer.h"
#include <list>

namespace Pandu
{
	class MeshModifier;
	
	typedef boost::shared_ptr<MeshModifier> TMeshModifierSharedPtr;
	typedef std::auto_ptr<MeshModifier> TMeshModifierAutoPtr;
	typedef std::list<TMeshModifierSharedPtr> TMeshModifiersSharedPtrList;

	class Mesh;
	
	class MeshModifier
	{
	private:

		Mesh* m_Mesh;

	public:

		friend class Mesh;

		MeshModifier(){}
		virtual ~MeshModifier(){}

	protected:

		virtual void ModifyMesh(const VertexBuffer& _vertexBuffer, VertexBuffer& _modifiedVertexBuffer) = 0;

		inline Mesh* GetMesh()	{	return m_Mesh;			}

	private:

		inline void SetMesh(Mesh* _mesh)
		{
			m_Mesh = _mesh;
			OnMeshSet();
		}

		virtual void OnMeshSet(){};

	};
}

#endif //__PANDUMeshModifier_h__