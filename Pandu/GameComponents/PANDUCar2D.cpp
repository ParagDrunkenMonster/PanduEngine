#include "PANDUCar2D.h"
#include "../Math/PANDUMathUtils.h"
#include "../Math/PANDUMathConstants.h"

namespace Pandu
{
	/*CARTYPE cartype;        // pointer to static car data

	VEC2 position_wc;       // position of car centre in world coordinates
	VEC2 velocity_wc;       // velocity vector of car in world coordinates

	float angle;           // angle of car body orientation (in rads)
	float angularvelocity;

	float steerangle;      // angle of steering (input)
	float throttle;        // amount of throttle (input)
	float brake;           // amount of braking (input)*/


	Car2D::Car2D(unsigned int _actorType,const Car2DType& _carType, TDynamicsWorldSharedPtr _world)
		: Actor(_actorType)
		, cartype(_carType)
		, m_World(_world)
	{
		Initialize();
	}

	Car2D::~Car2D()
	{
	}

	void Car2D::SetPosition(const Vector3& _pos)
	{
	}

	const Vector3& Car2D::GetPosition()
	{
		return Vector3::ZERO;
	}

	const Vector3& Car2D::GetDirection()
	{
		return Vector3::UNIT_Z;
	}

	void Car2D::SetDirection(const Vector3& _direction,bool _immediate)
	{
	}

	void Car2D::Update(const FrameEvent& _frameEvent)
	{
		sn = sin(angle);
		cs = cos(angle);
		// SAE convention: x is to the front of the car, y is to the right, z is down
		// transform velocity in world reference frame to velocity in car reference frame
		velocity.x =  cs * velocity_wc.y + sn * velocity_wc.x;
		velocity.y = -sn * velocity_wc.y + cs * velocity_wc.x;

		// Lateral force on wheels
		//
		// Resulting velocity of the wheels as result of the yaw rate of the car body
		// v = yawrate * r where r is distance of wheel to CG (approx. half wheel base)
		// yawrate (ang.velocity) must be in rad/s
		//
		yawspeed = cartype.wheelbase * 0.5f * angularvelocity;

		if( velocity.x == 0 )                // TODO: fix Math.singularity
			rot_angle = 0;
		else
			rot_angle = atan( yawspeed / velocity.x);
		// Calculate the side slip angle of the car (a.k.a. beta)
		if( velocity.x == 0 )                // TODO: fix Math.singularity
			sideslip = 0;
		else
			sideslip = atan( velocity.y / velocity.x);

		// Calculate slip angles for front and rear wheels (a.k.a. alpha)
		slipanglefront = sideslip + rot_angle - steerangle;
		slipanglerear  = sideslip - rot_angle;

		// weight per axle = half car mass times 1G (=9.8m/s^2)
		weight = cartype.mass * cartype.gravity* 0.5f;

		// lateral force on front wheels = (Ca * slip angle) capped to friction circle * load
		flatf.x = 0;
		flatf.y = cartype.CA_F * slipanglefront;
		flatf.y = std::min(cartype.MAX_GRIP, flatf.y);
		flatf.y = std::max(-cartype.MAX_GRIP, flatf.y);
		flatf.y *= weight;

		if(front_slip==1)
		{
			flatf.y *= 0.5f;
		}

		// lateral force on rear wheels
		flatr.x = 0;
		flatr.y = cartype.CA_R * slipanglerear;
		flatr.y = std::min(cartype.MAX_GRIP, flatr.y);
		flatr.y = std::max(-cartype.MAX_GRIP, flatr.y);
		flatr.y *= weight;
		if(rear_slip==1)
		{
			flatr.y *= 0.5;
		}

		// longtitudinal force on rear wheels - very simple traction model
		ftraction.x = 100.0f * (throttle - brake*Sign(velocity.x));
		ftraction.y = 0;
		if(rear_slip==1)
		{
			ftraction.x *= 0.5f;
		}

		// Forces and torque on body

		// drag and rolling resistance
		resistance.x = -( cartype.RESISTANCE*velocity.x + cartype.DRAG*velocity.x*Abs(velocity.x) );
		resistance.y = -( cartype.RESISTANCE*velocity.y + cartype.DRAG*velocity.y*Abs(velocity.y) );

		// sum forces
		force.x = ftraction.x + sin(steerangle) * flatf.x + flatr.x + resistance.x;
		force.y = ftraction.y + cos(steerangle) * flatf.y + flatr.y + resistance.y;

		// torque on body from lateral forces
		torque = cartype.b * flatf.y - cartype.c * flatr.y;

		// Acceleration

		// Newton F = m.a, therefore a = F/m
		acceleration.x = force.x/cartype.mass;
		acceleration.y = force.y/cartype.mass;
		angular_acceleration = torque / cartype.inertia;

		// Velocity and position

		// transform acceleration from car reference frame to world reference frame
		acceleration_wc.x =  cs * acceleration.y + sn * acceleration.x;
		acceleration_wc.y = -sn * acceleration.y + cs * acceleration.x;

		// velocity is integrated acceleration
		//
		velocity_wc.x += _frameEvent.deltaTimeSecond * acceleration_wc.x;
		velocity_wc.y += _frameEvent.deltaTimeSecond * acceleration_wc.y;

		// position is integrated velocity
		//
		position_wc.x += _frameEvent.deltaTimeSecond * velocity_wc.x;
		position_wc.y += _frameEvent.deltaTimeSecond * velocity_wc.y;


		// Angular velocity and heading

		// integrate angular acceleration to get angular velocity
		//
		angularvelocity += _frameEvent.deltaTimeSecond * angular_acceleration;

		// integrate angular velocity to get angular orientation
		//
		angle += _frameEvent.deltaTimeSecond * angularvelocity ;



		const btVector3& linearVel = m_Body->getLinearVelocity();
		m_Body->setLinearVelocity(btVector3(velocity.y,velocity.x,0.0f)  );
		//m_Body->applyTorqueImpulse(btVector3(torque,torque,torque));
		//m_Body->setAngularVelocity(btVector3(0.0f,0.0f,angularvelocity)  );
		//m_Body->getCenterOfMassTransform
	}
	
