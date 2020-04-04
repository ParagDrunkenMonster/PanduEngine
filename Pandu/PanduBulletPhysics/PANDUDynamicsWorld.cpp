#include "PANDUDynamicsWorld.h"
#include "PANDUCollisionDispatcher.h"

namespace Pandu
{
	DynamicsWorld::DynamicsWorld(const String& _name, WorldType _worldType)
		: m_Name(_name)
		, m_WorldType(_worldType)
		, m_MaxSubSteps(3)
		, m_FixedTimeStep(1.0f/60.0f)
	{
		CreateWorld(_worldType);
	}

	DynamicsWorld::~DynamicsWorld()
	{
		DestroyWorld();
	}

	void DynamicsWorld::StepSimulation(float _timeStep )
	{
		m_BulletDynamicsWorld->stepSimulation(_timeStep,m_MaxSubSteps,m_FixedTimeStep);
		UpdateTriggers();
	}

	void DynamicsWorld::CreateWorld(WorldType _worldType)
	{
		if(m_BulletDynamicsWorld.use_count() == 0)
		{
			m_CollisionConfiguration.reset(new btDefaultCollisionConfiguration());
			m_Dispatcher.reset(new	CollisionDispatcher(m_CollisionConfiguration.get()));
			m_Broadphase.reset(new btDbvtBroadphase());
			m_Solver.reset(new btSequentialImpulseConstraintSolver());

			switch (_worldType)
			{
			case WT_DiscreteDynamicWorld:
				{
					m_BulletDynamicsWorld.reset(new btDiscreteDynamicsWorld(m_Dispatcher.get(),m_Broadphase.get(),m_Solver.get(),m_CollisionConfiguration.get()) );
				}
				break;

			case WT_SoftBodyDynamicsWorld:
				{
				}
				break;

			case WT_FluidRigidDynamicsWorld:
				{
				}
				break;
			}

			// NEW => btGhostPairCallback =================================
			m_GhostPairCallback.reset(new btGhostPairCallback()); 
			m_BulletDynamicsWorld->getBroadphase()->getOverlappingPairCache()->setInternalGhostPairCallback(m_GhostPairCallback.get());	// Needed once to enable ghost objects inside Bullet

		}
	}

	void DynamicsWorld::DestroyWorld()
	{
		PANDU_ERROR(m_BulletDynamicsWorld.use_count() == 1, "World is already being used, first clear all references" );
		m_BulletDynamicsWorld.reset();

		PANDU_ERROR(m_Broadphase.use_count() == 1, "Broadphase is already being used, first clear all references" );
		m_Broadphase.reset();

		PANDU_ERROR(m_Dispatcher.use_count() == 1, "Dispatcher is already being used, first clear all references" );
		m_Dispatcher.reset();

		PANDU_ERROR(m_Solver.use_count() == 1, "Solver is already being used, first clear all references" );
		m_Solver.reset();

		PANDU_ERROR(m_CollisionConfiguration.use_count() == 1, "CollisionConfiguration is already being used, first clear all references" );
		m_CollisionConfiguration.reset();

		PANDU_ERROR(m_GhostPairCallback.use_count() == 1, "GhostPairCallback is already being used, first clear all references" );
		m_GhostPairCallback.reset();

		ClearAllTriggers();
		ClearAllCollisionShapes();
		ClearAllCollisionBody();
	}


	TBulletCollisionShapeSharedPtr DynamicsWorld::CreateCollisionShape(const CollisionShapeDesc& _shapeType)
	{
		TBulletCollisionShapeSharedPtr newShape;

		switch(_shapeType.type)
		{
		case CS_None:
			{
				newShape.reset(_shapeType.shape);
			}
			break;

		case CS_Box:
			{
				newShape.reset(new btBoxShape(btVector3(btScalar(_shapeType.width/2.0f),btScalar(_shapeType.height/2.0f),btScalar(_shapeType.depth/2.0f)) ) );
			}
			break;

		case CS_Shpere:
			{
			}
			break;

		case CS_Capsule:
			{
				newShape.reset(new btCapsuleShape(_shapeType.radius, _shapeType.height) );
			}
			break;
		}

		if(newShape != NULL)
		{
			m_AllCollisionShapes.push_back(newShape);
		}

		return newShape;
	}

