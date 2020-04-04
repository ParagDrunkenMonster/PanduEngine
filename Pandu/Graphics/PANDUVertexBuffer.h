/********************************************************************
	filename: 	PANDUVertexBuffer
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUVertexBuffer_h__
#define __PANDUVertexBuffer_h__

#include "PANDUBuffer.h"
#include "../Utils/PANDUString.h"

namespace Pandu
{
	class VertexBuffer
	{
	public:

		enum BufferType
		{
			eBT_Vertex = 0,
			eBT_Normal,
			eBT_Tangent,			//Attribute Name: PANDUTangent
			eBT_Binormal,			//Attribute Name: PANDUBinormal
			eBT_Texcoord0,
			eBT_Index,
			eBT_Bones,
			eBT_VertexWeight,
			eBT_BoneInfluenceCountIndex,
			eBT_BoneIndex,
			eBT_VertexWeightIndex,

			eBT_Count,
		};

		static const bool gCanCreateCustomAttribute[VertexBuffer::eBT_Count];
		static const String gCustomAttributeNames[VertexBuffer::eBT_Count];
		static const BufferType gBufferTypeCanCreateAttribute[];
		static const unsigned short gBufferCountCanCreateAttribute;
	
	private:

		Buffer* m_Buffer[eBT_Count];

	public:

		inline VertexBuffer()
		{
			InitializeAll();
		}

		inline ~VertexBuffer()
		{
			DestroyAll();
		}

		inline Buffer* AllocateBuffer(BufferType _bt, Buffer::BufferElementType _elementType, unsigned int _totalDataCount, unsigned int _dataStride, unsigned int _dataElementSize)
		{
			PANDU_ERROR( (_bt >= 0 && _bt < eBT_Count) , "No buffer type exist" );
			PANDU_ERROR( !m_Buffer[_bt] , "Buffer already created, first free existing buffer" );
			if(!m_Buffer[_bt])
			{
				m_Buffer[_bt] = new Buffer();
				m_Buffer[_bt]->AllocateBuffer(_elementType,_totalDataCount,_dataStride,_dataElementSize);
			}

			return m_Buffer[_bt];
		}

		inline void SetDataWithOwnership(BufferType _bt, char* _data, Buffer::BufferElementType _elementType, unsigned int _totalDataCount, unsigned int _dataStride, unsigned int _dataElementSize)
		{
			PANDU_ERROR( (_bt >= 0 && _bt < eBT_Count) , "No buffer type exist" );
			if(!m_Buffer[_bt])
			{
				m_Buffer[_bt] = new Buffer();
				m_Buffer[_bt]->SetDataWithOwnership(_data,_elementType,_totalDataCount,_dataStride,_dataElementSize);
			}
			else
			{
				m_Buffer[_bt]->SetDataWithOwnership(_data,_elementType,_totalDataCount,_dataStride,_dataElementSize);
			}
		}

		inline void CopyDataToBuffer(BufferType _bt, const char* const data , unsigned int _bufferSize)
		{
			PANDU_ERROR( (_bt >= 0 && _bt < eBT_Count) , "No buffer type exist" );
			PANDU_ERROR( m_Buffer[_bt] , "Buffer not created yet" );
			if(m_Buffer[_bt])
			{
				m_Buffer[_bt]->CopyDataToBuffer(data,_bufferSize);
			}
		}

		inline void FreeBuffer(BufferType _bt)
		{
			PANDU_ERROR( (_bt >= 0 && _bt < eBT_Count) , "No buffer type exist" );

			if(m_Buffer[_bt])
			{
				m_Buffer[_bt]->FreeData();
				delete m_Buffer[_bt];
				m_Buffer[_bt] = NULL;
			}
		}

		inline void FreeBufferData(BufferType _bt)
		{
			PANDU_ERROR( (_bt >= 0 && _bt < eBT_Count) , "No buffer type exist" );

			if(m_Buffer[_bt])
			{
				m_Buffer[_bt]->FreeData();
			}
		}

		inline Buffer* GetBuffer(BufferType _bt) const	{	return m_Buffer[_bt];			}

		//static functions
		static inline bool CanCreateAttributeOfVertexBufferType(VertexBuffer::BufferType _bt)
		{
			if( _bt >= 0 && _bt < VertexBuffer::eBT_Count )
			{
				return gCanCreateCustomAttribute[_bt];
			}
			return false;
		}

		static inline const String& CustomAttributeNames(VertexBuffer::BufferType _bt)
		{
			if( _bt >= 0 && _bt < VertexBuffer::eBT_Count )
			{
				return gCustomAttributeNames[_bt];
			}
			return String::gZeroLengthString;
		}

	private:

		inline void InitializeAll()
		{
			for( unsigned int i = 0 ; i < eBT_Count ; i++ )
			{
				m_Buffer[i] = NULL;
			}
		}

		inline void DestroyAll()
		{
			for( unsigned int i = 0 ; i < eBT_Count ; i++ )
			{
				if(m_Buffer[i])
				{
					delete m_Buffer[i];
					m_Buffer[i] = NULL;
				}
			}
		}
	};
}

#endif //__PANDUVertexBuffer_h__