/********************************************************************
	filename: 	PANDUDynamicsWorld
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUDynamicsWorld_h__
#define __PANDUDynamicsWorld_h__

#include "PANDUBulletTypes.h"
#include "PANDUActorEnterExitTrigger.h"
#include "../Utils/PANDUCoreDefines.h"
#include <list>

namespace Pandu
{
	class DynamicsWorld;

	typedef boost::shared_ptr<DynamicsWorld> TDynamicsWorldSharedPtr;
	typedef boost::weak_ptr<DynamicsWorld> TDynamicsWorldWeakPtr;

	class DynamicsWorld
	{
	public:

		friend class Physics;

		typedef std::list<TBulletCollisionShapeSharedPtr> TCollisionShapeList;
		typedef std::list<TBulletRigidBodySharedPtr> TRigidBodyList;

		typedef std::list<TTriggerSharedPtr> TTriggerSharedPtrList;

	private:

		const String m_Name;
		const WorldType m_WorldType;

		TBulletDynamicsWorldSharedPtr m_BulletDynamicsWorld;

		TBulletBroadPhaseSharedPtr	m_Broadphase;
		TBulletCollisionDispatcherSharedPtr	m_Dispatcher;
		TBulletConstraintSolverSharedPtr	m_Solver;
		TBulletCollisionConfigurationSharedPtr m_CollisionConfiguration;

		unsigned short m_MaxSubSteps;
		float m_FixedTimeStep;

		TCollisionShapeList m_AllCollisionShapes;
		TRigidBodyList m_AllBodies;

		TTriggerSharedPtrList m_AllTriggers;

		TGhostPairCallBackSharedPtr m_GhostPairCallback;

		
		void StepSimulation(float _timeStep);

		void DestroyWorld();

		void CreateWorld(WorldType _worldType);

		TBulletCollisionShapeSharedPtr CreateCollisionShape(const CollisionShapeDesc& _shapeType);
		void DestroyCollisionShape(TBulletCollisionShapeAutoPtr _shape);
		void ClearAllCollisionShapes();
		
		void ClearAllTriggers();
		void UpdateTriggers();

	public:

		DynamicsWorld(const String& _name, WorldType _worldType);
		~DynamicsWorld();

		const String& GetName() const	{	return m_Name;			}

		void SetGravity(const btVector3& _gravity);

		TBulletRigidBodySharedPtr CreateCollisionBody(const RigidBodyDesc& _desc);
		void DestroyCollisionBody(TBulletRigidBodyAutoPtr body);
		void ClearAllCollisionBody();

		void ClientResetScene();

		TBulletDynamicsWorldSharedPtr GetBulletWorld()	{	return m_BulletDynamicsWorld;		}

		TTriggerWeakPtr AddTrigger(TTriggerAutoPtr _trigger);
		void DestroyTrigger(TTriggerAutoPtr _trigger);
	};
}

#endif