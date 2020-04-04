/********************************************************************
	filename: 	BasicCameraController
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __BasicCameraController_h__
#define __BasicCameraController_h__

#include "Graphics/PANDUCamera.h"
#include "GameComponents/PANDUActor.h"


namespace Game
{
	class BasicCameraController
	{
	public:
		BasicCameraController(Pandu::TSharedFrustrumPtr _camera);
		~BasicCameraController();

		void SetFollowActor(Pandu::TActorWeakPtr _actor);
		void Update(const Pandu::FrameEvent& _frameEvent);

	private:

		Pandu::TSharedFrustrumPtr m_SharedCameraPtr;
		Pandu::TActorWeakPtr m_ActorToFollow;

		float m_LookAtDist;
		Pandu::Vector3 m_CurrLookAt;
		Pandu::Vector3 m_CurrPosition;
		const Pandu::Vector3 m_PositionOffset;
	};
}

#endif