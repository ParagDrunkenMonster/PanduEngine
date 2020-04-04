#include "PANDUVertexBuffer.h"

namespace Pandu
{
	const bool VertexBuffer::gCanCreateCustomAttribute[VertexBuffer::eBT_Count] = 
	{
		false,		//eBT_Vertex = 0,
		false,		//eBT_Normal,
		true,		//eBT_Tangent,
		true,		//eBT_Binormal,
		false,		//eBT_Texcoord0,
		false,		//eBT_Index,
		false,		//eBT_Bones,
		false,		//eBT_VertexWeight,
		false,		//eBT_BoneInfluenceCountIndex,
		false,		//eBT_BoneIndex,
		false,		//eBT_VertexWeightIndex,
	};

	const String VertexBuffer::gCustomAttributeNames[VertexBuffer::eBT_Count] = 
	{
		"",				//eBT_Vertex = 0,
		"",				//eBT_Normal,
		"PANDUTangent",	//eBT_Tangent,
		"PANDUBinormal",	//eBT_Binormal,
		"",				//eBT_Texcoord0,
		"",				//eBT_Index,
		"",				//eBT_Bones,
		"",				//eBT_VertexWeight,
		"",				//eBT_BoneInfluenceCountIndex,
		"",				//eBT_BoneIndex,
		"",				//eBT_VertexWeightIndex,
	};

	const VertexBuffer::BufferType VertexBuffer::gBufferTypeCanCreateAttribute[] = 
	{
		VertexBuffer::eBT_Tangent,
		VertexBuffer::eBT_Binormal
	};

	const unsigned short VertexBuffer::gBufferCountCanCreateAttribute = sizeof(gBufferTypeCanCreateAttribute)/sizeof(*gBufferTypeCanCreateAttribute);
}