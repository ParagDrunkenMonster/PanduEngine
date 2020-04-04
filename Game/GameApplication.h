/********************************************************************
	filename: 	GameApplication
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __GameApplication_h__
#define __GameApplication_h__

#include "PANDUApplication.h"
#include "PanduBulletPhysics/PANDUPhysics.h"
#include "GameComponents/PANDUActorOrToken.h"
#include "Components/PANDUInputMethodKey.h"
#include "Graphics/PANDUGraphicsRoot.h"
#include "Components/PANDUInputManager.h"
#include "Player.h"
#include "Debug/PANDUDebugScene.h"
#include "Graphics/PANDUSkeleton.h"
#include "Graphics/PANDUSkeletalAnimationBlender.h"
#include "Graphics/PANDUMeshManager.h"
#include "Graphics/PANDULightManager.h"


namespace Game
{
	class BasicCameraController;

	class GameApplication : public Pandu::Application
	{
	private:

		float m_Width;
		float m_Height;
		Pandu::TDynamicsWorldSharedPtr m_DynamicsWorld;

		Pandu::TInputSystemSharedPtr m_InputSystem;
		Pandu::TSharedViewportPtr m_ShadowRenderingViewport;
		Pandu::TSharedViewportPtr m_MainViewport;

		Pandu::TSharedSceneManagerPtr m_MainSceneManager;
		
		bool m_Initialized;

		Pandu::TSharedFrustrumPtr m_MainCamera;
		Pandu::TSharedFrustrumPtr m_MainLightCamera;
		BasicCameraController *m_CameraController;
		Pandu::TSharedLightPtr	m_MainLight;

		float* m_WorldCollisionVertices;
		unsigned int *m_WorldCollisionIndices;
		btTriangleIndexVertexArray* m_IndexVertexArrays;

		Pandu::TSharedVertexProgramPtr m_NormalMapVertexProgram;
		Pandu::TSharedFragmentProgramPtr m_NormalMapFragmentProgram;

		Pandu::TSharedVertexProgramPtr m_RenderShadowVertexProgram;
		Pandu::TSharedFragmentProgramPtr m_RenderShadowFragmentProgram;

		Pandu::TSkeletalAnimationBlenderSharedPtr m_TestAnimBlender;
		Pandu::TSkeletalAnimationBlenderSharedPtr m_TestAnimBlender1;

		Pandu::TSharedTexturePtr m_RenderTexture;
		Pandu::TSharedTexturePtr m_RenderDepthTexture;
		Pandu::TSharedFrameBufferPtr m_MainRenderFrameBuffer;

#ifdef DEBUG_DRAW
		Pandu::TSharedDebugScenePtr m_DebugScene;
#endif

	public:

		GameApplication(float _width, float _height);
		virtual ~GameApplication();

		virtual void WindowSizeChanged(float _newWidth, float _newHeight);

		//called before update call
		//if returns false application will quit...
		virtual bool PreUpdate();

		//Called before render call
		virtual void Update(const Pandu::FrameEvent& _event);

		void InitializeGame(Keys* _keys);

		void PlayerEnterTrigger(Pandu::TActorTokenWeakPtr _player);

	private:

		void CreatePlayer(Pandu::TInputSystemSharedPtr _keyIpSystem, Keys* _keys);

		void InitializeHUD();

		void LoadNormalMapShaders();
		void LoadShadowShaders();
		Pandu::TSharedMaterialPtr CreateCityObjectMaterial(const Pandu::String& _matName, const Pandu::String& _normalMapPath, const Pandu::String& _parallexMapPath, const Pandu::String& _occlusionMapPath);
		Pandu::TSharedMeshPtr CreateMesh(const Pandu::String& _geometryBinaryPath);
		void LoadLevel();

		void LoadCityPart(const Pandu::String& _partName, const Pandu::String& _pgbPath, const Pandu::String& _normalMapPath, const Pandu::String& _parallexMapPath, const Pandu::String& _occlusionMapPath);
		void LoadCity();
		Pandu::TBulletRigidBodySharedPtr LoadLevelCollision();

		void InitializeTestSkeleton();

		void InitTestNormalMapping();

		void UpdateShadowCamBasedOnMainCam();

#ifdef DEBUG_DRAW
		void InitializeDebugScene();
		void DestroyDebugScene();
#endif
	};
}

#endif