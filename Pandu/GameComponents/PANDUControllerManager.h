/********************************************************************
	filename: 	PANDUControllerManager
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUControllerManager_h__
#define __PANDUControllerManager_h__

#include "PANDUController.h"
#include "../Utils/PANDUCoreDefines.h"
#include <list>

namespace Pandu
{
	class ControllerManager
	{
	public:

		typedef std::list<TControllerSharedPtr> TControllerSharedPtrList;

	private:

		static ControllerManager *m_Instance;

		TControllerSharedPtrList m_AllControllers;

	public:

		//--------------------------------------------------------------------
		//Static functions
		static void CreateInstance();
		static void DestroyInstance();
		static ControllerManager* Get();
		//--------------------------------------------------------------------

		ControllerManager();
		~ControllerManager();

		void AddController(TControllerSharedPtr _newController);
		void ClearAllControllers();
		void Update(const FrameEvent& _frameEvent);

	};

#define gControllerManager ControllerManager::Get()
}

#endif