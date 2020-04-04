/********************************************************************
	filename: 	PANDUApplication
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUApplication_h__
#define __PANDUApplication_h__

#include <boost/smart_ptr.hpp>
#include "Graphics/PANDUFrameEvent.h"

namespace Pandu
{
	class Application;

	typedef boost::shared_ptr<Application> TSharedApplicationPtr;
	typedef std::auto_ptr<Application> TApplicationAutoPtr;

	class Application
	{
	public:

		Application(){}
		virtual ~Application(){}

		virtual void WindowSizeChanged(float _newWidth, float _newHeight) = 0;

		//called before update call
		//if returns false application will quit...
		virtual bool PreUpdate() = 0;

		//Called before render call
		virtual void Update(const FrameEvent& _event) = 0;
	};
}

#endif