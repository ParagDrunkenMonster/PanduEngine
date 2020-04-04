#include "PANDUCar.h"
#include "../Math/PANDUMathUtils.h"
#include "../Math/PANDUMathConstants.h"
#include "../PanduBulletPhysics/PANDUBulletUtils.h"
#include "../Components/PANDUTransformerManager.h"

namespace Pandu
{
	float	maxEngineForce = 2000.f;//this should be engine/velocity dependent
	float	maxBreakingForce = 500.f;

	float	autoSteeringIncrement = 0.04f;
	float	steeringClamp = 0.4f;
	float	wheelRadius = 0.5f;
	float	wheelWidth = 0.4f;
	float	wheelFriction = 1000;//BT_LARGE_FLOAT;
	float	suspensionStiffness = 20.f;
	float	suspensionDamping = 2.3f;
	float	suspensionCompression = 4.4f;
	float	rollInfluence = 0.1f;//1.0f;

	float suspensionRestLength(0.6f);

	int rightIndex = 0;
	int upIndex = 1;
	int forwardIndex = 2;
	btVector3 wheelDirectionCS0(0,-1,0);
	btVector3 wheelAxleCS(-1,0,0);

	#define CUBE_HALF_EXTENTS 1

	//-----------------------------------------------------------------------
	// VehicleToNodeTransformer
	//-----------------------------------------------------------------------

	VehicleToNodeTransformer::VehicleToNodeTransformer(Car* _car,TSharedSceneManagerPtr _sceneManager
											, btRaycastVehicle* _vehicle)
		: m_SceneManagerPtr(_sceneManager)
		, m_VehiclePtr(_vehicle)
		, m_ParentCar(_car)
	{
		Create();
	}

	VehicleToNodeTransformer::~VehicleToNodeTransformer()
	{
		Destroy();
	}

	void VehicleToNodeTransformer::Create()
	{
		int wheelCount = m_VehiclePtr->getNumWheels();

		for( int i = 0 ; i < wheelCount ; i++ )
		{
			TSharedSceneNodePtr node = CreateWheel(i);
			m_BodyPartsSceneNodes.push_back(node);
		}

		TSharedSceneNodePtr chesis = CreateChesis();
		m_BodyPartsSceneNodes.push_back(chesis);
	}

	void VehicleToNodeTransformer::Destroy()
	{
		int count = (int)m_BodyPartsSceneNodes.size();

		for( int i = 0 ; i < count ; i++ )
		{
			m_BodyPartsSceneNodes[i].reset();
		}
	}

	void VehicleToNodeTransformer::Update(const FrameEvent& _frameEvent)
	{
		int count = m_VehiclePtr->getNumWheels();

		for( int i = 0 ; i < count ; i++ )
		{
			m_VehiclePtr->updateWheelTransform(i);
			UpdateWheel(i);
		}

		UpdateChesis();
	}

	void VehicleToNodeTransformer::UpdateWheel(int wheelIndex)
	{
		const btWheelInfo& wheelInfo = m_VehiclePtr->getWheelInfo(wheelIndex);
		TSharedSceneNodePtr wheelNode = m_BodyPartsSceneNodes[wheelIndex];

		const btTransform& transform = wheelInfo.m_worldTransform;
		btQuaternion quat = transform.getRotation();
		wheelNode->SetRotation(Quaternion(quat.w(),quat.x(),quat.y(),quat.z()));
		btVector3 pos = transform.getOrigin();
		wheelNode->SetPosition(Vector3(pos.x(),pos.y(),pos.z()));
	}

	void VehicleToNodeTransformer::UpdateChesis()
	{
		int wheelCount = m_VehiclePtr->getNumWheels();

		TSharedSceneNodePtr chesisNode = m_BodyPartsSceneNodes[wheelCount];

		const btTransform& transform = m_VehiclePtr->getRigidBody()->getWorldTransform();
		btQuaternion quat = transform.getRotation();
		chesisNode->SetRotation(Quaternion(quat.w(),quat.x(),quat.y(),quat.z()));
		btVector3 pos = transform.getOrigin();
		chesisNode->SetPosition(Vector3(pos.x(),pos.y(),pos.z()));
	}

