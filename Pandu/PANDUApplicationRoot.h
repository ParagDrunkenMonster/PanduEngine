/********************************************************************
	filename: 	PANDUApplicationRoot
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUApplicationRoot_h__
#define __PANDUApplicationRoot_h__

#include "PANDUApplication.h"

namespace Pandu
{
	class ApplicationRoot
	{
	private:

		class Impl;
		boost::scoped_ptr<Impl> m_Impl;

		static ApplicationRoot* m_Instance;

	private:
	
		ApplicationRoot();
		~ApplicationRoot();

	public:

		//static functions
		static void CreateInstance();
		static ApplicationRoot* Get();
		static void DestroyInstance();

		//It will release the previous application
		void SetApplicationPtr(TApplicationAutoPtr application);

		bool Update();
	};

#define gAppRoot ApplicationRoot::Get()

}

#endif