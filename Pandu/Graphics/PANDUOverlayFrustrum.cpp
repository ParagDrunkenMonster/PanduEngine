#include "PANDUOverlayFrustrum.h"
#include <boost/bind.hpp>
#include "PANDUGlInclude.h"

Pandu::TSharedFrustrumPtr CreateCamera(const Pandu::String& _name, Pandu::Frustrum::ProjectionType _projType
		,Pandu::TSharedSceneManagerPtr _sceneManager)
{
	Pandu::TSharedFrustrumPtr newCamera(new Pandu::OverlayFrustrum(_name,_projType,_sceneManager));
	return newCamera;
}

namespace Pandu
{
	OverlayFrustrum::OverlayFrustrum(const String& _name, ProjectionType _type,TSharedSceneManagerPtr _sceneManager)
		: Frustrum(_name,_type,_sceneManager)
	{
		SetNearClipDistance(-1.0f);
		SetFarClipDistance(1.0f);
	}

	OverlayFrustrum::~OverlayFrustrum()
	{
	}

	void OverlayFrustrum::OnRenderStart()
	{
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}

	void OverlayFrustrum::OnRenderEnd()
	{
	}

	TCameraCreationFunction OverlayFrustrum::GetCreationFunction()
	{
		return boost::bind(CreateCamera,_1,_2,_3);
	}
}