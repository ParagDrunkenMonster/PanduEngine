/********************************************************************
	filename: 	PANDUCar
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUCar_h__
#define __PANDUCar_h__

#include "PANDUActor.h"
#include "../Math/PANDUVector2.h"
#include "../PanduBulletPhysics/PANDUDynamicsWorld.h"
#include "PANDUCarType.h"
#include "btBulletDynamicsCommon.h"
#include "btBulletCollisionCommon.h"
#include "../Graphics/PANDUMeshInstance.h"
#include "../Components/PANDUTransformer.h"
#include "../Graphics/PANDUSceneNode.h"
#include "../Graphics/PANDUSceneManager.h"

namespace Pandu
{
	//-----------------------------------------------------------------------
	// VehicleToNodeTransformer
	//-----------------------------------------------------------------------

	class Car;

	class VehicleToNodeTransformer : public Transformer
	{	
	private:

		friend class Car;

		TSharedSceneManagerPtr m_SceneManagerPtr;
		TSceneNodesArray m_BodyPartsSceneNodes;
		btRaycastVehicle* m_VehiclePtr;
		TSharedMeshInstancePtr m_WheelMesh;
		TSharedMeshInstancePtr m_ChesisMesh;
		Car* m_ParentCar;

		VehicleToNodeTransformer(Car* _car,TSharedSceneManagerPtr _sceneManager, btRaycastVehicle* _vehicle);
		virtual ~VehicleToNodeTransformer();

		void Create();
		void Destroy();

		TSharedSceneNodePtr CreateWheel(int wheelIndex);
		TSharedSceneNodePtr CreateChesis();

		void UpdateWheel(int wheelIndex);
		void UpdateChesis();

		void SetWheelMesh(TSharedMeshInstancePtr _wheelMesh);
		void SetChesisMesh(TSharedMeshInstancePtr _chesisMesh);
		
	public:
		virtual void Update(const FrameEvent& _frameEvent);
		const Matrix44& GetWorldTransform() const;
	};

	//-----------------------------------------------------------------------
	// Car
	//-----------------------------------------------------------------------

	class Car : public Actor
	{
	private:

		TSharedSceneManagerPtr m_SceneManagerPtr;
		TDynamicsWorldSharedPtr m_World;
	
		btRaycastVehicle::btVehicleTuning	m_Tuning;
		btVehicleRaycaster*	m_VehicleRayCaster;
		btRaycastVehicle*	m_Vehicle;
		btCollisionShape*	m_WheelShape;
		btCollisionShape*	m_ChassisShape;
		btCompoundShape*	m_Compound;
		btRigidBody*		m_CarChassis;

		CarType				m_CarType;

		float				m_VehicleSteering;  //angle in radians
		float				m_Throttle;			//varies from 0.0f - 1.0f
		float				m_Brake;			//varies from 0.0f - 1.0f

		bool				m_LastFrameSteered;

		TSharedMeshInstancePtr m_WheelMesh;
		TSharedMeshInstancePtr m_ChesisMesh;

		TTransformerSharedPtr m_Transformer;

		Vector3				m_Position;
		Quaternion			m_Rotation;

	public:

		Car(unsigned int _actorType,const String& _carName ,TSharedSceneManagerPtr _sceneManager
			, TDynamicsWorldSharedPtr _world,const CarType& _carType);
		~Car();

		virtual void SetPosition(const Vector3& _pos);
		virtual const Vector3& GetPosition() const;

		virtual const Quaternion& GetRotation() const;
	
		virtual void Update(const FrameEvent& _frameEvent);
		
		virtual TCollisionObjectWeakPtr GetCollisionObject();

		virtual bool TokenizeActor() const;
		virtual TActorToken GetActorToken() const;

		float SetThrottle(float _throttle);
		float SetSteering(float _steering);
		float SetBrake(float _brake);

		inline float GetSteering() const	{	return m_VehicleSteering;	}

		virtual void Reset();

		void ClientResetScene();

		void SetWheelMesh(TSharedMeshInstancePtr _wheelMesh);
		void SetChesisMesh(TSharedMeshInstancePtr _chesisMesh);

		//little expensive
		float ComputeSpeed() const;

	private:

		void Initialize();
		void Destroy();

		void UpdatePhysics();
		void ReturnSteeringToOriginalPos();

		void UpdatePosOrtVal();
	};
}

#endif