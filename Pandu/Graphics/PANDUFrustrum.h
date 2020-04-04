/********************************************************************
	filename: 	PANDUFrustrum
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUFrustrum_h__
#define __PANDUFrustrum_h__

//#include "../boost/smart_ptr.hpp"
#include "PANDUSceneManager.h"
#include "../Math/PANDUMatrix44.h"
#include "../Math/PANDUVector3.h"
#include "../Math/PANDUVector2.h"
#include <boost/function.hpp>
#include "PANDUOnRenderListener.h"
#include "../Utils/PANDUFlag.h"

namespace Pandu
{
	class Frustrum;

	typedef boost::shared_ptr<Frustrum> TSharedFrustrumPtr;
	typedef boost::weak_ptr<Frustrum> TFrustrumWeakPtr;
	
	class Frustrum
	{
	public:

		friend class Viewport;

		enum ProjectionType
		{
			PT_ORTHOGRAPHIC,
			PT_PERSPECTIVE
		};

		enum OperationsOnRender
		{
			OOR_None					= 0,
			OOR_ClearColourBuffer		= 1 << 0,
			OOR_ClearDepthBuffer		= 1 << 1,
			OOR_Flush					= 1 << 2,
		};

	private:

		const String				m_Name;
		const ProjectionType		m_ProjType;
		// y-direction field-of-view in radians
		float						m_FOVy ;
		float						m_TanHalfFOVy;
		// Far clip distance
		float						m_FarDist;
		// Near clip distance
		float						m_NearDist;
		// x/y viewport ratio
		float						m_Aspect;
		float						m_FocalLength;
		bool						m_DirtyProjectionParameters;
		float						m_Left;
		float						m_Right;
		float						m_Top;
		float						m_Bottom;

		bool						m_CustomProjParams;		
	    
		bool						m_DirtyProjectionMatrix;
		Matrix44					m_ProjMatrix;
		Matrix44					m_InvProjMatrix;

		// frustum center offset
		Vector2						m_FrustumOffset;
	    
		float						m_OrthoHeight;

		TOnRenderListenerWeakPtrList m_OnRenderListeners;

		Flag<unsigned int>			m_OperationsBeforeRender;
		Flag<unsigned int>			m_OperationsAfterRender;


		unsigned int				m_TechniqueIdToRenderScene;

	protected:

		TSharedSceneManagerPtr m_SceneManager;

	public:

		Frustrum(const String& _name,ProjectionType _projType,TSharedSceneManagerPtr _sceneManager)
			: m_Name(_name)
			, m_ProjType(_projType)
			, m_DirtyProjectionParameters(true)
			, m_DirtyProjectionMatrix(true)
			, m_FOVy(kPI/4)//45 degrees
			, m_TanHalfFOVy(tan(m_FOVy * 0.5f))
			, m_FarDist(100.0f)
			, m_NearDist(1.0f)
			, m_Aspect(1.3333f)
			, m_CustomProjParams(false)
			, m_FocalLength(1.0f)
			, m_FrustumOffset(Vector2::ZERO)
			, m_OrthoHeight(10)
			, m_SceneManager(_sceneManager)
			, m_TechniqueIdToRenderScene(0)
		{
		}

		virtual ~Frustrum()
		{
			m_OnRenderListeners.clear();
		}

		inline const String& GetName()	{	return m_Name;		}

		inline void SetViewportAspectRatio(float _aspectRatio)
		{
			m_Aspect = _aspectRatio;
			m_DirtyProjectionParameters = true;
		}

		inline void SetFarClipDistance(float _distance)
		{
			m_FarDist = _distance;
			m_DirtyProjectionParameters = true;
		}

		inline void SetNearClipDistance(float _distance)
		{
			m_NearDist = _distance;
			m_DirtyProjectionParameters = true;
		}

		const Matrix44& GetProjectionMatrix()
		{
			UpdateProjectionParameters();
			UpdateProjectionMatrix();
			return m_ProjMatrix;
		}

		inline const Matrix44& GetInverseProjectionMatrix()
		{
			UpdateProjectionParameters();
			UpdateProjectionMatrix();
			return m_InvProjMatrix;
		}

		inline void SetCustomProjeParams(float _left, float _bottom, float _width, float _height)
		{
			m_CustomProjParams = true;
			m_Left   = _left;
			m_Right  = _left + _width;
			m_Bottom = _bottom;
			m_Top    = _bottom + _height;
			m_DirtyProjectionParameters = false;
			m_DirtyProjectionMatrix = true;
		}

		inline void SetCustomProjectionParamsToFalse()
		{
			m_CustomProjParams = false;
			m_DirtyProjectionParameters = true;
			m_DirtyProjectionMatrix = true;
		}

		inline void AddOnRenderListener(TOnRenderListenerWeakPtr _listener)
		{
			m_OnRenderListeners.push_back(_listener);
		}

		inline void AddOperationBeforeRender(OperationsOnRender _operation)
		{
			m_OperationsBeforeRender.SetFlag(_operation);
		}

		inline void RemoveOperationBeforeRender(OperationsOnRender _operation)
		{
			m_OperationsBeforeRender.ClearFlag(_operation);
		}

		inline void RemoveAllOperationsBeforeRender()
		{
			m_OperationsBeforeRender.ResetFlag();
		}

		inline void AddOperationAfterRender(OperationsOnRender _operation)
		{
			m_OperationsAfterRender.SetFlag(_operation);
		}

		inline void RemoveOperationAfterRender(OperationsOnRender _operation)
		{
			m_OperationsAfterRender.ClearFlag(_operation);
		}

		inline void RemoveAllOperationsAfterRender()
		{
			m_OperationsAfterRender.ResetFlag();
		}

		inline void SetTechniqueIdToRenderScene( unsigned int _techniqueID )
		{
			m_TechniqueIdToRenderScene = _techniqueID;
		}

		inline float GetFOVY() const					{	return m_FOVy;						}
		inline float GetTanHalfFOVY() const				{	return m_TanHalfFOVy;				}

		virtual const Matrix44& GetViewMatrix()			{	return Matrix44::IDENTITY;			}
		virtual const Matrix44& GetInverseViewMatrix()	{	return Matrix44::IDENTITY;			}

	protected:

		void SetProjectionMatrix();

	private:

		void UpdateScene();
		virtual void OnRenderStart() = 0;
		void RenderScene();
		virtual void OnRenderEnd() = 0;
	
		void UpdateProjectionParameters();
		void UpdateProjectionMatrix();
		void LoadProjectionMatrixToGL();
		void CallListenerOnRenderFinished();

		void PerformOperationsOnRender(const Flag<unsigned int>& _operations) const;
	};

	typedef boost::function<TSharedFrustrumPtr (const String&, Frustrum::ProjectionType,TSharedSceneManagerPtr )> TCameraCreationFunction;

}

#endif