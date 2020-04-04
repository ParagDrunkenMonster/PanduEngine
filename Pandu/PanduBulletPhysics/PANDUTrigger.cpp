#include "PANDUTrigger.h"


namespace Pandu
{
	Trigger::Trigger( const TriggerDesc& _desc)
		: m_GhostObject(NULL)
		, m_CollisionShape(NULL)
		, m_TriggerDesc(NULL)
	{
		m_TriggerDesc = new TriggerDesc(_desc);
	}

	Trigger::~Trigger()
	{
		Destroy();
	}

	void Trigger::Initialize(TBulletDynamicsWorldSharedPtr _world )
	{
		if(m_TriggerDesc)
		{
			const TriggerDesc& _desc = *m_TriggerDesc;

			m_BulletDynamicsWorld = _world;

			m_GhostObject = new btGhostObject();
			// As far as I know only the world aabb of the m_CollisionShape will be used (i.e. a box always parallel to the x,y,z axis of variable size)
			m_CollisionShape = new btBoxShape(btVector3(_desc.collisionShape.width,_desc.collisionShape.height,_desc.collisionShape.depth));
			m_GhostObject->setCollisionShape(m_CollisionShape);
			m_GhostObject->setCollisionFlags(btCollisionObject::CF_NO_CONTACT_RESPONSE);		// We can choose to make it "solid" if we want...
			m_BulletDynamicsWorld->addCollisionObject(m_GhostObject,btBroadphaseProxy::SensorTrigger,btBroadphaseProxy::AllFilter & ~btBroadphaseProxy::SensorTrigger);
			m_GhostObject->setWorldTransform(_desc.transform);

			delete m_TriggerDesc;
			m_TriggerDesc = NULL;
		}
	}

	void Trigger::Destroy()
	{
		if(m_GhostObject)
		{
			m_BulletDynamicsWorld->removeCollisionObject(m_GhostObject);
			delete m_GhostObject;
			m_GhostObject = NULL;
		}
	
		if(m_CollisionShape)
		{
			delete m_CollisionShape;
			m_CollisionShape = NULL;
		}

		if(m_TriggerDesc != NULL)
		{
			delete m_TriggerDesc;
			m_TriggerDesc = NULL;
		}
	}
}