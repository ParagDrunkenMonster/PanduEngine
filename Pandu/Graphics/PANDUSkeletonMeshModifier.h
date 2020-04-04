/********************************************************************
	filename: 	PANDUSkeletonMeshModifier
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUSkeletonMeshModifier_h__
#define __PANDUSkeletonMeshModifier_h__

#include "PANDUSkeletonManager.h"
#include "PANDUMeshModifier.h"

namespace Pandu
{
	class SkeletonMeshModifier : public MeshModifier
	{
	protected:

		TSharedSkeletonInstancePtr m_SkeletonInstance;
	
	public:
	
		SkeletonMeshModifier();
		virtual ~SkeletonMeshModifier();

		inline TSharedSkeletonInstancePtr GetSkeletonInstance()		{		return m_SkeletonInstance;			}

	protected:

		virtual void OnMeshSet();
		virtual void ModifyMesh(const VertexBuffer& _vertexBuffer, VertexBuffer& _modifiedVertexBuffer);
	};
}

#endif //__PANDUSkeletonMeshModifier_h__