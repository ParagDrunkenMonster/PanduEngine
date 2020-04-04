#include "PANDUPlayerCharacterController.h"

namespace Pandu
{
	PlayerCharacterController::PlayerCharacterController(TActorWeakPtr _character)
		: m_Character(_character)
	{
	}

	PlayerCharacterController::~PlayerCharacterController()
	{
		m_Character.reset();
	}
		
	bool PlayerCharacterController::CanRemoveController() const
	{
		return false;
	}
}