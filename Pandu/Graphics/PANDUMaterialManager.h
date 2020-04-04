/********************************************************************
	filename: 	PANDUMaterialManager
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUMaterialManager_h__
#define __PANDUMaterialManager_h__

#include "../boost/smart_ptr.hpp"
#include <list>
#include "PANDUMaterial.h"


namespace Pandu
{
	class MaterialManager
	{
	public:
		friend class GraphicsRoot;

		typedef std::list<TSharedMaterialPtr> TSharedMaterialPtrList;

	private:

		class Impl;
		boost::scoped_ptr<Impl> m_Impl;

		static MaterialManager *m_Instance;

	private:

		//--------------------------------------------------------------------
		//Static functions
		static void CreateInstance();
		static void DestroyInstance();
		//--------------------------------------------------------------------

		MaterialManager();
		~MaterialManager();

	public:

		//--------------------------------------------------------------------
		//Static functions
		static MaterialManager* Get();
		//--------------------------------------------------------------------

		void ClearAllMaterial();

		TSharedMaterialPtr CreateMaterial(const String& _materialName);
		TSharedMaterialPtr GetMaterial(const String& _materialName);
		void DestroyMaterial(const String& _materialName);

	};

	#define gMaterialManager MaterialManager::Get()
}

#endif