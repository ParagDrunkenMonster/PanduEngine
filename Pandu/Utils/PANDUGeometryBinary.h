/********************************************************************
	filename: 	PANDUGeometryBinary
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUGeometryBinary_h__
#define __PANDUGeometryBinary_h__

#include "PANDUBinaryDataFile.h"
#include "../Graphics/PANDUMesh.h"


#define MAX_NAME_SIZE 64 //this value always needs to be power of 2
#define MAX_NAME_STRING_LENGTH (MAX_NAME_SIZE - 1)

namespace Pandu
{
	class GeometryBinary : public BinaryDataFile
	{
	public:

		static const unsigned gGeometryBinaryMagicNumber = '\0' << 24 | 'b' << 16 | 'g' << 8 | 'p';

		enum GBSemantic
		{
			eGBS_None		= 0,
			eGBS_MESHNAME,
			eGBS_POSITION,
			eGBS_NORMAL,
			eGBS_TANGENT,
			eGBS_BINORMAL,
			eGBS_TEXCOORD0,
			eGBS_TEXCOORD1,
			eGBS_TEXCOORD2,
			eGBS_TEXCOORD3,
			eGBS_TEXCOORD4,
			eGBS_TEXCOORD5,
			eGBS_TEXCOORD6,
			eGBS_COLOR0,
			eGBS_COLOR1,
			eGBS_COLOR2,
			eGBS_COLOR3,
			eGBS_COLOR4,
			eGBS_COLOR5,
			eGBS_COLOR6,
			eGBS_INDICES,
			eGBS_SKELETONNAME,
			eGBS_BONES,
			eGBS_BONEWEIGHTS,
			eGBS_BONEINFLUENCECOUNTINDICES,
			eGBS_WEIGHTINDICES,
			eGBS_BONEINDICES,
			eGBS_INVBINDMATRIX,
			eGBS_BINDSHAPEMATRIX,

			eGBS_Count,
		};


		inline GeometryBinary(const String& _fileName)
			: BinaryDataFile(gGeometryBinaryMagicNumber,1,_fileName,"pgb")
		{
		}

		inline ~GeometryBinary()
		{
		}

		virtual bool LoadFromFile();

		TSharedMeshPtr CreateMeshWithData();

	private:

		void SetGeometryDataToMesh(TSharedMeshPtr _mesh);
	};
}

#endif