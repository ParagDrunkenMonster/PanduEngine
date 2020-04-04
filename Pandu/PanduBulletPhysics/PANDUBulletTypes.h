/********************************************************************
	filename: 	PANDUBulletTypes
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUBulletTypes_h__
#define __PANDUBulletTypes_h__

#include "../boost/smart_ptr.hpp"
#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"
#include "BulletDynamics/Character/btKinematicCharacterController.h"
#include "BulletCollision/CollisionDispatch/btGhostObject.h"

namespace Pandu
{
	typedef boost::shared_ptr<btDynamicsWorld> TBulletDynamicsWorldSharedPtr;
	typedef boost::shared_ptr<btDefaultCollisionConfiguration> TBulletCollisionConfigurationSharedPtr;
	typedef boost::shared_ptr<btBroadphaseInterface> TBulletBroadPhaseSharedPtr;
	typedef boost::shared_ptr<btCollisionDispatcher> TBulletCollisionDispatcherSharedPtr;
	typedef boost::shared_ptr<btConstraintSolver> TBulletConstraintSolverSharedPtr;
	typedef boost::shared_ptr<btCollisionShape> TBulletCollisionShapeSharedPtr;
	typedef std::auto_ptr<btCollisionShape>	TBulletCollisionShapeAutoPtr;
	typedef boost::shared_ptr<btRigidBody> TBulletRigidBodySharedPtr;
	typedef std::auto_ptr<btRigidBody> TBulletRigidBodyAutoPtr;
	typedef boost::shared_ptr<btCollisionObject> TCollisionObjectSharedPtr;
	typedef boost::weak_ptr<btCollisionObject> TCollisionObjectWeakPtr;

	typedef boost::shared_ptr<btGhostPairCallback> TGhostPairCallBackSharedPtr;

	typedef boost::shared_ptr<btKinematicCharacterController> TBulletKinematicCharControllerSharedPtr;

	enum WorldType
	{
		WT_DiscreteDynamicWorld = 0,
		WT_SoftBodyDynamicsWorld,
		WT_FluidRigidDynamicsWorld,

		WT_Count
	};

	enum CollisionShape
	{
		CS_None = -1,
		CS_Box = 0,
		CS_Shpere,
		CS_Capsule,

		CS_Count
	};

	struct CollisionShapeDesc
	{
		CollisionShapeDesc()
			: type(CS_Box)
			, shape(NULL)
			, width(1.0f)
			, height(1.0f)
			, depth(1.0f)
			, radius(1.0f)
		{
		}

		CollisionShape type;
		btCollisionShape *shape;
		float width;
		float height;
		float depth;
		float radius;
	};

	struct RigidBodyDesc
	{
		RigidBodyDesc()
			: localInertia(btVector3(0.0f,0.0f,0.0f))
			, mass(0.0f)//by default object is static
		{
			transform.setIdentity();
		}

		CollisionShapeDesc collisionShape;

		btTransform transform;
		btVector3 localInertia;
		float mass;  //if mass is zero object is static;
	};

	struct TriggerDesc
	{
		TriggerDesc()
		{
			transform.setIdentity();
		}

		CollisionShapeDesc collisionShape;
		btTransform transform;
	};
}

#endif