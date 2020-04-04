/********************************************************************
	filename: 	PANDUDefaultInputSystem
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUDefaultInputSystem_h__
#define __PANDUDefaultInputSystem_h__

#include "PANDUInputSystem.h"

namespace Pandu
{
	class DefaultInputSystem : public InputSystem
	{
	public:
		
		DefaultInputSystem();
		~DefaultInputSystem();

	private:

		virtual void Update(const FrameEvent& _frameEvent);
	};
}

#endif