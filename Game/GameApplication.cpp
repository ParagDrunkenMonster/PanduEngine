#include "GameApplication.h"
#include "Graphics/PANDUBasicSceneManager.h"
#include "Graphics/PANDUOverlaySceneManager.h"
#include "Graphics/PANDUTextureManager.h"
#include "Models/Teapot.h"
#include "PanduBulletPhysics/PANDUPhysics.h"
#include "Components/PANDUTransformerManager.h"
#include "Components/PANDURigidBodyToNodeTransformer.h"
#include "Debug/PANDUGLDebugDrawer.h"
#include "Components/PANDUInputMethodKeyListener.h"
#include "GameActorTypes.h"
#include "GameComponents/PANDUActorManager.h"
#include "Components/PANDUFunctionExecuterCommand.h"
#include "Graphics/PANDUOverlayFrustrum.h"
#include "boost/bind.hpp"
#include "Utils/PANDUObjMeshLoader.h"
#include "Graphics/PANDUMeshInstance.h"
#include "BasicCameraController.h"
#include "Utils/PANDUGeometryBinary.h"
#include "Utils/PANDUSkeletonBinary.h"
#include "Utils/PANDUAnimationBinary.h"
#include "Debug/PANDUPhysicsDebugObject.h"
#include "Debug/PANDUSkeletonDebugObject.h"
#include "Graphics/PANDUSkeletonMeshModifier.h"
#include "Utils/PANDUBitmapImage.h"
#include "Graphics/PANDUShadowMapCamera.h"

namespace
{
	enum TechniqueTypes
	{
		TT_MainRenderTechnique = 0,
		TT_ShadowRenderTechnique,

		TT_Count
	};
	const char* kTechninqueTypes[TT_Count] = 
									{
										"MainTechnique",//TT_MainRenderTechnique = 0,
										"RenderShadowMap"//TT_ShadowRenderTechnique,
									};

	const unsigned int kShadowCameraSize = 200;
	const unsigned int kShadowMapSize = 4096;
}

