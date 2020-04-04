/********************************************************************
	filename: 	PANDUOverlayFrustrum
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUOverlayFrustrum_h__
#define __PANDUOverlayFrustrum_h__

#include "PANDUFrustrum.h"

namespace Pandu
{
	class OverlayFrustrum : public Frustrum
	{
	private:

	public:

		OverlayFrustrum(const String& _name, ProjectionType _type,TSharedSceneManagerPtr _sceneManager);
		virtual ~OverlayFrustrum();

	private:

		virtual void OnRenderStart();
		virtual void OnRenderEnd();

	public:

		static TCameraCreationFunction GetCreationFunction();
	};
}

#endif