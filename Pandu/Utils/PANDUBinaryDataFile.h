/********************************************************************
	filename: 	PANDUBinaryDataFile
	author:		Parag Moni Boro
	
	purpose:	Game Engine created for learning
*********************************************************************/

#ifndef __PANDUBinaryDataFile_h__
#define __PANDUBinaryDataFile_h__

#include "PANDUFlag.h"
#include "PANDUCoreDefines.h"
#include <vector>
#include <algorithm>
#include "PANDUFile.h"
#include "PANDUStringTokenizer.h"

namespace Pandu
{
	class BinaryDataFile
	{
	public:

		static const char gNameSeperator = '\n'; //used with dataElementType

		enum BDataElementType
		{
			eBDET_Float = 0,
			eBDET_Int,
			eBDET_UnsignedInt,
			eBDET_Char,
			eBDET_Name,
			eBDET_Float4x4,

			eBDET_Count,
		};

		enum ChunkId
		{
			eCI_Header = 0,
			eCI_Data,
		};

		// ----------------------------------------------------------------------------
		// Header Chunck
		// Total chunk size is 32 bytes 
		class BFileHeaderChunk
		{
		private:

			unsigned int m_MagicNumber;
			unsigned int m_ChunkId;
			unsigned int m_VersionNumber;
			unsigned int m_FileSize;
			unsigned int m_UnusedData[4]; //for future use //size 4 * 4

		public:

			inline BFileHeaderChunk()
				: m_MagicNumber(0)
				, m_VersionNumber(1)
				, m_ChunkId(eCI_Header)
				, m_FileSize(0)
			{
			}
			
			inline void SetMagicNumber(unsigned int _magicNumber)	{		m_MagicNumber = _magicNumber;				}
			inline unsigned int GetMagicNumber() const				{		return m_MagicNumber;						}
			inline ChunkId GetChunkId() const						{		return (ChunkId)m_ChunkId;					}
			inline void SetVersionNumber(unsigned int _vNumber)		{		m_VersionNumber = _vNumber;					}
			inline unsigned int GetVersionNumber() const			{		return m_VersionNumber;						}
			inline void SetFileSize(unsigned int _fileSize)			{		m_FileSize = _fileSize;						}
			inline unsigned int GetFileSize() const					{		return m_FileSize;							}
		};

		// End Header Chunck
		// ----------------------------------------------------------------------------

		// ----------------------------------------------------------------------------
		// Data Chunck
		// Total chunk size is 32 bytes 
		class BFileDataChunk
		{
		private:

			unsigned int m_ChunkId;
			TFlag32 m_DataContailsSemantic;
			unsigned int m_UnusedData[6];	//For future use //size 4 * 6

		public:

			inline BFileDataChunk()
				: m_ChunkId(eCI_Data)
			{
			}

			inline ChunkId GetChunkId() const							{	return (ChunkId)m_ChunkId;										}
			inline void AddSemantic( unsigned int _semantic )			{	m_DataContailsSemantic.SetFlag(1 << _semantic);					}
			inline void RemoveSemantic( unsigned int _semantic )		{	m_DataContailsSemantic.ClearFlag(1 << _semantic);				}
			inline bool HasSemantic( unsigned int _semantic )	const	{	return m_DataContailsSemantic.CheckFlag(1 << _semantic);		}
			inline void ClearAllSemantic()								{	m_DataContailsSemantic.ResetFlag();								}
		};

		// End Data Chunck
		// ----------------------------------------------------------------------------

		// ----------------------------------------------------------------------------
		// Data blocks
		class BFileData
		{
		public:

			// ------------------------------------------------------------------------
			struct DataBlock
			{
				// --------------------------------------------------------------------
				// Data block Header
				//each data block header is 32 bytes with m_DataCount = 0
				struct DataBlockHeader
				{
					unsigned int m_DataBlockSemantic;
					unsigned int m_DataElementType;
					unsigned int m_DataElementSize;
					unsigned int m_DataStride;
					unsigned int m_DataCount;
					unsigned int m_UserData[3];		// can be utilized by user

					inline DataBlockHeader()
						: m_DataBlockSemantic(0)
						, m_DataElementType(0)
						, m_DataElementSize(0)
						, m_DataStride(0)
						, m_DataCount(0)
					{
						m_UserData[0] = 0;
						m_UserData[1] = 0;
						m_UserData[2] = 0;
					}

					inline ~DataBlockHeader()
					{
						Reset();
					}