namespace Game
{
	GameApplication::GameApplication(float _width, float _height)
	: m_Width(_width)
	, m_Height(_height)
	, m_Initialized(false)
	, m_IndexVertexArrays(NULL)
	, m_WorldCollisionVertices(NULL)
	, m_WorldCollisionIndices(NULL)
	{
		Teapot pot;
		m_MainSceneManager = Pandu::gGraphicsRoot->CreateSceneManager(Pandu::BasicSceneManager::GetClassType(),Pandu::String("MainSceneManager") );

		m_ShadowRenderingViewport = Pandu::gGraphicsRoot->CreateViewport(Pandu::String("ShadowRenderingViewport"),0,0,(unsigned int)kShadowMapSize ,(unsigned int)kShadowMapSize);
		m_MainViewport = Pandu::gGraphicsRoot->CreateViewport(Pandu::String("MainViewport"),0,0,(unsigned int)_width ,(unsigned int)_height);

		m_MainLightCamera = m_ShadowRenderingViewport->CreateCamera(Pandu::ShadowMapCamera::GetCreationFunction(),Pandu::String("MainLightCamera"),Pandu::Frustrum::PT_ORTHOGRAPHIC,m_MainSceneManager);
		m_MainLightCamera->SetTechniqueIdToRenderScene(Pandu::gShaderManager->ShaderTechniqueID(kTechninqueTypes[TT_ShadowRenderTechnique]));
		m_MainLightCamera->SetCustomProjeParams(-(float)kShadowCameraSize * 0.5f,-(float)kShadowCameraSize * 0.5f,(float)kShadowCameraSize,(float)kShadowCameraSize);

		m_MainLightCamera->AddOperationBeforeRender(Pandu::Frustrum::OOR_ClearColourBuffer);
		m_MainLightCamera->AddOperationBeforeRender(Pandu::Frustrum::OOR_ClearDepthBuffer);
		m_MainLightCamera->AddOperationAfterRender(Pandu::Frustrum::OOR_Flush);

		m_MainCamera = m_MainViewport->CreateCamera(Pandu::Camera::GetCreationFunction(),Pandu::String("Test"),Pandu::Frustrum::PT_PERSPECTIVE,m_MainSceneManager);
		m_MainCamera->SetTechniqueIdToRenderScene(Pandu::gShaderManager->ShaderTechniqueID(kTechninqueTypes[TT_MainRenderTechnique]));

		m_MainCamera->AddOperationBeforeRender(Pandu::Frustrum::OOR_ClearColourBuffer);
		m_MainCamera->AddOperationBeforeRender(Pandu::Frustrum::OOR_ClearDepthBuffer);
		m_MainCamera->AddOperationAfterRender(Pandu::Frustrum::OOR_Flush);

		Pandu::Camera* cam = static_cast<Pandu::Camera*>(m_MainCamera.get());
		cam->SetPosition(Pandu::Vector3(-20,0.0,0));
		cam->SetLookAt(Pandu::Vector3(-20,-20,0));
		cam->SetUpAxis(Pandu::Vector3(0.0f,1.0f,0.0f));
		cam->SetFarClipDistance(1000);

		//SetRender Texture

		m_MainRenderFrameBuffer.reset(new Pandu::FrameBuffer());
		m_RenderDepthTexture = Pandu::gTextureManager->CreateTexture(Pandu::Texture::TT_Texture2D,"MainRenderDepthTexture",Pandu::Texture::TF_DEPTH_COMPONENT24,(unsigned int)kShadowMapSize,(unsigned int)kShadowMapSize );
		
		if( m_RenderDepthTexture != NULL )
		{
			if(m_MainRenderFrameBuffer->Load(true,m_RenderDepthTexture))
			{
				//m_RenderTexture = Pandu::gTextureManager->CreateTexture(Pandu::Texture::TT_Texture2D,"MainRenderTexture",Pandu::Texture::TF_R8G8B8A8,(unsigned int)_width,(unsigned int)_height );
				//if( m_RenderTexture != NULL && m_RenderDepthTexture != NULL )
				{
					//m_MainRenderFrameBuffer->AddColourTexture(m_RenderTexture);
					m_RenderDepthTexture->SetTextureParam(Pandu::Texture::TPN_WrapS,Pandu::Texture::TPIV_ClampToEdge);
					m_RenderDepthTexture->SetTextureParam(Pandu::Texture::TPN_WrapT,Pandu::Texture::TPIV_ClampToEdge);
					m_ShadowRenderingViewport->SetFrameBuffer(m_MainRenderFrameBuffer);
				}
			}
			else
			{
				m_MainRenderFrameBuffer.reset();
			}
		}
		else
		{
			m_MainRenderFrameBuffer.reset();
		}
		//-----------------

		//test.InitPhysics();

		m_DynamicsWorld = Pandu::gPhysics->CreateWorld(Pandu::String("TestWorld"),Pandu::WT_DiscreteDynamicWorld);

		m_DynamicsWorld->SetGravity(btVector3(0,-9.8f,0));

#ifdef DEBUG_DRAW
		InitializeDebugScene();
#endif


		LoadLevel();

		m_DynamicsWorld->ClientResetScene();

		
	}

	GameApplication::~GameApplication()
	{

#ifdef DEBUG_DRAW
		DestroyDebugScene();
#endif

		m_NormalMapVertexProgram.reset();
		m_NormalMapFragmentProgram.reset();

		delete m_CameraController;
		m_CameraController = NULL;

		Player::DestroyInstance();

		if(m_WorldCollisionVertices)
		{
			delete [] m_WorldCollisionVertices;
			m_WorldCollisionVertices = NULL;
		}
		if( m_WorldCollisionIndices )
		{
			delete [] m_WorldCollisionIndices;
			m_WorldCollisionIndices = NULL;
		}

		if(m_IndexVertexArrays)
		{
			delete m_IndexVertexArrays;
			m_IndexVertexArrays = NULL;
		}
	}

