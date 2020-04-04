/********************************************************************
	filename: 	PANDUBulletUtils
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUBulletUtils_h__
#define __PANDUBulletUtils_h__

#include "btBulletCollisionCommon.h"
#include "btBulletDynamicsCommon.h"

namespace Pandu
{
	static btRigidBody* CreateRigidBody(btDynamicsWorld* dynamicsWorld,float mass, const btTransform& startTransform,btCollisionShape* shape)
	{
		btAssert((!shape || shape->getShapeType() != INVALID_SHAPE_PROXYTYPE));

		//rigidbody is dynamic if and only if mass is non zero, otherwise static
		bool isDynamic = (mass != 0.0f);

		btVector3 localInertia(0,0,0);
		if (isDynamic)
		{
			shape->calculateLocalInertia(mass,localInertia);
		}

		btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);

		btRigidBody::btRigidBodyConstructionInfo cInfo(mass,myMotionState,shape,localInertia);

		btRigidBody* body = new btRigidBody(cInfo);
		body->setContactProcessingThreshold(BT_LARGE_FLOAT);

		dynamicsWorld->addRigidBody(body);

		return body;
	}
}

#endif