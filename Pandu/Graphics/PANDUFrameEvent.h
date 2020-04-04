/********************************************************************
	filename: 	PANDUFrameEvent
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUFrameEvent_h__
#define __PANDUFrameEvent_h__

namespace Pandu
{
	class FrameEvent
	{
	public:
		FrameEvent(): deltaTimeSecond(0.0f){}
		~FrameEvent(){}

		float deltaTimeSecond;
	};
}

#endif