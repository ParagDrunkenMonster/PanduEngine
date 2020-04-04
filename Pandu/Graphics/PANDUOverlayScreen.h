/********************************************************************
	filename: 	PANDUOverlayScreen
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUOverlayScreen_h__
#define __PANDUOverlayScreen_h__

#include "PANDUOverlay.h"
#include "../Math/PANDURect.h"

namespace Pandu
{
	class OverlayScreen : public Overlay
	{
	private:

		Rect m_OverlayRect;
		Rect m_ImageRect;
		bool m_DirtyImageRect;

	public:

		OverlayScreen(const String& _name,const Rect& _rect, const Rect& _imageRectTexCoord = Rect::gFullRect);
		virtual ~OverlayScreen();

	private:

		void Construct();

		virtual void PreRenderUpdate();
	};
}

#endif