					inline void Reset()
					{
						m_DataElementType = 0;
						m_DataElementSize = 0;
						m_DataStride = 0;
						m_DataCount = 0;
						m_UserData[0] = 0;
						m_UserData[1] = 0;
						m_UserData[2] = 0;
					}
				};

				// End Data block Header
				// --------------------------------------------------------------------

				DataBlockHeader m_DataBlockHeader;
				char *m_Data;

				inline DataBlock():m_Data(NULL){}
				inline ~DataBlock()
				{
					if(m_Data)
					{
						std::free(m_Data);
						m_Data = NULL;
					}
				}

				inline unsigned int GetDataSize() const			//GetData Size in bytes
				{
					unsigned int size = m_DataBlockHeader.m_DataElementSize * m_DataBlockHeader.m_DataCount;
					return size;
				}

				inline void RemoveOwnershipFromData()		
				{		
					m_Data = NULL;	
					m_DataBlockHeader.Reset();
				}

				inline unsigned int GetUserData(unsigned int _index) const
				{
					PANDU_ERROR(_index < 3 ,"Only max 3 user data supported");
					return m_DataBlockHeader.m_UserData[_index];
				}

				inline void SetUserData(unsigned int _index, unsigned int _userData)
				{
					PANDU_ERROR(_index < 3 ,"Only max 3 user data supported");
					m_DataBlockHeader.m_UserData[_index] = _userData;
				}
			};
			// ------------------------------------------------------------------------

			typedef std::vector<DataBlock*> TDataBlockArray;

		private:

			TDataBlockArray m_DataBlocks;

		public:

			inline BFileData()
			{
			}

			inline ~BFileData()
			{
				Clear();
			}

			inline void Clear();
			DataBlock* AddNewDataBlock();
			void RemoveDataBlock(DataBlock* _dataBlock);

			inline unsigned int DataBlocksCount() const						{		return (unsigned int)m_DataBlocks.size();	}
			DataBlock* GetDataBlock( unsigned int _index );
			const DataBlock* GetDataBlock( unsigned int _index ) const;
			inline const TDataBlockArray& GetAllDataBlocks() const			{		return m_DataBlocks;						}
		};

		// End Data Blocks
		// ----------------------------------------------------------------------------

	protected:

		unsigned int				m_MagicNumber;
		unsigned int				m_VersionNumber;
		const String				m_FileName;
		const String				m_FileExt;

		BFileHeaderChunk			m_FileHeader;
		BFileDataChunk				m_DataChunk;
		BFileData					m_Data;
		unsigned int				m_FileSize;
		bool						m_DataLoaded;
		


	public:

		inline BinaryDataFile(unsigned int _magicNumber,unsigned int _versionNumber, const String& _fileName, const String& _fileExt)
			: m_MagicNumber(_magicNumber)
			, m_VersionNumber(_versionNumber)
			, m_FileName(_fileName)
			, m_FileExt(_fileExt)
			, m_FileSize(0)
			, m_DataLoaded(false)
		{

		}

		virtual ~BinaryDataFile()
		{
		}

		inline const String& GetFileName() const						{		return m_FileName;				}
		inline BFileHeaderChunk& GetFileHeader()						{		return m_FileHeader;			}
		inline const BFileHeaderChunk& GetFileHeader() const			{		return m_FileHeader;			}
		inline BFileDataChunk& GetDataChunk()						{		return m_DataChunk;		}
		inline const BFileDataChunk& GetDataChunk() const			{		return m_DataChunk;		}
		inline BFileData& GetFileData()									{		return m_Data;					}
		inline const BFileData& GetFileData() const						{		return m_Data;					}

		inline unsigned int GetFileSize() const							{		return m_FileSize;				}

		inline void SetDataLoaded()										{		m_DataLoaded = true;			}

		virtual void DumpDataToFile();
		virtual bool LoadFromFile();

	
	protected:

		void DumpDataBlockToFile( const BFileData::DataBlock& _dataBlock, File& _file ) const;
		BFileData::DataBlock* GetNextDataBlockBySemantic(const BFileData& _fileData, unsigned int _semantic, unsigned int& _inOutCurrIndex) const;
		bool LoadFileDataBlockFromFile( File& _file, BFileData::DataBlock& _oDataBlock ) const;
		void ComputeFileSizeFromData();
		void Reset();
	};
}

#endif //__PANDUBinaryDataFile_h__