	TSharedSceneNodePtr VehicleToNodeTransformer::CreateWheel(int wheelIndex)
	{
		const btWheelInfo& wheelInfo = m_VehiclePtr->getWheelInfo(wheelIndex);

		char wheelIndexStr[32];

		itoa(wheelIndex,wheelIndexStr,10);
		Pandu::TSharedSceneNodePtr node = m_SceneManagerPtr->CreateChildSceneNodeOfNode(m_SceneManagerPtr->GetRootSceneNode(),m_ParentCar->GetName() + "_Wheel" + wheelIndexStr);		
		node->SetOverrideParentTransforms(true);

		if(m_WheelMesh != NULL)
		{
			node->AddRenderable(m_WheelMesh);
		}
		return node;
	}

	TSharedSceneNodePtr VehicleToNodeTransformer::CreateChesis()
	{
		Pandu::TSharedSceneNodePtr node = m_SceneManagerPtr->CreateChildSceneNodeOfNode(m_SceneManagerPtr->GetRootSceneNode(),m_ParentCar->GetName() + "_Chesis");
		node->SetOverrideParentTransforms(true);

		if(m_ChesisMesh != NULL)
		{
			node->AddRenderable(m_ChesisMesh);
		}
		return node;
	}

	void VehicleToNodeTransformer::SetWheelMesh(TSharedMeshInstancePtr _wheelMesh)
	{
		m_WheelMesh = _wheelMesh;

		int wheelCount = m_VehiclePtr->getNumWheels();

		for( int i = 0 ; i < wheelCount ; i++ )
		{
			m_BodyPartsSceneNodes[i]->AddRenderable(m_WheelMesh);
		}
	}

	void VehicleToNodeTransformer::SetChesisMesh(TSharedMeshInstancePtr _chesisMesh)
	{
		m_ChesisMesh = _chesisMesh;

		int wheelCount = m_VehiclePtr->getNumWheels();

		m_BodyPartsSceneNodes[wheelCount]->AddRenderable(m_ChesisMesh);
	}

	const Matrix44& VehicleToNodeTransformer::GetWorldTransform() const
	{
		int wheelCount = m_VehiclePtr->getNumWheels();
		return m_BodyPartsSceneNodes[wheelCount]->GetWorldTransform();
	}

	//-----------------------------------------------------------------------
	// Car
	//-----------------------------------------------------------------------
	Car::Car(unsigned int _actorType,const String& _carName
						, TSharedSceneManagerPtr _sceneManager
						, TDynamicsWorldSharedPtr _world,const CarType& _carType)
		: Actor(_actorType,_carName)
		, m_World(_world)
		, m_SceneManagerPtr(_sceneManager)
		, m_CarType(_carType)
		, m_VehicleRayCaster(NULL)
		, m_Vehicle(NULL)
		, m_WheelShape(NULL)
		, m_CarChassis(NULL)
		, m_ChassisShape(NULL)
		, m_Compound(NULL)
	{
		Initialize();
		Reset();
	}

	Car::~Car()
	{
		Destroy();
	}

	void Car::SetPosition(const Vector3& _pos)
	{
	}

	const Vector3& Car::GetPosition() const 
	{
		return m_Position;
	}

	const Quaternion& Car::GetRotation() const 
	{
		return m_Rotation;
	}

	void Car::Update(const FrameEvent& _frameEvent)
	{
		UpdatePhysics();
		ReturnSteeringToOriginalPos();
		UpdatePosOrtVal();
	}
	
	TCollisionObjectWeakPtr Car::GetCollisionObject()
	{
		return TCollisionObjectWeakPtr();
	}

	bool Car::TokenizeActor() const
	{
		return false;
	}

	TActorToken Car::GetActorToken() const
	{
		return TActorToken();
	}

