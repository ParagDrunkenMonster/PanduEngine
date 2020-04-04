#ifndef __PANDUCar2D_h__
#define __PANDUCar2D_h__

#include "PANDUActor.h"
#include "PANDUCar2DType.h"
#include "../Math/PANDUVector2.h"
#include "../PanduBulletPhysics/PANDUDynamicsWorld.h"

namespace Pandu
{
	class Car2D : public Actor
	{
	private:

		Car2DType cartype;        // pointer to static car data

		Vector2 position_wc;       // position of car centre in world coordinates
		Vector2 velocity_wc;       // velocity vector of car in world coordinates

		float angle;			// angle of car body orientation (in rads)
		float angularvelocity;

		float steerangle;      // angle of steering (input)
		float throttle;        // amount of throttle (input)
		float brake;           // amount of braking (input)

		Vector2			velocity;
		Vector2			acceleration_wc;
		float     rot_angle;
		float     sideslip;
		float     slipanglefront;
		float     slipanglerear;
		Vector2       force;
		int        rear_slip;
		int        front_slip;
		Vector2       resistance;
		Vector2       acceleration;
		float     torque;
		float     angular_acceleration;
		float     sn, cs;
		float     yawspeed;
		float     weight;
		Vector2       ftraction;
		Vector2       flatf, flatr;


		TDynamicsWorldSharedPtr m_World;
		TBulletRigidBodySharedPtr m_Body;

	public:

		Car2D(unsigned int _actorType,const Car2DType& _carType, TDynamicsWorldSharedPtr _world);
		~Car2D();

		virtual void SetPosition(const Vector3& _pos);
		virtual const Vector3& GetPosition();

		virtual const Vector3& GetDirection();
		virtual void SetDirection(const Vector3& _direction,bool _immediate = false);

		virtual void Update(const FrameEvent& _frameEvent);
		
		virtual TCollisionObjectWeakPtr GetCollisionObject();

		virtual bool TokenizeActor() const;
		virtual TActorToken GetActorToken() const;

		virtual bool OnKeyDown(unsigned long _gameKey);
		virtual bool OnKeyUp(unsigned long _gameKey);
		virtual bool OnKeyHeld(unsigned long _gameKey);
		virtual bool OnKeyPressed(unsigned long _gameKey);

	private:

		void Initialize();
	};
}

#endif