/********************************************************************
	filename: 	PANDUViewport
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUViewport_h__
#define __PANDUViewport_h__

#include "../boost/smart_ptr.hpp"
#include "PANDUColour.h"
#include <list>
#include "PANDUCamera.h"
#include "PANDUSceneManager.h"
#include "PANDUFrameBuffer.h"

namespace Pandu
{
	class Viewport;
	typedef boost::shared_ptr<Viewport> TSharedViewportPtr;

	class Viewport
	{
	public:

		typedef std::list<TSharedFrustrumPtr> TSharedFrustrumList;

	private:

		const String				m_Name;
		unsigned int				m_Left;
		unsigned int				m_Bottom;
		unsigned int				m_Width;
		unsigned int				m_Height;
		Colour						m_ClearColour;

		TSharedFrustrumList			m_CameraList;
		bool						m_DirtyAspectRatio;

		TSharedFrameBufferPtr		m_FrameBuffer;

		bool						m_EnableScissor;

	public:
		
		Viewport( const String& _name, unsigned int _left , unsigned int _bottom , unsigned int _width , unsigned int _height);
		virtual ~Viewport();

		void SetViewport(unsigned int _left , unsigned int _bottom , unsigned int _width , unsigned int _height);

		TSharedFrustrumPtr CreateCamera(TCameraCreationFunction _fun,const String& _name, Frustrum::ProjectionType _projType,TSharedSceneManagerPtr _sceneManager);
		
		TSharedFrustrumPtr GetCamera(const String& _name);
		void DestroyCamera(const String& _name);
		void ClearAllCameras();

		const String& GetName() const	{	return m_Name;	}

		inline void SetFrameBuffer(TSharedFrameBufferPtr _frameBuffer)		{	m_FrameBuffer = _frameBuffer;			}
		inline TSharedFrameBufferPtr GetFrameBuffer()						{	return m_FrameBuffer;					}
		inline void SetEnableScissor(bool _enable)							{	m_EnableScissor = _enable;				}

		virtual void UpdateViewport();
		virtual void RenderScene();

	private:

		void UpdateAspectRatio();
		void UpdateFrustrums();
		void SetViewport();
	};
};

#endif