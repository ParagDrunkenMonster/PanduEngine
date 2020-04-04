#include "PANDUViewport.h"
#include "PANDUGlInclude.h"

namespace Pandu
{
	Viewport::Viewport(const String& _name, unsigned int _left , unsigned int _bottom , unsigned int _width , unsigned int _height)
		: m_Name(_name)
		, m_DirtyAspectRatio(true)
		, m_ClearColour(Colour(0,0,0,1))
		, m_EnableScissor(false)
	{
		SetViewport(_left,_bottom,_width,_height);
	}

	Viewport::~Viewport()
	{
		ClearAllCameras();
	}

	void Viewport::SetViewport(unsigned int _left , unsigned int _bottom , unsigned int _width , unsigned int _height)
	{
		m_Left = _left;
		m_Bottom = _bottom;
		m_Width = _width;
		m_Height = _height;

		m_DirtyAspectRatio = true;
	}

	TSharedFrustrumPtr Viewport::CreateCamera(TCameraCreationFunction _fun,const String& _name
					, Frustrum::ProjectionType _projType,TSharedSceneManagerPtr _sceneManager)
	{
		TSharedFrustrumPtr camera = GetCamera(_name);

		PANDU_ERROR(camera == NULL , (String("Camera named ") + camera->GetName() 
			+ String(" already exists") ).CString() );

		if(camera == NULL)
		{
			camera = _fun(_name,_projType,_sceneManager);

			m_CameraList.push_back(camera);

			float ratio =  (float)m_Width/(float)m_Height;
			camera->SetViewportAspectRatio(ratio);
		}

		return camera;
	}

	TSharedFrustrumPtr Viewport::GetCamera(const String& _name)
	{
		TSharedFrustrumList::iterator itr = m_CameraList.begin();
		TSharedFrustrumList::iterator end = m_CameraList.end();

		TSharedFrustrumPtr found;

		for( ; itr != end ; itr++ )
		{
			if((*itr)->GetName() == _name )
			{
				found = (*itr);
				break;
			}
		}

		return found;
	}

	void Viewport::DestroyCamera(const String& _name)
	{
		TSharedFrustrumList::iterator itr = m_CameraList.begin();
		TSharedFrustrumList::iterator end = m_CameraList.end();

		TSharedFrustrumPtr found;

		for( ; itr != end ; itr++ )
		{
			if((*itr)->GetName() == _name )
			{
				found = (*itr);
				itr = m_CameraList.erase(itr);
				break;
			}
		}

		PANDU_ERROR(found.use_count() == 1 , (String("Camera named ") + found->GetName() 
					+ String(" could not be deleted still in use, but removed from list, may have memory leaks") ).CString() );

		found.reset();
	}

	void Viewport::ClearAllCameras()
	{
		TSharedFrustrumList::iterator itr = m_CameraList.begin();
		TSharedFrustrumList::iterator end = m_CameraList.end();

		for( ; itr != end ; itr++ )
		{
			PANDU_ERROR((*itr).use_count() == 1 , (String("Camera named ") + (*itr)->GetName() 
				+ String(" could not be deleted still in use, but removed from list, may have memory leaks") ).CString() );
			(*itr).reset();
		}

		m_CameraList.clear();
	}

	void Viewport::UpdateViewport()
	{
		UpdateAspectRatio();
		UpdateFrustrums();
	}

	void Viewport::RenderScene()
	{
		TSharedFrustrumList::iterator itr = m_CameraList.begin();
		TSharedFrustrumList::iterator end = m_CameraList.end();

		TSharedFrustrumPtr found;

		bool frameBufferEnabled = false;
		if( m_FrameBuffer != NULL && m_FrameBuffer->IsLoaded() )
		{
			frameBufferEnabled = m_FrameBuffer->EnableFrameBuffer();
		}
		else
		{
			glColorMask(GL_TRUE,GL_TRUE,GL_TRUE,GL_TRUE);
			glDepthMask(GL_TRUE);
		}

		SetViewport();
		for( ; itr != end ; itr++ )
		{
			(*itr)->RenderScene();
		}

		if(frameBufferEnabled)
		{
			m_FrameBuffer->DisableFrameBuffer();
		}
	}

	void Viewport::UpdateAspectRatio()
	{
		if(m_DirtyAspectRatio)
		{
			TSharedFrustrumList::iterator itr = m_CameraList.begin();
			TSharedFrustrumList::iterator end = m_CameraList.end();

			TSharedFrustrumPtr found;

			float ratio =  (float)m_Width/(float)m_Height;
			
			for( ; itr != end ; itr++ )
			{
				(*itr)->SetViewportAspectRatio(ratio);
				(*itr)->UpdateScene();
			}
		}
		m_DirtyAspectRatio = false;
	}
	
	void Viewport::UpdateFrustrums()
	{
		TSharedFrustrumList::iterator itr = m_CameraList.begin();
		TSharedFrustrumList::iterator end = m_CameraList.end();

		TSharedFrustrumPtr found;
		
		for( ; itr != end ; itr++ )
		{
			(*itr)->UpdateScene();
		}
	}

	void Viewport::SetViewport()
	{
		//set the viewport
		glViewport(m_Left, m_Bottom, m_Width, m_Height);
		if( m_EnableScissor )
		{
			glEnable(GL_SCISSOR_TEST);
			glScissor(m_Left, m_Bottom, m_Width, m_Height);
		}
		else
		{
			glDisable(GL_SCISSOR_TEST);
		}
		
		glClearColor(m_ClearColour.r,m_ClearColour.g, m_ClearColour.b,m_ClearColour.a);
	}
}