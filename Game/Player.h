/********************************************************************
	filename: 	Player
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __Player_h__
#define __Player_h__

#include "GameComponents/PANDUCharacter.h"
#include "PanduBulletPhysics/PANDUPhysics.h"
#include "Components/PANDUDefaultInputSystem.h"
#include "GamePlayerCharacterController.h"
#include "../NeHeGL.h"
#include "GameCarController.h"
#include "Graphics/PANDUSceneManager.h"
#include "Graphics/PANDUShaderManager.h"
#include "Graphics/PANDUMaterialManager.h"


namespace Game
{
	class Player
	{
	private:

		bool m_Initialized;
		bool m_MaterialSet;

		Pandu::TSharedVertexProgramPtr m_CarVertexShader;
		Pandu::TSharedFragmentProgramPtr m_CarPixelShader;

		Pandu::TSharedVertexProgramPtr m_RenderShadowVertexProgram;
		Pandu::TSharedFragmentProgramPtr m_RenderShadowFragmentProgram;

		Pandu::TSharedMeshInstancePtr	m_WheelMeshInst;
		Pandu::TSharedMeshInstancePtr	m_CarMeshInst;

		Pandu::TActorTokenSharedPtr m_PlayerToken;
		Pandu::TActorWeakPtr m_PlayerCharacter;
		TGamePlayerCharacterControllerSharedPtr m_PlayerController;
		TGameCarControllerSharedPtr m_CarController;

		Player();
		~Player();
                         
		static Player* m_Instance;

	public:

		Pandu::TActorTokenSharedPtr GetPlayerToken()		{	return m_PlayerToken;		}
		Pandu::TActorWeakPtr GetActor()	{	return m_PlayerCharacter;	}

		void SetMaterialToCar(Pandu::TSharedTexturePtr _shadowMapTex);

	public:
	
		static void CreateInstance();
		static void DestroyInstance();
		static Player* Get();

		static bool PlayerCharCreatDestRemoveCondition();
		
		void Initialize(Pandu::TSharedSceneManagerPtr _sceneManager,Pandu::TDynamicsWorldSharedPtr _world,Pandu::TInputSystemSharedPtr _ipSystem,Keys* _keys);

	private:
	};

#define gPlayer Player::Get()
}
#endif