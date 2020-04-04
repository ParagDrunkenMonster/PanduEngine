/********************************************************************
	filename: 	PANDUSkeletonManager
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUSkeletonManager_h__
#define __PANDUSkeletonManager_h__

#include <list>
#include "PANDUSkeleton.h"
#include "../boost/smart_ptr.hpp"
#include "PANDUMesh.h"

namespace Pandu
{
	class SkeletonManager
	{
	private:
		friend class GraphicsRoot;

		class Impl;
		boost::scoped_ptr<Impl> m_Impl;

		static SkeletonManager *m_Instance;

	private:

		//--------------------------------------------------------------------
		//Static functions
		static void CreateInstance();
		static void DestroyInstance();
		//--------------------------------------------------------------------

		SkeletonManager();
		~SkeletonManager();

	public:

		//--------------------------------------------------------------------
		//Static functions
		static SkeletonManager* Get();
		//--------------------------------------------------------------------

		TSharedSkeletonPtr CreateSkeleton(const String& _skeletonName);
		TSharedSkeletonPtr GetSkeleton(const String& _skeletonName);
		void DestroySkeleton(const String& _skeletonName);

	};

	#define gSkeletonManager SkeletonManager::Get()
}

#endif //__PANDUSkeletonManager_h__