	void GameApplication::InitializeGame(Keys* _keys)
	{
		if(!m_Initialized)
		{
			Pandu::TInputSystemAutoPtr inputSystem(new Pandu::DefaultInputSystem());
			
			m_InputSystem = Pandu::gInputManager->RegisterInputSystem(inputSystem);

			m_Initialized = true;

			CreatePlayer(m_InputSystem,_keys);

			InitializeHUD();

			m_CameraController = new BasicCameraController(m_MainCamera);
			m_CameraController->SetFollowActor(gPlayer->GetActor());


			m_DynamicsWorld->ClientResetScene();
		}
	}

	void GameApplication::CreatePlayer(Pandu::TInputSystemSharedPtr _keyIpSystem, Keys* _keys)
	{
		Pandu::TriggerDesc desc;

		desc.collisionShape.width = 10.0f;
		desc.collisionShape.height = 10.0f;
		desc.collisionShape.depth = 10.0f;
		desc.transform.setOrigin(btVector3(0,8,0));
		//desc.transform.setRotation(btQuaternion(btVector3(0,0,1.0f),3.1415926535897932384626433832795f/9.0f) );
		
		/*Pandu::ActorEnterExitTrigger *rawTrigger = new Pandu::ActorEnterExitTrigger(desc,m_PlayerToken);
		//rawTrigger->
		Pandu::TCommandAutoPtr weakEnterCommand(new Pandu::FunctionExecuterCommand(boost::bind(&GameApplication::PlayerEnterTrigger,this,m_PlayerToken)) );

		rawTrigger->AddCommand(weakEnterCommand,Pandu::ActorEnterExitTrigger::ET_OnEnter);

		Pandu::TTriggerAutoPtr autoTrigger(rawTrigger);
		m_DynamicsWorld->AddTrigger(autoTrigger);*/

		//Initialize(Pandu::TDynamicsWorldSharedPtr _world,Pandu::TInputSystemWeakPtr _ipSystem,Keys* _keys);
		Player::CreateInstance();
		gPlayer->Initialize(m_MainSceneManager,m_DynamicsWorld,_keyIpSystem,_keys);
		gPlayer->SetMaterialToCar(m_RenderDepthTexture);

	}

	void GameApplication::InitializeHUD()
	{
		Pandu::TSharedViewportPtr hudViewport = Pandu::gGraphicsRoot->CreateViewport(Pandu::String("OverlayViewport"),0,0,(unsigned int)m_Width,(unsigned int)m_Height);
		Pandu::TSharedSceneManagerPtr scnMgr = Pandu::gGraphicsRoot->CreateSceneManager(Pandu::OverlaySceneManager::GetClassType(),Pandu::String("OverlaySceneManager") );
		Pandu::TSharedSceneNodePtr node = scnMgr->CreateChildSceneNodeOfNode(scnMgr->GetRootSceneNode(),Pandu::String("ScreenNode") );

		Pandu::TOverlayAutoPtr autoOverlay(new Pandu::OverlayScreen(Pandu::String("Bckground") ,Pandu::Rect( 0, 0 , 200,200 ) ));
		Pandu::TOverlaySharedPtr overlay = (static_cast<Pandu::OverlaySceneManager*>(scnMgr.get()))->AddOverlay(autoOverlay);
		node->AddRenderable(overlay);

		Pandu::TSharedFrustrumPtr frustrum = hudViewport->CreateCamera(Pandu::OverlayFrustrum::GetCreationFunction(),Pandu::String("OverlayCamera"),Pandu::Frustrum::PT_ORTHOGRAPHIC,scnMgr);
		frustrum->SetCustomProjeParams(0,0,m_Width,m_Height);
		frustrum->AddOperationBeforeRender(Pandu::Frustrum::OOR_ClearDepthBuffer);
		//frustrum->AddOperationBeforeRender(Pandu::Frustrum::OOR_ClearColourBuffer);
		frustrum->AddOperationAfterRender(Pandu::Frustrum::OOR_Flush);
		/*Pandu::Camera* cam = static_cast<Pandu::Camera*>(frustrum.get());
		cam->SetPosition(Pandu::Vector3(0,30,200));
		cam->SetLookAt(Pandu::Vector3(0,30,0));
		cam->SetFarClipDistance(1000);*/

		
		Pandu::TSharedMaterialPtr mtrl = Pandu::gMaterialManager->CreateMaterial("TestHudMaterial");
		mtrl->SetLightingEnabled(false);
		mtrl->AddTexture(m_RenderDepthTexture);
		overlay->SetMaterial(mtrl);
		mtrl->SetMaterialColour(Pandu::Material::MCT_Ambient,Pandu::Colour(1.0f,1.0f,1.0f,1.0f));
	}	