	void DynamicsWorld::DestroyCollisionShape(TBulletCollisionShapeAutoPtr _shape)
	{
		TCollisionShapeList::iterator itr = m_AllCollisionShapes.begin();
		TCollisionShapeList::iterator end = m_AllCollisionShapes.end();

		for( ; itr != end ; itr++ )
		{
			if( (*itr).get() == _shape.get() )
			{
				_shape.release();
				PANDU_ERROR((*itr).use_count() == 1, "Shape is already being used, first clear all references" );
				(*itr).reset();
				itr = m_AllCollisionShapes.erase(itr);
				break;
			}
		}
	}

	void DynamicsWorld::ClearAllCollisionShapes()
	{
		TCollisionShapeList::iterator itr = m_AllCollisionShapes.begin();
		TCollisionShapeList::iterator end = m_AllCollisionShapes.end();

		for( ; itr != end ; itr++ )
		{
			PANDU_ERROR((*itr).use_count() == 1, "Shape is already being used, first clear all references" );
			(*itr).reset();
		}

		m_AllCollisionShapes.clear();
	}

	TBulletRigidBodySharedPtr DynamicsWorld::CreateCollisionBody(const RigidBodyDesc& _desc)
	{
		TBulletRigidBodySharedPtr newBody;

		TBulletCollisionShapeSharedPtr shape = CreateCollisionShape(_desc.collisionShape);

		bool isDynamic = (_desc.mass != 0.f);
		btVector3 localInertia = _desc.localInertia;

		if(shape != NULL)
		{
			if (isDynamic)
				shape->calculateLocalInertia(_desc.mass,localInertia);
		}


		//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
		btDefaultMotionState* myMotionState = new btDefaultMotionState(_desc.transform);
		btRigidBody::btRigidBodyConstructionInfo rbInfo(_desc.mass,myMotionState,shape.get(),_desc.localInertia);
		
		newBody.reset(new btRigidBody(rbInfo));

		newBody->setActivationState(ISLAND_SLEEPING);

		m_BulletDynamicsWorld->addRigidBody(newBody.get());

		newBody->setActivationState(ISLAND_SLEEPING);

		m_AllBodies.push_back(newBody);

		return newBody;
	}

	void DynamicsWorld::DestroyCollisionBody(TBulletRigidBodyAutoPtr _body)
	{
		TRigidBodyList::iterator itr = m_AllBodies.begin();
		TRigidBodyList::iterator end = m_AllBodies.end();

		for( ; itr != end ; itr++ )
		{
			if( (*itr).get() == _body.get() )
			{
				_body.release();

				PANDU_ERROR((*itr).use_count() == 1, "Body is already being used, first clear all references" );

				TBulletCollisionShapeAutoPtr shape((*itr)->getCollisionShape());
				DestroyCollisionShape(shape);

				m_BulletDynamicsWorld->removeRigidBody((*itr).get());
				(*itr).reset();
				itr = m_AllBodies.erase(itr);
				break;
			}
		}
	}

	void DynamicsWorld::ClearAllCollisionBody()
	{
		TRigidBodyList::iterator itr = m_AllBodies.begin();
		TRigidBodyList::iterator end = m_AllBodies.end();

		for( ; itr != end ; itr++ )
		{
			PANDU_ERROR((*itr).use_count() == 1, "Shape is already being used, first clear all references" );
			(*itr).reset();
		}

		m_AllBodies.clear();

		ClearAllCollisionShapes();
	}

	void DynamicsWorld::SetGravity(const btVector3& _gravity)
	{
		m_BulletDynamicsWorld->setGravity(_gravity);
	}

