#include "PANDUFrustrum.h"
#include "PANDUGlInclude.h"

namespace
{
	static const float INFINITE_FAR_PLANE_ADJUST = 0.00001f;
}

namespace Pandu
{	
	void Frustrum::UpdateProjectionParameters()
	{
		if(m_DirtyProjectionParameters == false || m_CustomProjParams == true)
		{
			return;
		}
		
		if (m_ProjType == PT_PERSPECTIVE)
		{
			//float thetaY = m_FOVy * 0.5f;
			
			float tanThetaY = (float) m_TanHalfFOVy;
			float tanThetaX = tanThetaY * m_Aspect;
			
			float nearFocal = m_NearDist / m_FocalLength;
			float nearOffsetX = m_FrustumOffset.x * nearFocal;
			float nearOffsetY = m_FrustumOffset.y * nearFocal;
			float half_w = tanThetaX * m_NearDist;
			float half_h = tanThetaY * m_NearDist;
			
			m_Left   = - half_w + nearOffsetX;
			m_Right  = + half_w + nearOffsetX;
			m_Bottom = - half_h + nearOffsetY;
			m_Top    = + half_h + nearOffsetY;
			
			m_DirtyProjectionMatrix = true;
		}
		else
		{
			// Unknown how to apply frustum offset to orthographic camera, just ignore here
			float half_w = m_OrthoHeight * m_Aspect * 0.5f;
			float half_h = m_OrthoHeight * 0.5f;

			m_Left   = - half_w;
			m_Right  = + half_w;
			m_Bottom = - half_h;
			m_Top    = + half_h;
			
			m_DirtyProjectionMatrix = true;
		}
		
		m_DirtyProjectionParameters = false;
	}

	void Frustrum::UpdateProjectionMatrix()
	{
		if(m_DirtyProjectionMatrix == false)
		{
			return;
		}
		
		float inv_w = 1.0f / (m_Right - m_Left);
		float inv_h = 1.0f / (m_Top - m_Bottom);
		float inv_d = 1.0f / (m_FarDist - m_NearDist);

		if (m_ProjType == PT_PERSPECTIVE)
		{
			// Calc matrix elements
			float A = 2 * m_NearDist * inv_w;
			float B = 2 * m_NearDist * inv_h;
			float C = (m_Right + m_Left) * inv_w;
			float D = (m_Top + m_Bottom) * inv_h;
			float q, qn;
			if (m_FarDist == 0)
			{
				// Infinite far plane
				q = INFINITE_FAR_PLANE_ADJUST - 1.0f;
				qn = m_NearDist * (INFINITE_FAR_PLANE_ADJUST - 2.0f);
			}
			else
			{
				q = - (m_FarDist + m_NearDist) * inv_d;
				qn = -2 * (m_FarDist * m_NearDist) * inv_d;
			}

			// NB: This creates 'uniform' perspective projection matrix,
			// which depth range [-1,1], right-handed rules
			//
			// [ A   0   C   0  ]
			// [ 0   B   D   0  ]
			// [ 0   0   q   qn ]
			// [ 0   0   -1  0  ]
			//
			// A = 2 * near / (right - left)
			// B = 2 * near / (top - bottom)
			// C = (right + left) / (right - left)
			// D = (top + bottom) / (top - bottom)
			// q = - (far + near) / (far - near)
			// qn = - 2 * (far * near) / (far - near)

			m_ProjMatrix = Matrix44::IDENTITY;
			m_ProjMatrix[0][0] = A;
			m_ProjMatrix[0][2] = C;
			m_ProjMatrix[1][1] = B;
			m_ProjMatrix[1][2] = D;
			m_ProjMatrix[2][2] = q;
			m_ProjMatrix[2][3] = qn;
			m_ProjMatrix[3][2] = -1.0f;
		}
		else
		{
			float A = 2 * inv_w;
			float B = 2 * inv_h;
			float C = - ( m_Right + m_Left ) * inv_w;
			float D = - ( m_Top + m_Bottom ) * inv_h;
			float q, qn;
			if (m_FarDist == 0)
			{
				// Can not do infinite far plane here, avoid divided zero only
				q = - INFINITE_FAR_PLANE_ADJUST / m_NearDist;
				qn = - INFINITE_FAR_PLANE_ADJUST - 1.0f;
			}
			else
			{
				q = - 2 * inv_d;
				qn = - (m_FarDist + m_NearDist)  * inv_d;
			}

			// NB: This creates 'uniform' orthographic projection matrix,
			// which depth range [-1,1], right-handed rules
			//
			// [ A   0   0   C  ]
			// [ 0   B   0   D  ]
			// [ 0   0   q   qn ]
			// [ 0   0   0   1  ]
			//
			// A = 2 * / (right - left)
			// B = 2 * / (top - bottom)
			// C = - (right + left) / (right - left)
			// D = - (top + bottom) / (top - bottom)
			// q = - 2 / (far - near)
			// qn = - (far + near) / (far - near)

			m_ProjMatrix = Matrix44::ZERO;
			m_ProjMatrix[0][0] = A;
			m_ProjMatrix[0][3] = C;
			m_ProjMatrix[1][1] = B;
			m_ProjMatrix[1][3] = D;
			m_ProjMatrix[2][2] = q;
			m_ProjMatrix[2][3] = qn;
			m_ProjMatrix[3][3] = 1;
		}

		m_InvProjMatrix = m_ProjMatrix;
		m_InvProjMatrix.Inverse();
		
		m_DirtyProjectionMatrix = false;
	}