	void GameApplication::WindowSizeChanged(float _newWidth, float _newHeight)
	{
	}

	//called before update call
	//if returns false application will quit...
	bool GameApplication::PreUpdate()
	{
		return true;
	}

	//Called before render call
	void GameApplication::Update(const Pandu::FrameEvent& _event)
	{	
		if( m_TestAnimBlender != NULL )
		{
			m_TestAnimBlender->Update(_event.deltaTimeSecond);
		}

		if( m_TestAnimBlender1 != NULL )
		{
			m_TestAnimBlender1->Update(_event.deltaTimeSecond * 2.0f);
		}
		
		m_CameraController->Update(_event);
		UpdateShadowCamBasedOnMainCam();
	}

	void GameApplication::PlayerEnterTrigger(Pandu::TActorTokenWeakPtr _player)
	{
		int i = 0;
		i++;
	}

	void GameApplication::LoadNormalMapShaders()
	{
		m_NormalMapVertexProgram = Pandu::gShaderManager->CreateVertexProgram("../Media/NormalMap.vert");
		m_NormalMapFragmentProgram = Pandu::gShaderManager->CreateFragmentProgram("../Media/NormalMap.frag");
	}

	void GameApplication::LoadShadowShaders()
	{
		m_RenderShadowVertexProgram = Pandu::gShaderManager->CreateVertexProgram("../Media/ShadowRender.vert");
		m_RenderShadowFragmentProgram = Pandu::gShaderManager->CreateFragmentProgram("../Media/ShadowRender.frag");
	}

	Pandu::TSharedMeshPtr GameApplication::CreateMesh(const Pandu::String& _geometryBinaryPath)
	{
		Pandu::GeometryBinary geometryBinary(_geometryBinaryPath);
		Pandu::MeshManager::TMeshCreator creator = boost::bind(&Pandu::GeometryBinary::CreateMeshWithData,&geometryBinary );
		Pandu::TSharedMeshPtr mesh = Pandu::gMeshManager->CreateMesh(creator);
		return mesh;
	}

