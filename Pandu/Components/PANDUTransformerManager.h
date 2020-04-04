/********************************************************************
	filename: 	PANDUTransformerManager
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUTransformerManager_h__
#define __PANDUTransformerManager_h__

#include "PANDUTransformer.h"
#include <list>
#include "../Utils/PANDUCoreDefines.h"

namespace Pandu
{
	class TransformerManager
	{
	public:

		typedef std::list<TTransformerSharedPtr> TTransformerPtrList;
		typedef TTransformerPtrList::iterator TTransformerPtrListItr;

	private:

		static TransformerManager *m_Instance;

		TTransformerPtrList m_AllTransformers;

	public:


		//--------------------------------------------------------------------
		//Static functions
		static void CreateInstance();
		static void DestroyInstance();
		static TransformerManager* Get();
		//--------------------------------------------------------------------

		TransformerManager();
		~TransformerManager();

		void ClearAllTransformers();

		TTransformerSharedPtr AddTransformer(TTransformerAutoPtr _transformer);
		void DestroyTransformer(TTransformerPtrListItr _transformer);

		void Update(const FrameEvent& _frameEvent);
	};

#define gTransformerManager TransformerManager::Get()
}

#endif