	void Frustrum::SetProjectionMatrix()
	{
		 // set perspective viewing frustum
	    glMatrixMode(GL_PROJECTION);
	    //gl.glLoadIdentity();
	    LoadProjectionMatrixToGL();
	}

	void Frustrum::LoadProjectionMatrixToGL()
	{
		const Matrix44& proj = GetProjectionMatrix(); 
		
		float matrix[16];
	    
		int p = 0;
		for(int i = 0 ; i < 4 ; i++)
		{
			for(int j = 0 ; j < 4 ; j++)
			{
				matrix[p] = proj[j][i];
				p++;
			}
		}

		if(m_ProjType == PT_PERSPECTIVE)
		{
			glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		}

		glLoadMatrixf(matrix);
	}

	void Frustrum::UpdateScene()
	{
		m_SceneManager->UpdateScene();
	}

	void Frustrum::RenderScene()
	{
		PerformOperationsOnRender(m_OperationsBeforeRender);
		SetProjectionMatrix();
		OnRenderStart();
		m_SceneManager->RenderScene(m_TechniqueIdToRenderScene);
		
		OnRenderEnd();

		CallListenerOnRenderFinished();
		PerformOperationsOnRender(m_OperationsAfterRender);
	}

	void Frustrum::CallListenerOnRenderFinished()
	{
		TOnRenderListenerWeakPtrList::iterator itr = m_OnRenderListeners.begin();

		while( itr != m_OnRenderListeners.end() )
		{
			TOnRenderListenerSharedPtr listener = (*itr).lock();

			if(listener != NULL)
			{
				listener->OnRenderFinished();
			}
			else
			{
				itr = m_OnRenderListeners.end();
			}
			itr++;
		}
	}

	void Frustrum::PerformOperationsOnRender(const Flag<unsigned int>& _operations) const
	{
		GLbitfield mask = 0;
		if( _operations.CheckFlag(OOR_ClearColourBuffer) )
		{
			mask = mask | GL_COLOR_BUFFER_BIT;
		}
		if( _operations.CheckFlag(OOR_ClearDepthBuffer) )
		{
			mask = mask | GL_DEPTH_BUFFER_BIT;
		}

		if( mask != 0 )
		{
			glClear(mask);
		}

		if( _operations.CheckFlag(OOR_Flush) )
		{
			glFlush();
		}
	}
}