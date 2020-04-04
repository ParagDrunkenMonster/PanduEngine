#include "PANDUTransformerManager.h"


namespace Pandu
{
	TransformerManager* TransformerManager::m_Instance = NULL;

	//--------------------------------------------------------------------
	//Static functions
	void TransformerManager::CreateInstance()
	{
		if(m_Instance == NULL)
		{
			m_Instance = new TransformerManager();
		}
	}

	void TransformerManager::DestroyInstance()
	{
		if(m_Instance != NULL)
		{
			delete m_Instance;
			m_Instance = NULL;
		}
	}

	TransformerManager* TransformerManager::Get()
	{
		return m_Instance;
	}

	//--------------------------------------------------------------------

	TransformerManager::TransformerManager()
	{
	}

	TransformerManager::~TransformerManager()
	{
		ClearAllTransformers();
	}

	void TransformerManager::ClearAllTransformers()
	{
		TTransformerPtrListItr itr = m_AllTransformers.begin();
		TTransformerPtrListItr end = m_AllTransformers.end();

		for( ; itr != end ; itr++)
		{
			PANDU_ERROR( (*itr).use_count() == 1, "Transformer is still in use, first clear the reference before delete.." );
			(*itr).reset();
		}

		m_AllTransformers.clear();
	}

	TTransformerSharedPtr TransformerManager::AddTransformer(TTransformerAutoPtr _transformer)
	{
		TTransformerSharedPtr trans(_transformer.release());
		m_AllTransformers.push_back(trans);
		//TTransformerPtrListItr itr = (m_AllTransformers.end()--);
		return trans;
	}

	void TransformerManager::DestroyTransformer(TTransformerPtrListItr _transformer)
	{
		PANDU_ERROR( (*_transformer).use_count() == 1, "Transformer is still in use, first clear the reference before delete.." );
		(*_transformer).reset();
		m_AllTransformers.erase(_transformer);
	}

	void TransformerManager::Update(const FrameEvent& _frameEvent)
	{
		TTransformerPtrListItr itr = m_AllTransformers.begin();
		TTransformerPtrListItr end = m_AllTransformers.end();

		for( ; itr != end ; itr++)
		{
			(*itr)->Update(_frameEvent);
		}
	}
}