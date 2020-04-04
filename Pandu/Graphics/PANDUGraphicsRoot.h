/********************************************************************
	filename: 	PANDUGraphicsRoot
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUGraphicsRoot_h__
#define __PANDUGraphicsRoot_h__

#include "../boost/smart_ptr.hpp"
#include <list>
#include <map>
#include "PANDUSceneManager.h"
#include "PANDUViewport.h"

namespace Pandu
{
	class GraphicsRoot
	{
	public:
		
		typedef TSharedSceneManagerPtr (*SceneManagerCreatorFunc)(const String& _name);
		typedef std::map<const String,SceneManagerCreatorFunc> TRegisteredSceneManagerCreators;

	private:

		class Impl;
		boost::scoped_ptr<Impl> m_Impl;

		static GraphicsRoot *m_Instance;


	private:

		GraphicsRoot();
		~GraphicsRoot();

	public:

		//--------------------------------------------------------------------
		//Static functions
		static void CreateInstance();
		static void DestroyInstance();
		static GraphicsRoot* Get();
		//--------------------------------------------------------------------

		void RegisterClassCreator(const String& _typeName, SceneManagerCreatorFunc _creatorFn);

		TSharedSceneManagerPtr CreateSceneManager(const String& _typeName,const String& _managerName);
		TSharedSceneManagerPtr GetSceneManager(const String& _managerName);
		void DestroySceneManager(const String& _managerName);

		TSharedViewportPtr GetViewport(const String& _viewportName);
		void DestroyViewport(const String& _viewportName);
		TSharedViewportPtr CreateViewport(const String& _name, unsigned int _left , unsigned int _bottom , unsigned int _width , unsigned int _height);

		void RenderOneFrame();
	};

#define gGraphicsRoot GraphicsRoot::Get()
}

#endif