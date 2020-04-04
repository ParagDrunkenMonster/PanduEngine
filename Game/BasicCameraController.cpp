#include "BasicCameraController.h"


namespace Game
{

	BasicCameraController::BasicCameraController(Pandu::TSharedFrustrumPtr _camera)
		: m_SharedCameraPtr(_camera)
		, m_PositionOffset(Pandu::Vector3(0.0f,10.0f,-15.0f))
		, m_LookAtDist(15.0f)
	{
		Pandu::Camera *camera = dynamic_cast<Pandu::Camera*>(_camera.get());
		assert(camera);
	}

	BasicCameraController::~BasicCameraController()
	{
		m_SharedCameraPtr.reset();
		m_ActorToFollow.reset();
	}

	void BasicCameraController::SetFollowActor(Pandu::TActorWeakPtr _actor)
	{	
		Pandu::Camera *camera = dynamic_cast<Pandu::Camera*>(m_SharedCameraPtr.get());

		m_ActorToFollow = _actor;	

		Pandu::TActorSharedPtr strongActor = _actor.lock();

		const Pandu::Vector3& actorPos = strongActor->GetPosition();
		const Pandu::Quaternion& rotation = strongActor->GetRotation();

		m_CurrPosition = actorPos + rotation * m_PositionOffset;
		m_CurrLookAt = actorPos + rotation * Pandu::Vector3::UNIT_Z * m_LookAtDist;

		camera->SetPosition(m_CurrPosition);
		camera->SetLookAt(m_CurrLookAt);
		camera->SetUpAxis(Pandu::Vector3::UNIT_Y);
	}

	void BasicCameraController::Update(const Pandu::FrameEvent& _frameEvent)
	{
		Pandu::Camera *camera = dynamic_cast<Pandu::Camera*>(m_SharedCameraPtr.get());

		Pandu::TActorSharedPtr strongActor = m_ActorToFollow.lock();

		const Pandu::Vector3& actorPos = strongActor->GetPosition();
		const Pandu::Quaternion& rotation = strongActor->GetRotation();
		Pandu::Vector3 targetPos = actorPos + rotation * m_PositionOffset;
		Pandu::Vector3 targetLookAt = actorPos + rotation * Pandu::Vector3::UNIT_Z * m_LookAtDist;

		m_CurrPosition = m_CurrPosition + ( targetPos - m_CurrPosition ) * 0.25f;
		m_CurrLookAt = m_CurrLookAt + ( targetLookAt - m_CurrLookAt ) * 0.1f;

		camera->SetPosition(m_CurrPosition);
		camera->SetLookAt(m_CurrLookAt);
		camera->SetUpAxis(Pandu::Vector3::UNIT_Y);
	}
}
