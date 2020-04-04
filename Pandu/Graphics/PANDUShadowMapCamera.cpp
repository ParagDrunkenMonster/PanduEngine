#include "PANDUShadowMapCamera.h"

namespace 
{
	static unsigned short kShadowCamCount =  0;
	
	Pandu::TSharedFrustrumPtr CreateShadowCamera(const Pandu::String& _name, Pandu::Frustrum::ProjectionType _projType,Pandu::TSharedSceneManagerPtr _sceneManager)
	{
		Pandu::TSharedFrustrumPtr newCamera(new Pandu::ShadowMapCamera(_name,_projType,_sceneManager));
		return newCamera;
	}
}

namespace Pandu
{
	ShadowMapCamera::ShadowMapCamera(const String& _name, ProjectionType _projType, TSharedSceneManagerPtr _sceneManager)
		: Camera(_name,_projType,_sceneManager)
		, m_ShadowMapCamIndex(kShadowCamCount++)
		, m_TextureMatrix(Pandu::Matrix44::IDENTITY)
	{
		m_TextureMatrix.SetScale(Pandu::Vector3::UNIT * 0.5f);
		m_TextureMatrix.SetTranslate(Pandu::Vector3::UNIT * 0.5f);

		const unsigned short count = AutoShaderParameter::GetShadowProjectionMatrixParamCount();

		if( kShadowCamCount > count )
		{
			gLogManager->WriteWarningLog(String("Only ") + count + " ShadowMapCamera's supported, this camera " + _name + " is not going to effect any shadow parameters");
		}
	}

	ShadowMapCamera::~ShadowMapCamera()
	{

	}

	void ShadowMapCamera::OnRenderStart()
	{
		Camera::OnRenderStart();
		const unsigned short count = AutoShaderParameter::GetShadowProjectionMatrixParamCount();
		if(m_ShadowMapCamIndex < count )
		{
			const Matrix44 shadowProj = m_TextureMatrix * GetProjectionMatrix() * GetViewMatrix();
			AutoShaderParameter::SetShadowProjectionMatrix( (AutoShaderParameter::AutoShaderParameterType)(AutoShaderParameter::ASPT_ModelShadowProjectionMatrix0 + m_ShadowMapCamIndex),shadowProj );
		}
		else
		{
			gLogManager->WriteWarningLog(String("Only ") + count + " ShadowMapCamera's supported, this camera " + GetName() + " is not going to effect any shadow parameters");
		}
	}

	void ShadowMapCamera::OnRenderEnd()
	{
		Camera::OnRenderEnd();
	}

	unsigned short ShadowMapCamera::GetShadowMapCameraCount()
	{
		return kShadowCamCount;
	}

	TCameraCreationFunction ShadowMapCamera::GetCreationFunction()
	{
		return boost::bind(CreateShadowCamera,_1,_2,_3);
	}
}