	TCollisionObjectWeakPtr Car2D::GetCollisionObject()
	{
		return TCollisionObjectWeakPtr();
	}

	bool Car2D::TokenizeActor() const
	{
		return false;
	}

	TActorToken Car2D::GetActorToken() const
	{
		return TActorToken();
	}

	bool Car2D::OnKeyDown(unsigned long _gameKey)
	{
		return false;
	}

	bool Car2D::OnKeyUp(unsigned long _gameKey)
	{
		return false;
	}

	bool Car2D::OnKeyHeld(unsigned long _gameKey)
	{
		/*
		GIK_Left = 0,
		GIK_Right,
		GIK_Up,
		GIK_Down,
		*/

		if(_gameKey == 2)
		{
			if( throttle < 100) 
			{
				throttle += 10;
			}
		}
		else if(_gameKey == 3)
		{
			if( throttle >= 10) 
			{
				throttle -= 10;
			}
		}
		else if(_gameKey == 0)
		{
			if( steerangle > - kPI/4.0 ) 
			{
				steerangle -= kPI/30.0;
			}
		}
		else if(_gameKey == 1)
		{
			if( steerangle <  kPI/4.0 ) 
			{
				steerangle += kPI/30.0;
			}
		}
		return false;
	}

	bool Car2D::OnKeyPressed(unsigned long _gameKey)
	{
		return false;
	}

	void Car2D::Initialize()
	{
		position_wc = Vector2(-40,20);
		velocity_wc = Vector2::ZERO;

		angle = 0.0f;
		angularvelocity = 0.0f;

		steerangle = 0.0f;
		throttle = 0.0f;
		brake = 0.0f;

		velocity = Vector2::ZERO;
		acceleration_wc = Vector2::ZERO;

		rot_angle = 0.0f;
		sideslip = 0.0f;
		slipanglefront = 0.0f;
		slipanglerear = 0.0f;

		force = Vector2::ZERO;

		rear_slip = 1;
		front_slip = 1;

		resistance = Vector2::ZERO;
		acceleration = Vector2::ZERO;

		torque = 0.0f;
		angular_acceleration = 0.0f;
		sn = 0.0f;
		cs = 0.0f;
		yawspeed = 0.0f;
		weight = 0.0f;

		ftraction = Vector2::ZERO;
		flatf = Vector2::ZERO;
		flatr = Vector2::ZERO;



		RigidBodyDesc desc;
		desc.collisionShape.type = Pandu::CS_Capsule;
		desc.collisionShape.width = 0;
		desc.collisionShape.height = 10.0f;
		desc.collisionShape.depth = 0;
		desc.collisionShape.radius = 5.0f;
		desc.mass = 1.0f;
		desc.transform.setOrigin(btVector3(position_wc.x,position_wc.y,0.0f));
		
		m_Body = m_World->CreateCollisionBody(desc);
		

		m_Body->setSleepingThresholds(0.0, 0.0);
		//m_Body->setAngularFactor(0.0);

	}
}