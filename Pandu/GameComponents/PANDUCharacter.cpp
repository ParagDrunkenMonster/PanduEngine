#include "PANDUCharacter.h"

namespace Pandu
{
	Character::Character(unsigned int _interactiveActorType, const String& _carName
						, TDynamicsWorldSharedPtr _world
						,float _charHeight, float _charWidth, const Vector3& _pos, const Vector3& _dir)
		: Actor(_interactiveActorType,_carName)
		, m_World(_world)
		, m_CharacterHeight(_charHeight)
		, m_CharacterWidth(_charWidth)
		, m_Speed(0)
		, m_ImmediateLookDirection(false)
		, m_Direction(_dir)
		, m_DestDir(_dir)
		, m_Position(_pos)
	{
		Initialize(_pos,_dir);
	}

	Character::~Character()
	{
		Destroy();
	}

	void Character::Initialize(const Vector3& _pos, const Vector3& _dir)
	{
		Pandu::RigidBodyDesc desc;
		desc.collisionShape.type = Pandu::CS_Capsule;
		desc.collisionShape.width = 2;
		desc.collisionShape.height = 10.0f;
		desc.collisionShape.depth = 2;
		desc.collisionShape.radius = 5.0f;
		desc.mass = 1.0f;
		desc.transform.setOrigin(btVector3(_pos.x,_pos.y,_pos.z));
		
		m_Body = m_World->CreateCollisionBody(desc);
		

		m_Body->setSleepingThresholds(0.0, 0.0);
		m_Body->setAngularFactor(0.0);

		SetForward(_dir,true);
	}

	void Character::Destroy()
	{
		TBulletRigidBodyAutoPtr body(m_Body.get());
		m_Body.reset();
		m_World->DestroyCollisionBody(body);
		m_World.reset();
	}

	void Character::Update(const FrameEvent& _frameEvent)
	{
		UpdateOrientation(_frameEvent);
		UpdatePosition(_frameEvent);
		UpdatePosOrtVal();
	}

	void Character::UpdatePosition(const FrameEvent& _frameEvent)
	{
		//m_Character->setWalkDirection(btVector3(m_Direction.x,m_Direction.y,m_Direction.z) * m_Speed * _frameEvent.deltaTimeSecond);
		const btVector3& linearVel = m_Body->getLinearVelocity();
		m_Body->setLinearVelocity(linearVel + btVector3(m_Direction.x,m_Direction.y,m_Direction.z) * m_Speed );
		m_Speed = 0.0f;
	}

	void Character::UpdateOrientation(const FrameEvent& _frameEvent)
	{
		if(!m_ImmediateLookDirection && m_DestDir != m_Direction)
		{
		}
	}

	void Character::SetPosition(const Vector3& _pos)
	{
		m_Body->getWorldTransform().setOrigin(btVector3(_pos.x,_pos.y,_pos.z));
		m_Position = _pos;
	}

	const Vector3& Character::GetPosition() const 
	{
		return m_Position;
	}

	const Quaternion& Character::GetRotation() const 
	{
		return Quaternion::IDENTITY;
	}

	void Character::SetForward(const Vector3& _direction,bool _immediate)
	{
		m_ImmediateLookDirection = _immediate;
		if(_immediate)
		{
			m_Direction = _direction;
			m_DestDir = _direction;
		}
		else
		{
			m_DestDir = _direction;
		}
	}

	void Character::UpdatePosOrtVal()
	{
		const btVector3& origin = m_Body->getWorldTransform().getOrigin();
		m_Position = Vector3(origin.x(),origin.y(),origin.z());
	}

	bool Character::TokenizeActor() const
	{
		return false;
	}

	TActorToken Character::GetActorToken() const
	{
		return TActorToken();
	}
}