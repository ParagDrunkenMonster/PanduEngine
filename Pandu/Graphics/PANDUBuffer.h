/********************************************************************
	filename: 	PANDUBuffer
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUBuffer_h__
#define __PANDUBuffer_h__

#include <malloc.h>
#include <algorithm>
#include "../Utils/PANDUCoreDefines.h"

namespace Pandu
{
	class Buffer
	{
	public:

		enum BufferElementType
		{
			eBET_None,
			eBET_Float,
			eBET_UnsignedInt,
		};

	private:

		BufferElementType m_DataElementType;
		unsigned int m_TotalDataCount;
		unsigned int m_DataStride;
		unsigned int m_DataElementSize;
		char* m_Data;

	public:

		inline Buffer()
			: m_DataElementType(eBET_None)
			, m_TotalDataCount(0)
			, m_DataStride(0)
			, m_DataElementSize(0)
			, m_Data(0)
		{

		}

		inline ~Buffer()
		{
			FreeData();
		}

		inline void AllocateBuffer(BufferElementType _elementType, unsigned int _totalDataCount, unsigned int _dataStride, unsigned int _dataElementSize)
		{
			PANDU_ERROR(m_DataElementType == eBET_None, "Data is already allocated please free the allocated data first" );

			if( m_DataElementType == eBET_None && !m_Data)
			{
				m_DataElementType = _elementType;
				m_TotalDataCount = _totalDataCount;
				m_DataStride = _dataStride;
				m_DataElementSize = _dataElementSize;
				m_Data = static_cast<char*>(malloc(m_TotalDataCount * m_DataElementSize));
			}
		}

		inline void SetDataWithOwnership(char* _data, BufferElementType _elementType, unsigned int _totalDataCount, unsigned int _dataStride, unsigned int _dataElementSize)
		{
			PANDU_ERROR(m_DataElementType == eBET_None, "Data is already allocated please free the allocated data first" );

			if( m_DataElementType == eBET_None && !m_Data)
			{
				m_DataElementType = _elementType;
				m_TotalDataCount = _totalDataCount;
				m_DataStride = _dataStride;
				m_DataElementSize = _dataElementSize;
				m_Data = _data;
			}
		}

		inline void CopyDataToBuffer(const char* const data , unsigned int _bufferSize)
		{
			PANDU_ERROR(m_DataElementType != eBET_None, "Buffer is not allocated yet" );

			if( m_DataElementType != eBET_None && m_Data)
			{
				const unsigned int sizeToCpy = std::min<unsigned int>(m_TotalDataCount*m_DataElementSize,_bufferSize);
				std::memcpy(m_Data,data,(std::size_t)sizeToCpy);
			}
		}

		inline void FreeData()
		{
			m_DataElementType = eBET_None;
			m_TotalDataCount = 0;
			m_DataStride = 0;
			m_DataElementSize = 0;

			if( m_Data )
			{
				std::free(m_Data);
				m_Data = NULL;
			}
		}

		inline BufferElementType GetDataElementType() const		{	return m_DataElementType;				}
		inline unsigned int GetTotalDataCount() const			{	return m_TotalDataCount;				}
		inline unsigned int GetDataStride() const				{	return m_DataStride;					}
		inline unsigned int GetDataElementSize() const			{	return m_DataElementSize;				}
		inline char* GetData()									{	return m_Data;							}
		inline const char* GetData() const						{	return m_Data;							}

	};
}

#endif //__PANDUBuffer_h__