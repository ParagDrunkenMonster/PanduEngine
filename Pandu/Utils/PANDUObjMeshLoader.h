/********************************************************************
	filename: 	PANDUObjMeshLoader
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUObjMeshLoader_h__
#define __PANDUObjMeshLoader_h__

#include "../Graphics/PANDUMesh.h"
#include "PANDUCharacterUtils.h"

namespace Pandu
{
	// model is always suppose to have vertex/texcoord/normals
	class ObjMeshLoader
	{
	public:

		typedef std::vector<float> TFloatArray;
		typedef std::vector<unsigned int> TUIntArray;
		
		enum ObjFileCommands
		{
			OFC_Vertex		= 0,
			OFC_Normal,
			OFC_TexCoord0,
			OFC_Face,

			OFC_Count
		};

	private:

		TFloatArray m_Vertices;
		TFloatArray m_Normals;
		TFloatArray m_TexCoord0;
		TUIntArray m_VertexIndices;
		TUIntArray m_NormalIndices;
		TUIntArray m_TexCoord0Indices;

	public:

		ObjMeshLoader();
		~ObjMeshLoader();

		TSharedMeshPtr LoadMesh(const String& _meshFileName);
		bool LoadMesh(const String& _meshFileName,float*& _oVertices,unsigned int& oVertCount,
						unsigned int*& _oIndices,unsigned int& oIndsCount);

	private:

		typedef void (ObjMeshLoader::*CommandFunction)(TCharTokenizerItr,TCharTokenizerItr);

		bool LoadMeshFile(const String& _meshFileName);

		void SetBuffers(TSharedMeshPtr _mesh);
		void AlignBuffersWithTexcoords(TSharedMeshPtr _mesh);
		void AlignBuffersWithVertices(TSharedMeshPtr _mesh);
		void AlignBuffersWithNormals(TSharedMeshPtr _mesh);

		CommandFunction GetCommand(TCharTokenizerItr _command);
		void LoadVertexCommand(TCharTokenizerItr _args, TCharTokenizerItr _end);
		void LoadNormalsCommand(TCharTokenizerItr _args, TCharTokenizerItr _end);
		void LoadTexCoord0sCommand(TCharTokenizerItr _args, TCharTokenizerItr _end);
		void LoadFacesCommand(TCharTokenizerItr _args, TCharTokenizerItr _end);
		void AllignArraysWithArray(float* _actualArrayToBe, const TFloatArray& _currArray
			,const TUIntArray& _arrayInds, unsigned int _countPerVal,const TUIntArray& _destArrayInds);

		void ClearAllBuffers();

		template<class T> void CopyBuffers(const std::vector<T>& _src,T*& _dest)
		{
			unsigned int count = (unsigned int)_src.size();

			for(unsigned int i = 0 ; i < count ; i++ )
			{
				_dest[i] = _src[i];
			}
		}
		
	};
}

#endif