	void DynamicsWorld::ClientResetScene()
	{
		int numObjects = 0;
		int i;

		if (m_BulletDynamicsWorld != NULL)
		{
			numObjects = m_BulletDynamicsWorld->getNumCollisionObjects();

			///create a copy of the array, not a reference!
			btCollisionObjectArray copyArray = m_BulletDynamicsWorld->getCollisionObjectArray();

		
			for (i=0;i<numObjects;i++)
			{
				btCollisionObject* colObj = copyArray[i];
				btRigidBody* body = btRigidBody::upcast(colObj);
				if (body)
				{
					if (body->getMotionState())
					{
						btDefaultMotionState* myMotionState = (btDefaultMotionState*)body->getMotionState();
						myMotionState->m_graphicsWorldTrans = myMotionState->m_startWorldTrans;
						body->setCenterOfMassTransform( myMotionState->m_graphicsWorldTrans );
						colObj->setInterpolationWorldTransform( myMotionState->m_startWorldTrans );
						colObj->forceActivationState(ACTIVE_TAG);
						colObj->activate();
						colObj->setDeactivationTime(0);
						//colObj->setActivationState(WANTS_DEACTIVATION);
					}
					//removed cached contact points (this is not necessary if all objects have been removed from the dynamics world)
					if (m_BulletDynamicsWorld->getBroadphase()->getOverlappingPairCache())
						m_BulletDynamicsWorld->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(colObj->getBroadphaseHandle(),m_BulletDynamicsWorld->getDispatcher());

					btRigidBody* body = btRigidBody::upcast(colObj);
					if (body && !body->isStaticObject())
					{
						btRigidBody::upcast(colObj)->setLinearVelocity(btVector3(0,0,0));
						btRigidBody::upcast(colObj)->setAngularVelocity(btVector3(0,0,0));
					}
				}

			}

			///reset some internal cached data in the broadphase
			m_BulletDynamicsWorld->getBroadphase()->resetPool(m_BulletDynamicsWorld->getDispatcher());
			m_BulletDynamicsWorld->getConstraintSolver()->reset();

		}
	}

	TTriggerWeakPtr DynamicsWorld::AddTrigger(TTriggerAutoPtr _trigger)
	{
		TTriggerSharedPtr strongTrigger(_trigger.release());

		strongTrigger->Initialize(m_BulletDynamicsWorld);
		m_AllTriggers.push_back(strongTrigger);

		return strongTrigger;
	}

	void DynamicsWorld::ClearAllTriggers()
	{
		TTriggerSharedPtrList::iterator itr = m_AllTriggers.begin();
		TTriggerSharedPtrList::iterator end = m_AllTriggers.end();

		for( ; itr != end ; itr++ )
		{
			PANDU_ERROR((*itr).use_count() == 1, "Trigger is already being used, first clear all references" );
			(*itr).reset();
		}
	}

	void DynamicsWorld::UpdateTriggers()
	{
		TTriggerSharedPtrList::iterator itr = m_AllTriggers.begin();
		TTriggerSharedPtrList::iterator end = m_AllTriggers.end();

		for( ; itr != end ; itr++ )
		{
			(*itr)->Update();
		}
	}

	void DynamicsWorld::DestroyTrigger(TTriggerAutoPtr _trigger)
	{
		//better algorithms needed probably maps
		Trigger* ptrTrigger = _trigger.release();

		TTriggerSharedPtrList::iterator itr = m_AllTriggers.begin();
		TTriggerSharedPtrList::iterator end = m_AllTriggers.end();

		for( ; itr != end ; itr++ )
		{
			if((*itr).get() == ptrTrigger)
			{
				PANDU_ERROR((*itr).use_count() == 1, "Trigger is already being used, first clear all references" );
				(*itr).reset();

				itr = m_AllTriggers.erase(itr);
				break;
			}
		}
	}
}