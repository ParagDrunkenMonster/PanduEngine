/********************************************************************
	filename: 	PANDUPhysics
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUPhysics_h__
#define __PANDUPhysics_h__

#include "PANDUBulletTypes.h"
#include "PANDUDynamicsWorld.h"
#include <list>

namespace Pandu
{
	class Physics
	{
	public:
		
		typedef std::list<TDynamicsWorldSharedPtr> TDynamicsWorldsList;

	private:

		static Physics *m_Instance;


		TDynamicsWorldsList m_AllWorlds;

		Physics();
		~Physics();

		void ClearAllWorld();
		

	public:

		//--------------------------------------------------------------------
		//Static functions
		static void CreateInstance();
		static Physics* Get();
		static void DestroyInstance();
		//--------------------------------------------------------------------

		TDynamicsWorldSharedPtr CreateWorld(const String& _name , WorldType _worldType);
		TDynamicsWorldSharedPtr GetWorld(const String& _name);
		void DestroyWorld(const String& _name);

		void StepSimulation(float _time);
	};

#define gPhysics Physics::Get()
}

#endif