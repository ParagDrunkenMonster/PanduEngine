#include "PANDUDefaultInputSystem.h"

namespace Pandu
{
	DefaultInputSystem::DefaultInputSystem()
	: InputSystem()
	{
	}

	DefaultInputSystem::~DefaultInputSystem()
	{
	}

	void DefaultInputSystem::Update(const FrameEvent& _frameEvent)
	{
		UpdateAllInputMethods(_frameEvent);
	}
}