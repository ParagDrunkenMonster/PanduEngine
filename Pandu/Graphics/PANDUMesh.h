/********************************************************************
	filename: 	PANDUMesh
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUMesh_h__
#define __PANDUMesh_h__

#include "../boost/smart_ptr.hpp"
#include <vector>
#include <list>
#include "../Math/PANDUVector3.h"
#include "../Math/PANDUMatrix44.h"
#include "PANDUVertexBuffer.h"
#include "PANDUMeshInstance.h"
#include "../Utils/PANDUString.h"


namespace Pandu
{

	class Mesh;

	typedef boost::shared_ptr<Mesh> TSharedMeshPtr;
	typedef std::list<TSharedMeshPtr> TSharedMeshPtrList;

	class Mesh
	{
	public:

		friend class MeshModifier;

		typedef std::vector<Matrix44> TInvBindMatrixArray;

		enum PolygonMode
		{
			ePM_Lines = 0,
			ePM_LineLoop,
			ePM_LineStrip,
			ePM_Triangles,
			ePM_TriangleStrip,
			ePM_TriangleFan,

			ePM_Count
		};

	private:

		class Impl;
		boost::scoped_ptr<Impl> m_Impl;

	public:
		Mesh(const String& _meshName);
		virtual ~Mesh();

		const String& GetName() const;

		TSharedMeshInstancePtr CreateMeshInstance() const;

		bool SetVertexBuffer(const float* const _vBuffer, unsigned int _vCount);
		bool SetIndexBuffer(const unsigned int* const _iBuffer, unsigned int _iCount);
		bool SetTexCoordBuffer(const float* const _tBuffer, unsigned int _tCount);
		bool SetNormalBuffer(const float* const _nBuffer, unsigned int _nCount);

		bool SetVertexBufferWithDataOwnership(float* _vBuffer, unsigned int _vCount);
		bool SetIndexBufferWithDataOwnership(unsigned int* _iBuffer, unsigned int _iCount);
		bool SetTexCoordBufferWithDataOwnership(float* _tBuffer, unsigned int _tCount);
		bool SetNormalBufferWithDataOwnership(float* _nBuffer, unsigned int _nCount);
		bool SetTangentBufferWithDataOwnership(float* _tgntBuffer, unsigned int _tgntCount);
		bool SetBinormalBufferWithDataOwnership(float* _biBuffer, unsigned int _biCount );

		void SetSkeletonName( const String& _name );
		const String& GetSkeletonName() const;

		bool SetBonesWithDataOwnership(unsigned int* _boneIndBuffer, unsigned int _boneCount);
		bool SetVertexWeightBufferWithDataOwnership(float* _weightBuffer, unsigned int _weightCount);
		bool SetBoneInfluenceCountIndexBufferWithDataOwnership(unsigned int* _boneInfCountIndsBuffer, unsigned int _count);
		bool SetBoneIndexBufferWithDataOwnership(unsigned int* _boneIndexBuffer, unsigned int _count);
		bool SetVertexWeightIndexBufferWithDataOwnership(unsigned int* _weightIndexBuffer, unsigned int _count);

		void AddInverseBindMatrix(const Matrix44& _matrix);
		void SetBindShapeMatrix(const Matrix44& _matrix);

		void Lock();
		void Unlock();

		const TInvBindMatrixArray& GetInvBindMatricesArray() const;
		const Matrix44& GetBindShapeMatrix() const;

		void SetPolygonMode(PolygonMode _polygonMode);
	};

	
}

#endif