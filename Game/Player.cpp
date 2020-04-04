#include "Player.h"
#include "GameInputKeys.h"
#include "GameActorTypes.h"
#include "GLKeyInputMethod.h"
#include "GameInputBinding.h"
#include "GameComponents/PANDUActorManager.h"
#include "boost/bind.hpp"
#include "Utils/PANDUObjMeshLoader.h"
#include "Graphics/PANDUMeshInstance.h"
#include "Graphics/PANDUMeshManager.h"
#include "GameComponents/PANDUControllerManager.h"

namespace Game
{
	Player* Player::m_Instance = NULL;

	Player::Player()
		: m_Initialized(false)
		, m_MaterialSet(false)
	{
	}

	Player::~Player()
	{
		m_PlayerToken.reset();
	}

	void Player::Initialize(Pandu::TSharedSceneManagerPtr _sceneManager,Pandu::TDynamicsWorldSharedPtr _world,Pandu::TInputSystemSharedPtr _ipSystem,Keys* _keys)
	{
		if(m_Initialized)
		{
			return;
		}

		m_Initialized = true;

		m_CarVertexShader = Pandu::gShaderManager->CreateVertexProgram("../Media/BasicLighting.vert");
		m_CarPixelShader = Pandu::gShaderManager->CreateFragmentProgram("../Media/BasicLighting.frag");

		m_RenderShadowVertexProgram = Pandu::gShaderManager->CreateVertexProgram("../Media/ShadowRender.vert");
		m_RenderShadowFragmentProgram = Pandu::gShaderManager->CreateFragmentProgram("../Media/ShadowRender.frag");


		/*Pandu::TActorAutoPtr player(new Pandu::Character((unsigned int)GAT_Player,_world,100.0f,10.3f,Pandu::Vector3(-0.0f,40.0f,0.0f),Pandu::Vector3::UNIT_X));
		//Pandu::TActorAutoPtr player(new Pandu::Car(GAT_Player,m_DynamicsWorld));

		//_keyIpMethod->AddGameKeyListener(player);

		//Pandu::TActorSharedPtr playerActor = player;

		Pandu::TInputBindingAutoPtr inputBinding(new GameInputBinding());

		Pandu::TInputMethodAutoPtr keyInputMethod(new GLKeyInputMethod(inputBinding,_keys));
		Pandu::TInputMethodWeakPtr keyIPMethodWeakPtr = inputSystem->AddInputMethod(keyInputMethod);
		
		Pandu::TActorCrDstCondition crtDestCond = Pandu::TActorCrDstCondition(&Player::PlayerCharCreatDestRemoveCondition);

		m_PlayerToken = Pandu::gActorManager->AddActor(player,crtDestCond,crtDestCond,crtDestCond);

		Pandu::TActorTokenSharedPtr playerToken = m_PlayerToken.lock();
		Pandu::TActorWeakPtr actorToListener = playerToken->GetActor();

		Pandu::TInputMethodSharedPtr keyIPMethodSharedPtr = keyIPMethodWeakPtr.lock();
		//keyIPMethodSharedPtr->AddGameKeyListener(actorTokenToListener);

		m_PlayerController.reset(new GamePlayerCharacterController(actorToListener));

		(static_cast<GLKeyInputMethod*>(keyIPMethodSharedPtr.get()))->AddGameKeyListener(m_PlayerController);*/

		Pandu::ObjMeshLoader loader;

		Pandu::MeshManager::TMeshCreator creator = boost::bind(&Pandu::ObjMeshLoader::LoadMesh,&loader,Pandu::String("../Media/wheel.obj") );
		Pandu::TSharedMeshPtr wheelMesh = Pandu::gMeshManager->CreateMesh(creator);

		creator = boost::bind(&Pandu::ObjMeshLoader::LoadMesh,&loader,Pandu::String("../Media/car.obj") );
		Pandu::TSharedMeshPtr carMesh = Pandu::gMeshManager->CreateMesh(creator);



		Pandu::CarType carType;

		Pandu::TActorAutoPtr player(new Pandu::Car((unsigned int)GAT_Car,Pandu::String("PlayerCar"),_sceneManager,_world,carType));

		Pandu::Car* car = (Pandu::Car*)player.get();

		

		m_WheelMeshInst = wheelMesh->CreateMeshInstance();
		m_CarMeshInst = carMesh->CreateMeshInstance();

		car->SetWheelMesh(m_WheelMeshInst);
		car->SetChesisMesh(m_CarMeshInst);

		Pandu::TInputBindingAutoPtr inputBinding(new GameInputBinding());

		Pandu::TInputMethodAutoPtr keyInputMethod(new GLKeyInputMethod(inputBinding,_keys));
		Pandu::TInputMethodSharedPtr keyIPMethodSharedPtr = _ipSystem->AddInputMethod(keyInputMethod);
		
		Pandu::TActorCrDstCondition crtDestCond = Pandu::TActorCrDstCondition(&Player::PlayerCharCreatDestRemoveCondition);

		m_PlayerToken = Pandu::gActorManager->AddActor(player,crtDestCond,crtDestCond,crtDestCond);

		m_PlayerCharacter = m_PlayerToken->GetActor();

		

		//keyIPMethodSharedPtr->AddGameKeyListener(actorTokenToListener);

		m_CarController.reset(new GameCarController(m_PlayerCharacter));
		
		Pandu::gControllerManager->AddController(m_CarController);

		(static_cast<GLKeyInputMethod*>(keyIPMethodSharedPtr.get()))->AddGameKeyListener(m_CarController);

	}

