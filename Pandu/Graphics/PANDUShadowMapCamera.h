/********************************************************************
	filename: 	PANDUShadowMapCamera
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUShadowMapCamera_h__
#define __PANDUShadowMapCamera_h__

#include "PANDUCamera.h"
#include "PANDUAutoShaderParameter.h"

namespace Pandu
{
	class ShadowMapCamera : public Camera
	{
	private:

		const unsigned short		m_ShadowMapCamIndex;
		Matrix44					m_TextureMatrix;

	public:

		ShadowMapCamera(const String& _name, ProjectionType _projType, TSharedSceneManagerPtr _sceneManager);
		virtual ~ShadowMapCamera();

		inline void SetTextureMatrix(const Matrix44& _textureMat)		{	m_TextureMatrix = _textureMat;			}

		static unsigned short GetShadowMapCameraCount();
		static TCameraCreationFunction GetCreationFunction();

	private:

		virtual void OnRenderStart();
		virtual void OnRenderEnd();
	};
}

#endif