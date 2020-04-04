/********************************************************************
	filename: 	PANDUAnimationBinary
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUAnimationBinary_h__
#define __PANDUAnimationBinary_h__

#include "PANDUBinaryDataFile.h"
#include "Graphics/PANDUSkeletonManager.h"

namespace Pandu
{
	class AnimationBinary : public BinaryDataFile
	{
	public:

		static const unsigned int gAnimationBinaryMagicNumber = '\0' << 24 | 'b' << 16 | 'a' << 8 | 'p';
	
		enum ABSemantic
		{
			eABS_None		= 0,
			eABS_SKELETON_NAME,
			eABS_ANIM_NAME,
			eABS_ANIM_TIME_DATA,
			eABS_BONE_ANIM_TRANSFORM_DATA,
			eABS_BONE_ANIM_INTERPOLATE_DATA,
			eABS_Count,
		};

		AnimationBinary(const String& _fileName)
			: BinaryDataFile(gAnimationBinaryMagicNumber,1,_fileName,String("pab"))
		{
		}

		~AnimationBinary()
		{

		}

		virtual bool LoadFromFile();

	private:

		void LoadCreateAnimationDataToAnimation(TSharedSkeletalAnimationPtr _animation, TSharedSkeletonPtr _skeleton);
		void LoadAnimationFramesForBone(TSharedSkeletalAnimationPtr _animation, unsigned int _boneIndex
			, const BFileData::DataBlock& _transformData, const BFileData::DataBlock& _interpolateData);
	};
}

#endif //__PANDUAnimationBinary_h__