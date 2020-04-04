/********************************************************************
	filename: 	PANDUController
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUController_h__
#define __PANDUController_h__

#include "../boost/smart_ptr.hpp"
#include "../Graphics/PANDUFrameEvent.h"

namespace Pandu
{
	class Controller;

	typedef boost::shared_ptr<Controller> TControllerSharedPtr;
	typedef boost::weak_ptr<Controller> TControllerWeakPtr;

	class Controller
	{
	public:

		Controller(){}
		virtual ~Controller(){}

		virtual bool CanRemoveController() const	= 0;
		virtual void Update(const FrameEvent& _frameEvent) = 0;
	};
}

#endif