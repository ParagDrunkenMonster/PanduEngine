/********************************************************************
	filename: 	PANDUAnimationController
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUAnimationController_h__
#define __PANDUAnimationController_h__

namespace Pandu
{
	class AnimationController
	{
	public:

		enum ControllerType
		{
			eCT_Loop,
			eCT_Once,
		};

	protected:

		const ControllerType m_ControllerType;
	
	public:
	
		inline AnimationController(ControllerType _controllerType):m_ControllerType(_controllerType){}
		virtual ~AnimationController(){}

		virtual void Update(float _deltaTime) = 0;
	};
}

#endif //__PANDUAnimationController_h__