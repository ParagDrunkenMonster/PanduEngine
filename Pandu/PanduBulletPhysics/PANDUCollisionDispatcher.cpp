#include "PANDUCollisionDispatcher.h"

namespace Pandu
{
	CollisionDispatcher::CollisionDispatcher(btCollisionConfiguration* collisionConfiguration)
		: btCollisionDispatcher(collisionConfiguration)
	{
	}

	CollisionDispatcher::~CollisionDispatcher()
	{
	}

	bool CollisionDispatcher::needsCollision(btCollisionObject* body0 ,btCollisionObject* body1)
	{
		return btCollisionDispatcher::needsCollision(body0,body1);
	}

	bool CollisionDispatcher::needsResponse(btCollisionObject* body0 ,btCollisionObject* body1)
	{
		return btCollisionDispatcher::needsResponse(body0,body1);
	}
}