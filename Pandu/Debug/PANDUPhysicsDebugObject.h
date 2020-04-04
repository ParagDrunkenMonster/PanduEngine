/********************************************************************
	filename: 	PANDUPhysicsDebugObject
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUPhysicsDebugObject_h__
#define __PANDUPhysicsDebugObject_h__

#include "PANDUDebugObject.h"
#include "../PanduBulletPhysics/PANDUDynamicsWorld.h"

namespace Pandu
{
	class PhysicsDebugObject : public DebugObject
	{
	private:

		TDynamicsWorldWeakPtr m_DynamicsWorld;
	
	public:
	
		PhysicsDebugObject(TDynamicsWorldWeakPtr _dynamicsWorld);
		~PhysicsDebugObject();

		virtual void Render();

	private:

		virtual void DebugDrawerUpdated();
		bool CanDelete() const;
		
	};
}

#endif //__PANDUPhysicsDebugObject_h__