	void Car::Initialize()
	{
		m_ChassisShape = new btBoxShape(btVector3(1.f,0.5f,2.f));
		m_Compound = new btCompoundShape();

		btTransform localTrans;
		
		localTrans.setIdentity();
		localTrans.setOrigin(btVector3(0,1,0));

		m_Compound->addChildShape(localTrans,m_ChassisShape);

		btTransform tr;
		tr.setIdentity();
		tr.setOrigin(btVector3(0,5,0));

		m_CarChassis = CreateRigidBody(m_World->GetBulletWorld().get(),800,tr,m_Compound);//chassisShape);
		//m_CarChassis->setDamping(0.2,0.2);
		
		m_WheelShape = new btCylinderShapeX(btVector3(wheelWidth,wheelRadius,wheelRadius));

		ClientResetScene();

		TBulletDynamicsWorldSharedPtr dynamicsWorldShared = m_World->GetBulletWorld();
		btDynamicsWorld *dynamicsWorld = dynamicsWorldShared.get();
		/// create vehicle
		{
			
			m_VehicleRayCaster = new btDefaultVehicleRaycaster(dynamicsWorld);
			m_Vehicle = new btRaycastVehicle(m_Tuning,m_CarChassis,m_VehicleRayCaster);
			
			///never deactivate the vehicle
			//m_CarChassis->setActivationState(DISABLE_DEACTIVATION);

			dynamicsWorld->addVehicle(m_Vehicle);

			float connectionHeight = 1.2f;

		
			bool isFrontWheel=true;

			//choose coordinate system
			m_Vehicle->setCoordinateSystem(rightIndex,upIndex,forwardIndex);

			btVector3 connectionPointCS0(CUBE_HALF_EXTENTS-(0.3f*wheelWidth),connectionHeight,2.0f*CUBE_HALF_EXTENTS-wheelRadius);

			m_Vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_Tuning,isFrontWheel);

			connectionPointCS0 = btVector3(-CUBE_HALF_EXTENTS+(0.3f*wheelWidth),connectionHeight,2.0f*CUBE_HALF_EXTENTS-wheelRadius);


			m_Vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_Tuning,isFrontWheel);

			connectionPointCS0 = btVector3(-CUBE_HALF_EXTENTS+(0.3f*wheelWidth),connectionHeight,-2.0f*CUBE_HALF_EXTENTS+wheelRadius);