	Pandu::TSharedMaterialPtr GameApplication::CreateCityObjectMaterial(const Pandu::String& _matName, const Pandu::String& _normalMapPath, const Pandu::String& _parallexMapPath, const Pandu::String& _occlusionMapPath)
	{
		Pandu::TSharedMaterialPtr normalMapMaterial = Pandu::gMaterialManager->CreateMaterial(_matName);

		Pandu::TSharedTexturePtr tex = Pandu::gTextureManager->CreateTexture(Pandu::Texture::TT_Texture2D,_normalMapPath);
		normalMapMaterial->AddTexture(tex);//0

		tex = Pandu::gTextureManager->CreateTexture(Pandu::Texture::TT_Texture2D,_parallexMapPath);
		normalMapMaterial->AddTexture(tex);//1

		tex = Pandu::gTextureManager->CreateTexture(Pandu::Texture::TT_Texture2D,_occlusionMapPath);
		normalMapMaterial->AddTexture(tex);//2

		normalMapMaterial->AddTexture(m_RenderDepthTexture);//3


		Pandu::TSharedShaderTechniquePtr technique = normalMapMaterial->CreateShaderTechnique(Pandu::gShaderManager->ShaderTechniqueID(kTechninqueTypes[TT_MainRenderTechnique]));
		Pandu::TSharedShaderPassPtr pass = technique->AddShaderPass(m_NormalMapVertexProgram,m_NormalMapFragmentProgram);
		Pandu::TSharedShaderParameterPtr parameter = pass->CreateShaderParameter("NormalMapSampler",Pandu::ShaderParameter::ePT_Texture,1);
		if(parameter != NULL)
		{
			parameter->SetValue((short)0);
		}

		Pandu::TSharedShaderParameterPtr parameter1 = pass->CreateShaderParameter("ParallaxMapSampler",Pandu::ShaderParameter::ePT_Texture,1);
		if(parameter1 != NULL)
		{
			parameter1->SetValue((short)1);
		}

		Pandu::TSharedShaderParameterPtr parameter2 = pass->CreateShaderParameter("OcclusionMapSampler",Pandu::ShaderParameter::ePT_Texture,1);
		if(parameter2 != NULL)
		{
			parameter2->SetValue((short)2);
		}

		Pandu::TSharedShaderParameterPtr parameter3 = pass->CreateShaderParameter("ShadowMapSampler",Pandu::ShaderParameter::ePT_Texture,1);
		if(parameter3 != NULL)
		{
			parameter3->SetValue((short)3);
		}

		Pandu::TSharedShaderParameterPtr parameter4 = pass->CreateShaderParameter("ParallaxScale",Pandu::ShaderParameter::ePT_Float,1);
		if(parameter4 != NULL)
		{
			parameter4->SetValue(0.04f);
		}

		Pandu::TSharedShaderParameterPtr parameter5 = pass->CreateShaderParameter("ParallaxBias",Pandu::ShaderParameter::ePT_Float,1);
		if(parameter5 != NULL)
		{
			parameter5->SetValue(0.03f);
		}

		Pandu::TSharedShaderAttributePtr attrb = pass->CreateShaderAttributeForBuffer(Pandu::VertexBuffer::eBT_Tangent,Pandu::ShaderAttribute::eALA_Location0);
		attrb = pass->CreateShaderAttributeForBuffer(Pandu::VertexBuffer::eBT_Binormal,Pandu::ShaderAttribute::eALA_Location1);

		normalMapMaterial->SetMaterialColour(Pandu::Material::MCT_Ambient,Pandu::Colour(0.1f,0.1f,0.1f,1.0f));


		//---------------------------------------------------------------------------------------

		Pandu::TSharedShaderTechniquePtr shadowRenderTechnique = normalMapMaterial->CreateShaderTechnique(Pandu::gShaderManager->ShaderTechniqueID("ShadowRender"));
		Pandu::TSharedShaderPassPtr shadowRenderPass = shadowRenderTechnique->AddShaderPass(m_RenderShadowVertexProgram,m_RenderShadowFragmentProgram);
		
		return normalMapMaterial;
	}

