#include "PANDUGeometryBinary.h"

namespace Pandu
{
	bool GeometryBinary::LoadFromFile()
	{
		bool loaded = BinaryDataFile::LoadFromFile();

		if( loaded )
		{
			BFileHeaderChunk &fileHeader = GetFileHeader();
			BFileDataChunk &geometry = GetDataChunk();
			BFileData &fileData = GetFileData();
			
			if( !geometry.HasSemantic(eGBS_MESHNAME) || !geometry.HasSemantic(eGBS_POSITION) || !geometry.HasSemantic(eGBS_INDICES) )
			{
				Pandu::gLogManager->WriteLog("Basic data required to build a mesh is not present in the file or file is corrupted...");
				return false;
			}
			
			return true;
		}
		return false;
	}

	TSharedMeshPtr GeometryBinary::CreateMeshWithData()
	{
		if( !m_DataLoaded )
		{
			bool loaded = LoadFromFile();
			if( !loaded )
			{
				TSharedMeshPtr();
			}
		}

		const BFileHeaderChunk &fileHeader = GetFileHeader();
		const BFileDataChunk &geometry = GetDataChunk();
		const BFileData &fileData = GetFileData();

		unsigned int currIndex = 0;

		BFileData::DataBlock* meshNameDataBlock = GetNextDataBlockBySemantic(fileData,eGBS_MESHNAME,currIndex);

		TSharedMeshPtr newMesh(new Mesh(meshNameDataBlock->m_Data ));

		newMesh->Lock();
		SetGeometryDataToMesh(newMesh);
		newMesh->Unlock();

		return newMesh;
	}

