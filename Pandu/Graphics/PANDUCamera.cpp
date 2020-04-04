#include "PANDUCamera.h"
#include "PANDUGlInclude.h"


namespace
{
	Pandu::TSharedFrustrumPtr CreateCamera(const Pandu::String& _name, Pandu::Frustrum::ProjectionType _projType
		,Pandu::TSharedSceneManagerPtr _sceneManager)
	{
		Pandu::TSharedFrustrumPtr newCamera(new Pandu::Camera(_name,_projType,_sceneManager));
		return newCamera;
	}
}

namespace Pandu
{
	Camera::Camera(const String& _name, ProjectionType _projType,TSharedSceneManagerPtr _sceneManager)
		: Frustrum(_name,_projType,_sceneManager)
		, m_Position(Vector3(0.0f,0.0f, -5.0f))
		, m_Direction(Vector3::UNIT_Z)
		, m_LookAt(Vector3(0.0f,0.0f,0.0f) )
		, m_UpAxis(Vector3::UNIT_Y)
		, m_DirtyViewMatrix(true)
	{
		ComputeDirection();
	}

	Camera::~Camera()
	{
	}

	void Camera::SetViewMatrix()
	{
		UpdateViewMatrix();
		
		float matrix[16];
        
		int p = 0;
		for(int i = 0 ; i < 4 ; i++)
		{
			for(int j = 0 ; j < 4 ; j++)
			{
				matrix[p] = m_ViewMatrix[j][i];
				p++;
			}
		}
		
		glLoadMatrixf(matrix);
	}

	void Camera::UpdateViewMatrix()
	{
		if(m_DirtyViewMatrix == false)
		{
			return;
		}
			
		Vector3 right = m_Direction.Cross(m_UpAxis);
		right.Normalize();
		Vector3 finalUp = right.Cross(m_Direction);
		finalUp.Normalize();
		
		m_ViewMatrix = Matrix44::IDENTITY;
		m_ViewMatrix.FromAxes(right,finalUp,-m_Direction);
		
		m_InvViewMatrix = m_ViewMatrix;
		m_InvViewMatrix.SetTranslate(m_Position);

		m_ViewMatrix.Transpose();
		Vector3 trans = -(m_ViewMatrix * m_Position);

		m_ViewMatrix[0][3] = trans.x;
		m_ViewMatrix[1][3] = trans.y;
		m_ViewMatrix[2][3] = trans.z;
		
		m_DirtyViewMatrix = false;
	}

	void Camera::OnRenderStart()
	{          
        glMatrixMode(GL_MODELVIEW);
		SetViewMatrix();
	}

	void Camera::OnRenderEnd()
	{
	}

	TCameraCreationFunction Camera::GetCreationFunction()
	{
		return boost::bind(CreateCamera,_1,_2,_3);
	}
}