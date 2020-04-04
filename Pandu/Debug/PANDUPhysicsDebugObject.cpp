#include "PANDUPhysicsDebugObject.h"

namespace Pandu
{
	PhysicsDebugObject::PhysicsDebugObject(TDynamicsWorldWeakPtr _dynamicsWorld)
		: m_DynamicsWorld(_dynamicsWorld)
	{
		
	}

	PhysicsDebugObject::~PhysicsDebugObject()
	{
		m_DynamicsWorld.reset();
	}

	void PhysicsDebugObject::Render()
	{
		TDynamicsWorldSharedPtr dynamicsWorld = m_DynamicsWorld.lock();
		TSharedDebugDrawerPtr debugDrawer = m_DebugDrawer.lock();
		if( dynamicsWorld && !debugDrawer )
		{
			dynamicsWorld->GetBulletWorld()->setDebugDrawer(NULL);
		}

		if( dynamicsWorld )
		{
			dynamicsWorld->GetBulletWorld()->debugDrawWorld();
		}
	}

	void PhysicsDebugObject::DebugDrawerUpdated()
	{
		TSharedDebugDrawerPtr debugDrawer = m_DebugDrawer.lock();
		TDynamicsWorldSharedPtr dynamicsWorld = m_DynamicsWorld.lock();
		if( dynamicsWorld && debugDrawer )
		{
			dynamicsWorld->GetBulletWorld()->setDebugDrawer(debugDrawer.get());
		}
	}

	bool PhysicsDebugObject::CanDelete() const
	{
		TSharedDebugDrawerPtr debugDrawer = m_DebugDrawer.lock();
		if( debugDrawer == NULL )
		{
			return true;
		}

		return false;
	}
}