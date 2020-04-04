#include "PANDUSkeletonMeshModifier.h"

namespace Pandu
{
	SkeletonMeshModifier::SkeletonMeshModifier()
	{

	}

	SkeletonMeshModifier::~SkeletonMeshModifier()
	{
		m_SkeletonInstance.reset();
	}

	void SkeletonMeshModifier::OnMeshSet()
	{
		TSharedSkeletonPtr skeleton = gSkeletonManager->GetSkeleton(GetMesh()->GetSkeletonName());
		PANDU_ERROR(skeleton != NULL, (String("Can't find skeleton named '") + GetMesh()->GetSkeletonName() + "'").CString() );
		m_SkeletonInstance = skeleton->CreateSkeletonInstance();
		m_SkeletonInstance->Update();
	}

	void SkeletonMeshModifier::ModifyMesh(const VertexBuffer& _vertexBuffer, VertexBuffer& _modifiedVertexBuffer)
	{
		const TBoneSharedPtrArray& bonesArray = m_SkeletonInstance->GetBonesSharedPtrArray();

		const Matrix44& BSM = GetMesh()->GetBindShapeMatrix();
		const Mesh::TInvBindMatrixArray& IBMi = GetMesh()->GetInvBindMatricesArray();
		const Buffer* bnsBffr = _vertexBuffer.GetBuffer(VertexBuffer::eBT_Bones);
		const Buffer* vrtxWt = _vertexBuffer.GetBuffer(VertexBuffer::eBT_VertexWeight);
		const Buffer* vbBneInfluenceCountInd = _vertexBuffer.GetBuffer(VertexBuffer::eBT_BoneInfluenceCountIndex);
		const Buffer* bneIndx = _vertexBuffer.GetBuffer(VertexBuffer::eBT_BoneIndex);
		const Buffer* wtIndx = _vertexBuffer.GetBuffer(VertexBuffer::eBT_VertexWeightIndex);
		const Buffer* vertBffr = _vertexBuffer.GetBuffer(VertexBuffer::eBT_Vertex);

		const unsigned int vertsCount = vertBffr->GetTotalDataCount()/3;
		const float* fVertBffr = (const float*)(vertBffr->GetData());

		const unsigned int bneInfCountIndCount = vbBneInfluenceCountInd->GetTotalDataCount();
		const unsigned int* iVbBneInfluenceCountInd = (const unsigned int*)vbBneInfluenceCountInd->GetData();
		const unsigned int* iBneIndx = (const unsigned int*)(bneIndx->GetData());
		const unsigned int* iWtIndx = (const unsigned int*)(wtIndx->GetData());
		const unsigned int* iBnsBffr = (const unsigned int*)(bnsBffr->GetData());
		const float* fVrtxWt = (const float*)(vrtxWt->GetData());

		PANDU_ERROR(vertsCount == bneInfCountIndCount, "Verts count and bone influences index count has to be equal always" );

		Buffer* opVBuffer = _modifiedVertexBuffer.GetBuffer(VertexBuffer::eBT_Vertex);
		if(!opVBuffer)
		{
			opVBuffer = _modifiedVertexBuffer.AllocateBuffer(VertexBuffer::eBT_Vertex,vertBffr->GetDataElementType(),vertBffr->GetTotalDataCount(),vertBffr->GetDataStride(),vertBffr->GetDataElementSize());
		}

		float *fOPVBuffer = (float*)(opVBuffer->GetData());

		/*
		 The skinning calculation for each vertex v in a bind shape is
		 for i to n
			  v += {[(v * BSM) * IBMi * JMi] * JW}
	 
		 • n: The number of joints that influence vertex v
		 • BSM: Bind-shape matrix
		 • IBMi: Inverse bind-pose matrix of joint i
		 • JMi: Transformation matrix of joint i
		 • JW: Weight of the influence of joint i on vertex v
		*/
		
		unsigned int ind = 0;
		for( unsigned int i = 0 ; i < vertsCount ; i++ )
		{
			Vector3 tempVertex(0.0f,0.0f,0.0f);
			const Vector3 vertex(fVertBffr[i * 3 + 0],fVertBffr[i * 3 + 1],fVertBffr[i * 3 + 2]);

			float totalJointsWeight = 0;

			for( unsigned int infI = 0 ; infI < iVbBneInfluenceCountInd[i] ; infI++ )
			{
				const unsigned bneIndInMesh = iBneIndx[ind];
				const unsigned int boneInd = iBnsBffr[bneIndInMesh];
				const Bone* bone = bonesArray[boneInd].get();
				
				// TODO:
				// Need to optimize precomputing and storing IBMi*BSM*vertex 
				tempVertex += (bone->GetWorldTransform() * IBMi[bneIndInMesh] * (BSM * vertex)) * fVrtxWt[iWtIndx[ind]];
				totalJointsWeight += fVrtxWt[iWtIndx[ind]];
				ind++;
			}
			float invTotalWt = 1.0f/totalJointsWeight;
			tempVertex *= invTotalWt;

			fOPVBuffer[i * 3 + 0] = tempVertex.x;
			fOPVBuffer[i * 3 + 1] = tempVertex.y;
			fOPVBuffer[i * 3 + 2] = tempVertex.z;
			//memcpy(&fOPVBuffer[i * 3],tempVertex.arr,3);
		}
	}
}