	void GameApplication::LoadLevel()
	{
		
		LoadNormalMapShaders();
		LoadShadowShaders();
		LoadCity();

		

		//test pandu binary loader
		{
			Pandu::SkeletonBinary sklBinary("../Media/bin/Skeleton/goblin_max.psb");
			bool loaded = sklBinary.LoadFromFile();

			Pandu::TSharedSkeletonPtr skeleton;

			if(loaded)
			{
				skeleton = sklBinary.CreateSkeletonWithData();
				Pandu::AnimationBinary animBinary("../Media/bin/Animation/Goblin.pab");
				bool animLoaded = animBinary.LoadFromFile();
				int i = 0;
				i++;
			}

			bool shaderLoaded = false;
			Pandu::TSharedVertexProgramPtr vp;
			Pandu::TSharedFragmentProgramPtr fp;
			//load shaders
			{
				vp = Pandu::gShaderManager->CreateVertexProgram("../Media/vertexshader.vert");
				fp = Pandu::gShaderManager->CreateFragmentProgram("../Media/fragmentshader.frag");
			
				shaderLoaded = true;
			}
		
			Pandu::GeometryBinary geometryBinary("../Media/bin/objgoblin_1.pgb");

			Pandu::MeshManager::TMeshCreator creator = boost::bind(&Pandu::GeometryBinary::CreateMeshWithData,&geometryBinary );
			Pandu::TSharedSceneNodePtr node = m_MainSceneManager->CreateChildSceneNodeOfNode(m_MainSceneManager->GetRootSceneNode(),Pandu::String("BinaryDataTest") );

			Pandu::SkeletonMeshModifier* sklModifier = new Pandu::SkeletonMeshModifier();
			Pandu::TMeshModifierAutoPtr meshModifier(sklModifier);


			//Pandu::gLogManager->WriteErrorLog("########## TEST...1");

			Pandu::TSharedMaterialPtr mtrl = Pandu::gMaterialManager->CreateMaterial("Test");
			if(shaderLoaded)
			{
				Pandu::TSharedShaderTechniquePtr technique = mtrl->CreateShaderTechnique(Pandu::gShaderManager->ShaderTechniqueID(kTechninqueTypes[TT_MainRenderTechnique]));
				Pandu::TSharedShaderPassPtr pass = technique->AddShaderPass(vp,fp);
				Pandu::TSharedShaderParameterPtr parameter = pass->CreateShaderParameter("scaleValue",Pandu::ShaderParameter::ePT_Float,1);
				parameter->SetValue(1.0f);
			}

			//Pandu::gLogManager->WriteErrorLog("########## TEST...2");
			//mtrl->SetPolygonFillMode(Pandu::Material::PFM_Line);
			Pandu::TSharedMeshPtr mesh = Pandu::gMeshManager->CreateMesh(creator);
			Pandu::TSharedMeshInstancePtr meshInstance = mesh->CreateMeshInstance();
			meshInstance->AddModifier(meshModifier);
			meshInstance->SetMaterial(mtrl);
			node->AddRenderable(meshInstance);
			node->SetPosition(Pandu::Vector3(0.0f,6.0f,0.0f));

			m_TestAnimBlender.reset(new Pandu::SkeletalAnimationBlender(sklModifier->GetSkeletonInstance()));
			m_TestAnimBlender->SetAnimation(Pandu::TSkeletalAnimationControllerAutoPtr(new Pandu::SkeletalAnimationController(Pandu::AnimationController::eCT_Loop,"Goblin")));

			{

				Pandu::TSharedSceneNodePtr node = m_MainSceneManager->CreateChildSceneNodeOfNode(m_MainSceneManager->GetRootSceneNode(),Pandu::String("TestAnimNode") );

				Pandu::SkeletonMeshModifier* sklModifier = new Pandu::SkeletonMeshModifier();
				Pandu::TMeshModifierAutoPtr meshModifier(sklModifier);

				Pandu::TSharedMeshInstancePtr meshInstance = mesh->CreateMeshInstance();
				meshInstance->AddModifier(meshModifier);
				//meshInstance->SetMaterial(mtrl);
				node->AddRenderable(meshInstance);
				node->SetPosition(Pandu::Vector3(4.0f,6.0f,0.0f));

				m_TestAnimBlender1.reset(new Pandu::SkeletalAnimationBlender(sklModifier->GetSkeletonInstance()));
				m_TestAnimBlender1->SetAnimation(Pandu::TSkeletalAnimationControllerAutoPtr(new Pandu::SkeletalAnimationController(Pandu::AnimationController::eCT_Loop,"Goblin")));
				//m_TestAnimBlender1->Update(1.0f/10.0f);
			}

#ifdef DEBUG_DRAW
			if( skeleton && m_DebugScene != NULL )
			{
				m_DebugScene->AddDebugObject(Pandu::TDebugObjectAutoPtr(new Pandu::SkeletonDebugObject(sklModifier->GetSkeletonInstance())) );
			}
#endif
		}

	}

