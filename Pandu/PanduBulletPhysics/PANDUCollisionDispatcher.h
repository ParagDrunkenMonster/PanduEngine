/********************************************************************
	filename: 	PANDUCollisionDispatcher
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUCollisionDispatcher_h__
#define __PANDUCollisionDispatcher_h__

#include "PANDUBulletTypes.h"

namespace Pandu
{
	class CollisionDispatcher : public btCollisionDispatcher
	{
	public:

		CollisionDispatcher(btCollisionConfiguration* collisionConfiguration);
		virtual ~CollisionDispatcher();

		virtual bool needsCollision(btCollisionObject* body0 ,btCollisionObject* body1);
		virtual bool needsResponse(btCollisionObject* body0 ,btCollisionObject* body1);
	};
}

#endif