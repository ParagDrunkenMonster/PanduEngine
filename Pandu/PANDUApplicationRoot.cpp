#include "PANDUApplicationRoot.h"
#include "Graphics/PANDUGraphicsRoot.h"
#include "PanduBulletPhysics/PANDUPhysics.h"
#include "PanduBulletPhysics/PANDUClock.h"
#include "Components/PANDUTransformerManager.h"
#include "GameComponents/PANDUActorManager.h"
#include "GameComponents/PANDUControllerManager.h"
#include "Components/PANDUInputManager.h"
#include "Graphics/PANDUShaderManager.h"

namespace Pandu
{
	//===============================================================================
	//ApplicationRoot Implementation

	class ApplicationRoot::Impl
	{
	public:	

		TSharedApplicationPtr m_ApplicationPtr;
		Clock m_Clock;

		Impl()
		{
			LogManager::CreateInstance();
			ShaderManager::CreateInstance();
			GraphicsRoot::CreateInstance();
			Physics::CreateInstance();
			TransformerManager::CreateInstance();
			ActorManager::CreateInstance();
			ControllerManager::CreateInstance();
			InputManager::CreateInstance();
			m_Clock.Reset();
		}

		~Impl()
		{
			m_ApplicationPtr.reset();
			InputManager::DestroyInstance();
			ControllerManager::DestroyInstance();
			ActorManager::DestroyInstance();
			TransformerManager::DestroyInstance();
			Physics::DestroyInstance();
			GraphicsRoot::DestroyInstance();
			ShaderManager::DestroyInstance();
			LogManager::DestroyInstance();
			
		}

		inline void SetApplicationPtr(TApplicationAutoPtr application)
		{
			m_ApplicationPtr.reset(application.release());
		}

		inline bool Update()
		{
			if(m_ApplicationPtr != NULL)
			{
				FrameEvent fEvent;
				bool keepRunning = m_ApplicationPtr->PreUpdate();
				if(keepRunning)
				{
					float dt = (float)m_Clock.GetTimeMicroseconds() / 1000000.0f;
					m_Clock.Reset();

					fEvent.deltaTimeSecond = dt;
					
					gInputManager->Update(fEvent);
					gControllerManager->Update(fEvent);
					m_ApplicationPtr->Update(fEvent);
					gActorManager->Update(fEvent);
					gPhysics->StepSimulation(fEvent.deltaTimeSecond);
					gTransformerManager->Update(fEvent);
					gGraphicsRoot->RenderOneFrame();
				}
				return keepRunning;
			}

			return false;
		}
	};

	//===============================================================================

	ApplicationRoot* ApplicationRoot::m_Instance = NULL;

	//--------------------------------------------------------------------
	//Static functions
	void ApplicationRoot::CreateInstance()
	{
		if(m_Instance == NULL)
		{
			m_Instance = new ApplicationRoot();
		}
	}

	void ApplicationRoot::DestroyInstance()
	{
		if(m_Instance != NULL)
		{
			delete m_Instance;
			m_Instance = NULL;
		}
	}

	ApplicationRoot* ApplicationRoot::Get()
	{
		return m_Instance;
	}

	//--------------------------------------------------------------------

	ApplicationRoot::ApplicationRoot()
		: m_Impl(new Impl())
	{
	}

	ApplicationRoot::~ApplicationRoot()
	{
	}

	//It will release the previous application
	void ApplicationRoot::SetApplicationPtr(TApplicationAutoPtr application)
	{
		m_Impl->SetApplicationPtr(application);
	}

	bool ApplicationRoot::Update()
	{
		return m_Impl->Update();
	}
}