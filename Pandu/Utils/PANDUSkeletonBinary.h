/********************************************************************
	filename: 	PANDUSkeletonBinary
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUSkeletonBinary_h__
#define __PANDUSkeletonBinary_h__

#include "PANDUBinaryDataFile.h"
#include "../Graphics/PANDUSkeletonManager.h"


namespace Pandu
{
	class SkeletonBinary : public BinaryDataFile
	{
	public:

		static const unsigned int gSkeletonBinaryMagicNumber = '\0' << 24 | 'b' << 16 | 's' << 8 | 'p';

		enum SBSemantic
		{
			eSBS_None		= 0,
			eSBS_SKELETONNAME,
			eSBS_BONENAME,
			eSBS_TRANSFORM,
			eSBS_CHILDCOUNT,
			eSBS_CHILD,
			eSBS_Count,
		};

		inline SkeletonBinary(const String& _fileName)
			: BinaryDataFile(gSkeletonBinaryMagicNumber,1,_fileName,String("psb"))
		{
		}

		inline ~SkeletonBinary()
		{
		}

		virtual bool LoadFromFile();
		
		TSharedSkeletonPtr CreateSkeletonWithData();

	private:

		void SetSkeletonDataToSkeleton(TSharedSkeletonPtr _skeleton);
	};
}

#endif