	void GameApplication::LoadCity()
	{	
		Pandu::TSharedSceneNodePtr lightnode = m_MainSceneManager->CreateChildSceneNodeOfNode(m_MainSceneManager->GetRootSceneNode(),Pandu::String("LevelLightNode") );
		m_MainLight = Pandu::gLightManager->CreateLight(Pandu::String("TestLight") );
		lightnode->AddLight(m_MainLight);
		m_MainLight->SetPosition(Pandu::Vector3(0.0f,-100.0f,0.0f));

		Pandu::Camera* cam = static_cast<Pandu::Camera*>(m_MainLightCamera.get());

		cam->SetPosition(Pandu::Vector3::ZERO + m_MainLight->GetLightDirection() * 500.0f );
		cam->SetLookAt(Pandu::Vector3::ZERO);
		cam->SetNearClipDistance(100);
		cam->SetFarClipDistance(1000);
		cam->GetProjectionMatrix();
		cam->GetViewMatrix();
		//light->SetLightType(Pandu::Light::LT_Point);

		LoadCityPart("Street","../Media/bin/RoadShape.pgb","../Media/BrickWall_NRM.bmp","../Media/BrickWall_DISP.bmp","../Media/BrickWall_OCC.bmp");
		LoadCityPart("Boundry","../Media/bin/BoundryShape.pgb","../Media/Concrete_NRM.bmp","../Media/Concrete_DISP.bmp","../Media/Concrete_OCC.bmp");
		LoadCityPart("Platform","../Media/bin/PlatformShape.pgb","../Media/Concrete_NRM.bmp","../Media/Concrete_DISP.bmp","../Media/Concrete_OCC.bmp");
		LoadCityPart("Building","../Media/bin/CityShape.pgb","../Media/ConcreteWall_NRM.bmp","../Media/ConcreteWall_DISP.bmp","../Media/ConcreteWall_OCC.bmp");

		Pandu::TBulletRigidBodySharedPtr body = LoadLevelCollision();
		//Pandu::TTransformerAutoPtr trans(new Pandu::RigidBodyToNodeTransformer(body,node));
		//Pandu::gTransformerManager->AddTransformer(trans);
	}

	void GameApplication::UpdateShadowCamBasedOnMainCam()
	{
		//m_MainLightCamera->AddOperationAfterRender(Pandu::Frustrum::OOR_Flush);

		//m_MainCamera = m_MainViewport->CreateCamera(Pandu::Camera::GetCreationFunction(),Pandu::String("Test"),Pandu::Frustrum::PT_PERSPECTIVE,m_MainSceneManager);
		Pandu::Camera* cam = static_cast<Pandu::Camera*>(m_MainCamera.get());
		Pandu::Camera* shadowCam = static_cast<Pandu::Camera*>(m_MainLightCamera.get());

		const Pandu::Vector3& camPos = cam->GetPosition();
		const Pandu::Vector3& camDir = cam->GetDirection();
		const Pandu::Vector3& lightDir = m_MainLight->GetLightDirection();

		Pandu::Vector3 camHorzDir(camDir.x,0.0f,camDir.y);
		camHorzDir.Normalize();

		const Pandu::Vector3 lookAtPos = camPos ;//- camHorzDir * ( kShadowCameraSize / 2.0f);
		const float shadowCamDistFromCam = 500.0f;
		shadowCam->SetLookAt(lookAtPos);
		shadowCam->SetPosition(lookAtPos + lightDir * shadowCamDistFromCam);

	}