			isFrontWheel = false;
			m_Vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_Tuning,isFrontWheel);

			connectionPointCS0 = btVector3(CUBE_HALF_EXTENTS-(0.3f*wheelWidth),connectionHeight,-2.0f*CUBE_HALF_EXTENTS+wheelRadius);

			m_Vehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,m_Tuning,isFrontWheel);
				
			for ( int i = 0 ; i < m_Vehicle->getNumWheels() ; i++ )
			{
				btWheelInfo& wheel = m_Vehicle->getWheelInfo(i);
				wheel.m_suspensionStiffness = suspensionStiffness;
				wheel.m_wheelsDampingRelaxation = suspensionDamping;
				wheel.m_wheelsDampingCompression = suspensionCompression;
				wheel.m_frictionSlip = wheelFriction;
				wheel.m_rollInfluence = rollInfluence;
			}
		}

		TTransformerAutoPtr trans(new VehicleToNodeTransformer(this,m_SceneManagerPtr,m_Vehicle));

		m_Transformer = gTransformerManager->AddTransformer(trans);
		
		UpdatePosOrtVal();
	}

	float Car::SetThrottle(float _throttle)
	{ 
		m_Throttle = _throttle; 
		Clamp(m_Throttle,-1.0f,1.0f);
		if(m_Throttle != 0.0f)
		{
			m_CarChassis->activate(true);
		}

		return m_Throttle;
	}

	float Car::SetSteering(float _steering)
	{
		m_VehicleSteering = _steering;
		Clamp(m_VehicleSteering,-steeringClamp,steeringClamp);
		m_LastFrameSteered = true;

		return m_VehicleSteering;
	}

	float Car::SetBrake(float _brake)
	{
		m_Brake = _brake;
		Clamp(m_Brake,0.0f,1.0f);

		return m_Brake;
	}

	void Car::UpdatePhysics()
	{
		int wheelIndex = 2;
		m_Vehicle->applyEngineForce(maxEngineForce * m_Throttle,wheelIndex);
		m_Vehicle->setBrake(maxBreakingForce * m_Brake,wheelIndex);
		wheelIndex = 3;
		m_Vehicle->applyEngineForce(maxEngineForce * m_Throttle,wheelIndex);
		m_Vehicle->setBrake(maxBreakingForce * m_Brake,wheelIndex);

		wheelIndex = 0;
		m_Vehicle->setSteeringValue(m_VehicleSteering,wheelIndex);
		wheelIndex = 1;
		m_Vehicle->setSteeringValue(m_VehicleSteering,wheelIndex);
	}

	void Car::ReturnSteeringToOriginalPos()
	{
		if(!m_LastFrameSteered )
		{
			if(m_VehicleSteering != 0.0f)
			{
				const btVector3& velocity = m_Vehicle->getRigidBody()->getLinearVelocity();
				btVector3 forwardVector = m_Vehicle->getForwardVector();

				if(velocity.dot(forwardVector) != 0.0f )
				{
					m_VehicleSteering =  Sign(m_VehicleSteering) * Max(0.0f,Abs(m_VehicleSteering) - autoSteeringIncrement);
				}
			}
		}

		m_LastFrameSteered = false;
	}

	void Car::ClientResetScene()
	{
		Reset();

		m_CarChassis->setCenterOfMassTransform(btTransform::getIdentity());
		m_CarChassis->setLinearVelocity(btVector3(0,0,0));
		m_CarChassis->setAngularVelocity(btVector3(0,0,0));

		btDynamicsWorld* world = m_World->GetBulletWorld().get();

		world->getBroadphase()->getOverlappingPairCache()->cleanProxyFromPairs(m_CarChassis->getBroadphaseHandle(),world->getDispatcher());
		if (m_Vehicle)
		{
			m_Vehicle->resetSuspension();
			for (int i=0;i<m_Vehicle->getNumWheels();i++)
			{
				//synchronize the wheels with the (interpolated) chassis worldtransform
				m_Vehicle->updateWheelTransform(i,true);
			}
		}

	}

	void Car::Destroy()
	{
		m_Compound->removeAllChildShapes();

		delete m_ChassisShape;
		m_ChassisShape = NULL;

		delete m_Compound;
		m_Compound = NULL;

		m_World->GetBulletWorld()->removeRigidBody(m_CarChassis);
		delete m_CarChassis;
		m_CarChassis = NULL;

		delete m_WheelShape;
		m_WheelShape = NULL;

		delete m_VehicleRayCaster;
		m_VehicleRayCaster = NULL;

		delete m_Vehicle;
		m_Vehicle = NULL;
		
		m_World.reset();

		m_WheelMesh.reset();
	}

	void Car::Reset()
	{
		m_Throttle = 0.0f;
		m_Brake = 0.0f;
		m_LastFrameSteered = false;
	}

	void Car::SetWheelMesh(TSharedMeshInstancePtr _wheelMesh)
	{
		m_WheelMesh = _wheelMesh;
		if(m_Transformer != NULL)
		{
			((VehicleToNodeTransformer*)m_Transformer.get())->SetWheelMesh(m_WheelMesh);
		}
	}

	void Car::SetChesisMesh(TSharedMeshInstancePtr _chesisMesh)
	{
		m_ChesisMesh = _chesisMesh;
		if(m_Transformer != NULL)
		{
			((VehicleToNodeTransformer*)m_Transformer.get())->SetChesisMesh(m_ChesisMesh);
		}
	}

	void Car::UpdatePosOrtVal()
	{
		const btVector3& origin = m_CarChassis->getWorldTransform().getOrigin();
		m_Position =  Vector3(origin.x(),origin.y(),origin.z());

		const btQuaternion& rot = m_CarChassis->getWorldTransform().getRotation();
		m_Rotation = Quaternion(rot.w(),rot.x(),rot.y(),rot.z());
	}

	float Car::ComputeSpeed() const
	{
		VehicleToNodeTransformer* transformer = static_cast<VehicleToNodeTransformer*>(m_Transformer.get());
		Pandu::Vector3 forward = transformer->GetWorldTransform().GetColumnVec3(2);

		const btVector3& btVel = m_CarChassis->getLinearVelocity();
		Vector3 linearVel(btVel.x(),btVel.y(),btVel.z());

		return forward.Dot(linearVel);
	}
}