	void GeometryBinary::SetGeometryDataToMesh(TSharedMeshPtr _mesh)
	{
		if( !m_DataLoaded )
		{
			Pandu::gLogManager->WriteLog("Error data not loaded...");
			return;
		}

		const BFileHeaderChunk &fileHeader = GetFileHeader();
		const BFileDataChunk &geometry = GetDataChunk();
		const BFileData &fileData = GetFileData();

		unsigned int currIndex[eGBS_Count];
		
		for( unsigned int i = 0 ; i < eGBS_Count ; i++ )
		{
			currIndex[i] = 0;
		}

		BFileData::DataBlock* vertexDataBlock = GetNextDataBlockBySemantic(fileData,eGBS_POSITION,currIndex[eGBS_POSITION]);
		
		if( _mesh->SetVertexBufferWithDataOwnership((float*)vertexDataBlock->m_Data,vertexDataBlock->m_DataBlockHeader.m_DataCount))
		{
			vertexDataBlock->RemoveOwnershipFromData();
		}

		BFileData::DataBlock* indDataBlock = GetNextDataBlockBySemantic(fileData,eGBS_INDICES,currIndex[eGBS_INDICES]);

		if( _mesh->SetIndexBufferWithDataOwnership((unsigned int*)indDataBlock->m_Data,indDataBlock->m_DataBlockHeader.m_DataCount))
		{
			indDataBlock->RemoveOwnershipFromData();
		}

		BFileData::DataBlock* normDataBlock = GetNextDataBlockBySemantic(fileData,eGBS_NORMAL,currIndex[eGBS_NORMAL]);

		if( normDataBlock && _mesh->SetNormalBufferWithDataOwnership((float*)normDataBlock->m_Data,normDataBlock->m_DataBlockHeader.m_DataCount))
		{
			normDataBlock->RemoveOwnershipFromData();
		}

		BFileData::DataBlock* tex0DataBlock = GetNextDataBlockBySemantic(fileData,eGBS_TEXCOORD0,currIndex[eGBS_TEXCOORD0]);

		if( tex0DataBlock && _mesh->SetTexCoordBufferWithDataOwnership((float*)tex0DataBlock->m_Data,tex0DataBlock->m_DataBlockHeader.m_DataCount))
		{
			tex0DataBlock->RemoveOwnershipFromData();
		}

		BFileData::DataBlock* tangentDataBlock = GetNextDataBlockBySemantic(fileData,eGBS_TANGENT,currIndex[eGBS_TANGENT]);

		if( tangentDataBlock && _mesh->SetTangentBufferWithDataOwnership((float*)tangentDataBlock->m_Data,tangentDataBlock->m_DataBlockHeader.m_DataCount))
		{
			tangentDataBlock->RemoveOwnershipFromData();
		}

		BFileData::DataBlock* biNormalDataBlock = GetNextDataBlockBySemantic(fileData,eGBS_BINORMAL,currIndex[eGBS_BINORMAL]);

		if( biNormalDataBlock && _mesh->SetBinormalBufferWithDataOwnership((float*)biNormalDataBlock->m_Data,biNormalDataBlock->m_DataBlockHeader.m_DataCount))
		{
			biNormalDataBlock->RemoveOwnershipFromData();
		}

		//----------------------------------------------------------------------------------------

		BFileData::DataBlock* skeletonNameDataBlock = GetNextDataBlockBySemantic(fileData,eGBS_SKELETONNAME,currIndex[eGBS_SKELETONNAME]);

		if(skeletonNameDataBlock)
		{
			_mesh->SetSkeletonName(skeletonNameDataBlock->m_Data);
		}

		BFileData::DataBlock* boneIndexDataBlock = GetNextDataBlockBySemantic(fileData,eGBS_BONES,currIndex[eGBS_BONES]);

		if(boneIndexDataBlock && _mesh->SetBonesWithDataOwnership((unsigned int*)boneIndexDataBlock->m_Data,boneIndexDataBlock->m_DataBlockHeader.m_DataCount))
		{
			boneIndexDataBlock->RemoveOwnershipFromData();
		}

		BFileData::DataBlock* boneWeightsDataBlock = GetNextDataBlockBySemantic(fileData,eGBS_BONEWEIGHTS,currIndex[eGBS_BONEWEIGHTS]);

		if(boneWeightsDataBlock && _mesh->SetVertexWeightBufferWithDataOwnership((float*)boneWeightsDataBlock->m_Data,boneWeightsDataBlock->m_DataBlockHeader.m_DataCount))
		{
			boneWeightsDataBlock->RemoveOwnershipFromData();
		}

		BFileData::DataBlock* invBindMatrix = GetNextDataBlockBySemantic(fileData,eGBS_INVBINDMATRIX,currIndex[eGBS_INVBINDMATRIX]);

		if(invBindMatrix )
		{
			const unsigned int bindMatCount = invBindMatrix->m_DataBlockHeader.m_DataCount/invBindMatrix->m_DataBlockHeader.m_DataStride;
			float* floatData = (float*)invBindMatrix->m_Data;

			for( unsigned int bMat = 0 ; bMat < bindMatCount ; bMat++ )
			{
				Matrix44 mat;
				for( unsigned int i = 0 ; i < invBindMatrix->m_DataBlockHeader.m_DataStride ; i++ )
				{
					mat.arr[i] = floatData[bMat * invBindMatrix->m_DataBlockHeader.m_DataStride + i];
				}

				_mesh->AddInverseBindMatrix(mat);
			}
		}

		BFileData::DataBlock* bindShapeMatrix = GetNextDataBlockBySemantic(fileData,eGBS_BINDSHAPEMATRIX,currIndex[eGBS_BINDSHAPEMATRIX]);

		if(bindShapeMatrix )
		{
			float* floatData = (float*)bindShapeMatrix->m_Data;
			Matrix44 mat;
			for( int i = 0 ; i < 16 ; i++ )
			{
				mat.arr[i] = floatData[i];
			}

			_mesh->SetBindShapeMatrix(mat);
		}

		BFileData::DataBlock* bInflCountInds = GetNextDataBlockBySemantic(fileData,eGBS_BONEINFLUENCECOUNTINDICES,currIndex[eGBS_BONEINFLUENCECOUNTINDICES]);

		if(bInflCountInds && _mesh->SetBoneInfluenceCountIndexBufferWithDataOwnership((unsigned int*)bInflCountInds->m_Data,bInflCountInds->m_DataBlockHeader.m_DataCount) )
		{
			bInflCountInds->RemoveOwnershipFromData();
		}

		BFileData::DataBlock* weightInds = GetNextDataBlockBySemantic(fileData,eGBS_WEIGHTINDICES,currIndex[eGBS_WEIGHTINDICES]);

		if(weightInds && _mesh->SetVertexWeightIndexBufferWithDataOwnership((unsigned int*)weightInds->m_Data,weightInds->m_DataBlockHeader.m_DataCount) )
		{
			weightInds->RemoveOwnershipFromData();
		}

		BFileData::DataBlock* boneInds = GetNextDataBlockBySemantic(fileData,eGBS_BONEINDICES,currIndex[eGBS_BONEINDICES]);

		if(boneInds && _mesh->SetBoneIndexBufferWithDataOwnership((unsigned int*)boneInds->m_Data,boneInds->m_DataBlockHeader.m_DataCount) )
		{
			boneInds->RemoveOwnershipFromData();
		}

		Reset();
	}
}