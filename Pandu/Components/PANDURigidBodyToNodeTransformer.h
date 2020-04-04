/********************************************************************
	filename: 	PANDURigidBodyToNodeTransformer
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDURigidBodyToNodeTransformer_h__
#define __PANDURigidBodyToNodeTransformer_h__

#include "PANDUTransformer.h"
#include "../PanduBulletPhysics/PANDUBulletTypes.h"
#include "../Graphics/PANDUNode.h"

namespace Pandu
{
	class RigidBodyToNodeTransformer : public Transformer
	{
	private:
		
		TBulletRigidBodySharedPtr m_RigidBody;
		TSharedNodePtr m_Node;

	public:
		
		RigidBodyToNodeTransformer(TBulletRigidBodySharedPtr _rb,TSharedNodePtr _node);
		virtual ~RigidBodyToNodeTransformer();

		virtual void Update(const FrameEvent& _frameEvent);
	};
}

#endif