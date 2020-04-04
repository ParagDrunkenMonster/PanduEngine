/********************************************************************
	filename: 	PANDUMeshManager
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUMeshManager_h__
#define __PANDUMeshManager_h__

#include "../boost/smart_ptr.hpp"
#include <boost/function.hpp>
#include "PANDUMesh.h"

namespace Pandu
{
	class MeshManager
	{
	public:
		friend class GraphicsRoot;

		typedef boost::function<TSharedMeshPtr ()> TMeshCreator;

	private:

		class Impl;
		boost::scoped_ptr<Impl> m_Impl;

		static MeshManager *m_Instance;

	private:

		//--------------------------------------------------------------------
		//Static functions
		static void CreateInstance();
		static void DestroyInstance();
		//--------------------------------------------------------------------

		MeshManager();
		~MeshManager();

	public:

		//--------------------------------------------------------------------
		//Static functions
		static MeshManager* Get();
		//--------------------------------------------------------------------

		void ClearAllMesh();

		TSharedMeshPtr CreateMesh(const String& _meshName);
		TSharedMeshPtr CreateMesh(TMeshCreator _meshCreator);
		TSharedMeshPtr GetMesh(const String& _meshName);
		void DestroyMesh(const String& _meshName);

	};
}

#define gMeshManager MeshManager::Get()

#endif