	void GameApplication::LoadCityPart(const Pandu::String& _partName, const Pandu::String& _pgbPath, const Pandu::String& _normalMapPath, const Pandu::String& _parallexMapPath, const Pandu::String& _occlusionMapPath)
	{
		Pandu::TSharedMaterialPtr material = CreateCityObjectMaterial(_partName + "_material",_normalMapPath,_parallexMapPath,_occlusionMapPath);
		//streetMat->SetMaterialColour(Pandu::Material::MCT_Specular,Pandu::Colour(1.0f,1.0f,1.0f,1.0f));
		//streetMat->SetMaterialShininess(60.0f);
		Pandu::TSharedSceneNodePtr node = m_MainSceneManager->CreateChildSceneNodeOfNode(m_MainSceneManager->GetRootSceneNode(),_partName + "_node" );

		Pandu::TSharedMeshPtr mesh = CreateMesh(_pgbPath);
		Pandu::TSharedMeshInstancePtr meshInst = mesh->CreateMeshInstance();
		node->AddRenderable(meshInst);

		meshInst->SetMaterial(material);

		//just for now a hack
		if(_partName == "Boundry")
		{
			material->SetEnableCullFace(false);
		}
	}

	Pandu::TBulletRigidBodySharedPtr GameApplication::LoadLevelCollision()
	{
		int vertStride = 3*sizeof(float);
		int indexStride = 3*sizeof(int);

		unsigned int vertexBufferCount,indexBufferCount;

		Pandu::ObjMeshLoader loader;

		loader.LoadMesh(Pandu::String("../Media/CityCollision.obj"),m_WorldCollisionVertices,vertexBufferCount,m_WorldCollisionIndices,indexBufferCount);

		unsigned int triangleCount = indexBufferCount/3;
		unsigned int verticesCount = vertexBufferCount/3;


		m_IndexVertexArrays = new btTriangleIndexVertexArray(triangleCount,
									(int*)m_WorldCollisionIndices,indexStride,verticesCount,m_WorldCollisionVertices,vertStride);

		bool useQuantizedAabbCompression = true;

		btVector3 aabbMin(-1000,-1000,-1000),aabbMax(1000,1000,1000);

		btBvhTriangleMeshShape *trimeshShape  = new btBvhTriangleMeshShape(m_IndexVertexArrays,useQuantizedAabbCompression,aabbMin,aabbMax);
	
/*		int maxSerializeBufferSize = 1024*1024*5;
		btDefaultSerializer*	serializer = new btDefaultSerializer(maxSerializeBufferSize);
		serializer->startSerialization();
#ifdef SERIALIZE_SHAPE
		trimeshShape->serializeSingleShape(serializer);
#else
		trimeshShape->serializeSingleBvh(serializer);
#endif

		serializer->finishSerialization();
		FILE* f2 = fopen("../Media/myShape.bullet","wb");
		fwrite(serializer->getBufferPointer(),serializer->getCurrentBufferSize(),1,f2);
		fclose(f2);*/

		Pandu::RigidBodyDesc desc;
		desc.mass = 0.0f;//static;
		desc.collisionShape.type = Pandu::CS_None;
		desc.collisionShape.shape = trimeshShape;
		desc.transform.setOrigin(btVector3(0,-0,0));

		Pandu::TBulletRigidBodySharedPtr body = m_DynamicsWorld->CreateCollisionBody(desc);


		body.get()->setCollisionFlags(body.get()->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);//STATIC_OBJECT);

		//enable custom material callback
		//staticBody->setCollisionFlags(staticBody->getCollisionFlags()  | btCollisionObject::CF_CUSTOM_MATERIAL_CALLBACK);

		return body;
	}

#ifdef DEBUG_DRAW
	void GameApplication::InitializeDebugScene()
	{
		//m_DebugScene.reset(new Pandu::DebugScene(Pandu::TDebugDrawerAutoPtr(new Pandu::GLDebugDrawer()),m_MainCamera) );
		//m_MainCamera->AddOnRenderListener(m_DebugScene);
		//m_DebugScene->AddDebugObject(Pandu::TDebugObjectAutoPtr(new Pandu::PhysicsDebugObject(m_DynamicsWorld)));	
	}

	void GameApplication::DestroyDebugScene()
	{
		PANDU_ERROR(m_DebugScene.use_count() <= 1, "Debug scene is already being used by another object, please clear all references before deleting" );
		m_DebugScene.reset();
	}
#endif
}