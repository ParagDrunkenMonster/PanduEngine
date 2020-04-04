/********************************************************************
	filename: 	PANDUCharacter
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUCharacter_h__
#define __PANDUCharacter_h__

#include "../Math/PANDUVector3.h"
#include "../Math/PANDUQuaternion.h"
#include "PanduBulletPhysics/PANDUDynamicsWorld.h"
#include "PANDUActor.h"

namespace Pandu
{

	class Character : public Actor
	{
	protected:

		TDynamicsWorldSharedPtr m_World;
		const float m_CharacterHeight;
		const float m_CharacterWidth;

		TBulletRigidBodySharedPtr m_Body;

		Vector3 m_Position;
		Vector3 m_Direction;
		Vector3 m_DestDir;
		float m_Speed;
		bool m_ImmediateLookDirection;


	public:
		Character(unsigned int _interactiveActorType, const String& _carName
				,TDynamicsWorldSharedPtr _world,float _charHeight, float _charWidth
				, const Vector3& _pos, const Vector3& _dir);
		virtual ~Character();

		virtual void Update(const FrameEvent& _frameEvent);
		virtual bool TokenizeActor() const;
		virtual TActorToken GetActorToken() const;

		virtual void SetPosition(const Vector3& _pos);
		virtual const Vector3& GetPosition() const;

		virtual const Quaternion& GetRotation() const;

		inline float GetSpeed() const	{	return m_Speed;		}
		inline void SetSpeed(float _speed)	{	m_Speed = _speed;	}

		virtual TCollisionObjectWeakPtr GetCollisionObject()	{	return m_Body;	}


		const Vector3& GetForward()	{	return m_Direction;		}

		void SetForward(const Vector3& _direction,bool _immediate = false);

	private:

		void UpdatePosition(const FrameEvent& _frameEvent);
		void UpdateOrientation(const FrameEvent& _frameEvent);
		void Initialize( const Vector3& _pos, const Vector3& _dir);
		void Destroy();
		void UpdatePosOrtVal();
	};
}

#endif