	void Player::SetMaterialToCar(Pandu::TSharedTexturePtr _shadowMapTex)
	{
		if( m_MaterialSet )
		{
			return ;
		}

		Pandu::TSharedMaterialPtr carMat = Pandu::gMaterialManager->CreateMaterial("CarMaterial");
		carMat->AddTexture(_shadowMapTex);//0

		
		Pandu::TSharedShaderTechniquePtr technique = carMat->CreateShaderTechnique(Pandu::gShaderManager->ShaderTechniqueID("MainTechnique"));
		Pandu::TSharedShaderPassPtr pass = technique->AddShaderPass(m_CarVertexShader,m_CarPixelShader);
		
		Pandu::TSharedShaderParameterPtr textureParam = pass->CreateShaderParameter("ShadowMapSampler",Pandu::ShaderParameter::ePT_Texture,1);
		if(textureParam != NULL)
		{
			textureParam->SetValue((short)0);
		}

		Pandu::TSharedShaderParameterPtr invShadowMapSize = pass->CreateShaderParameter("InvShadowMapSize",Pandu::ShaderParameter::ePT_Float2,1);
		if(invShadowMapSize != NULL)
		{
			invShadowMapSize->SetValue(Pandu::Vector2(1.0f/(float)_shadowMapTex->GetTextureWidth(),1.0f/(float)_shadowMapTex->GetTextureHeight()));
		}

		carMat->SetMaterialColour(Pandu::Material::MCT_Ambient,Pandu::Colour(0.25f,0.25f,0.25f,1.0f));


		//---------------------------------------------------------------------------------------

		Pandu::TSharedShaderTechniquePtr shadowRenderTechnique = carMat->CreateShaderTechnique(Pandu::gShaderManager->ShaderTechniqueID("ShadowRender"));
		Pandu::TSharedShaderPassPtr shadowRenderPass = shadowRenderTechnique->AddShaderPass(m_RenderShadowVertexProgram,m_RenderShadowFragmentProgram);

		m_WheelMeshInst->SetMaterial(carMat);
		m_CarMeshInst->SetMaterial(carMat);


		m_MaterialSet = true;
	}

	//-----------------------------------------------------------------------------
	//static functions

	void Player::CreateInstance()
	{
		if(!m_Instance)
		{
			m_Instance = new Player();
		}
	}

	void Player::DestroyInstance()
	{
		if(m_Instance)
		{
			delete m_Instance;
			m_Instance = NULL;
		}
	}

	Player* Player::Get()
	{
		return m_Instance;
	}

	bool Player::PlayerCharCreatDestRemoveCondition()
	{
		return false;
	}

}

