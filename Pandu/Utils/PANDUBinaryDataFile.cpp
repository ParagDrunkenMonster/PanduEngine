#include "PANDUBinaryDataFile.h"

namespace Pandu
{
	// ----------------------------------------------------------------------------
	// Data blocks
	void BinaryDataFile::BFileData::Clear()
	{
		unsigned int count = (unsigned int)m_DataBlocks.size();
		for( unsigned int i = 0 ; i < count ; i++ )
		{
			delete m_DataBlocks[i];
		}
		m_DataBlocks.clear();
	}

	inline BinaryDataFile::BFileData::DataBlock* BinaryDataFile::BFileData::AddNewDataBlock()		
	{	
		DataBlock* newDataBlock = new DataBlock();
		m_DataBlocks.push_back(newDataBlock);	
		return newDataBlock;
	}

	void BinaryDataFile::BFileData::RemoveDataBlock(DataBlock* _dataBlock)
	{
		TDataBlockArray::iterator begin = m_DataBlocks.begin();
		TDataBlockArray::iterator end = m_DataBlocks.end();
		TDataBlockArray::iterator found = end;

		found = std::find (begin, end, _dataBlock);

		if( found != end )
		{
			delete *found;
			found = m_DataBlocks.erase(found);
		}
	}

	BinaryDataFile::BFileData::DataBlock* BinaryDataFile::BFileData::GetDataBlock( unsigned int _index )			
	{
		if( _index >= DataBlocksCount() )
		{
			return NULL;
		}
		return m_DataBlocks[_index];
	}

	const BinaryDataFile::BFileData::DataBlock* BinaryDataFile::BFileData::GetDataBlock( unsigned int _index )	const		
	{
		if( _index >= DataBlocksCount() )
		{
			return NULL;
		}
		return m_DataBlocks[_index];
	}

	// End Data blocks
	// ----------------------------------------------------------------------------
	
	void BinaryDataFile::ComputeFileSizeFromData() 
	{
		const BFileData::TDataBlockArray& dataBlocksArray = m_Data.GetAllDataBlocks();
		unsigned int dataBlockCount = (unsigned int)dataBlocksArray.size();

		unsigned int size = sizeof(BFileHeaderChunk) + sizeof(BFileDataChunk);

		for( unsigned int i = 0 ; i < dataBlockCount ; i++ )
		{
			const BFileData::DataBlock& dataBlock = *dataBlocksArray[i];
			size += sizeof(BFileData::DataBlock::DataBlockHeader);
			size += dataBlock.m_DataBlockHeader.m_DataCount * dataBlock.m_DataBlockHeader.m_DataElementSize;
		}

		m_FileSize = size;
	}

	void BinaryDataFile::DumpDataToFile()
	{
		if( !m_DataLoaded )
		{
			Pandu::gLogManager->WriteLog("Error data not loaded...");
			return;
		}

		BFileHeaderChunk &fileHeader = GetFileHeader();
		BFileDataChunk &dataChunck = GetDataChunk();
		const BFileData &fileData = GetFileData();

		const unsigned int extLength = (unsigned int)strlen(m_FileExt.CString()) + 1;

		const unsigned int fileNameLength = (const unsigned int)m_FileName.Length();
		String extension("");
		String fileNameLocal( m_FileName );

		if( fileNameLength > extLength)
		{
			extension = String( &(m_FileName.CString())[fileNameLength-extLength] );
		}

		if( extension != ( "." + m_FileExt) )
		{
			fileNameLocal = fileNameLocal + ( "." + m_FileExt) ;
		}

		File file( fileNameLocal );
		bool fileOpen = file.Open(Pandu::TFlag32( (unsigned int)(File::eFM_Binary | File::eFM_Write)) ); 

		if( fileOpen )
		{
			fileHeader.SetMagicNumber(m_MagicNumber);
			fileHeader.SetVersionNumber(m_VersionNumber);

			ComputeFileSizeFromData();
			fileHeader.SetFileSize(GetFileSize());

			file.WriteBufferToFile( (const char*)&fileHeader,sizeof(BFileHeaderChunk));
			file.WriteBufferToFile( (const char*)&dataChunck,sizeof(BFileDataChunk));

			const BFileData::TDataBlockArray& allDataBlocks = fileData.GetAllDataBlocks();
			const unsigned int dataBlockCount = (unsigned int)allDataBlocks.size();

			for( unsigned int i = 0 ; i < dataBlockCount ; i++ )
			{
				DumpDataBlockToFile(*allDataBlocks[i],file);
			}

			Reset();
		}
		else
		{
			Pandu::gLogManager->WriteLog(String("Unable to open file '") + fileNameLocal + "'...");
		}

		file.Close();
	}

	void BinaryDataFile::DumpDataBlockToFile( const BFileData::DataBlock& _dataBlock, File& _file ) const
	{
		_file.WriteBufferToFile( (const char*)&_dataBlock.m_DataBlockHeader,sizeof(BFileData::DataBlock::DataBlockHeader));

		const float* test = (const float*)_dataBlock.m_Data;
		_file.WriteBufferToFile( (const char*)_dataBlock.m_Data,_dataBlock.GetDataSize());
	}

	void BinaryDataFile::Reset()
	{
		m_FileHeader.SetFileSize(0);
		m_DataChunk.ClearAllSemantic();
		m_Data.Clear();
		m_FileSize = 0;
		m_DataLoaded = false;
	}

	bool BinaryDataFile::LoadFromFile()
	{
		if( m_DataLoaded )
		{
			Pandu::gLogManager->WriteLog("Error data already loaded...");
			return false;
		}

		BFileHeaderChunk &fileHeader = GetFileHeader();
		BFileDataChunk &dataChunck = GetDataChunk();
		BFileData &fileData = GetFileData();

		File file( m_FileName );
		bool open = file.Open(Pandu::TFlag32( (unsigned int)(File::eFM_Binary | File::eFM_Read)) );

		bool loaded = file.LoadBufferFromFile( (char*)&fileHeader,sizeof(BFileHeaderChunk));

		if( loaded )
		{
			unsigned int fileSizefromFile = fileHeader.GetFileSize();
			unsigned int fileSize = file.GetFileLength();

			if( fileSizefromFile == fileSize )
			{
				loaded = file.LoadBufferFromFile( (char*)&dataChunck,sizeof(BFileDataChunk));

				if( loaded )
				{
					if( fileHeader.GetMagicNumber() == m_MagicNumber && fileHeader.GetChunkId() == eCI_Header )
					{
						if( dataChunck.GetChunkId() == eCI_Data )
						{
							unsigned int sizeofLeftFileData = fileSizefromFile - ( sizeof(BFileHeaderChunk) + sizeof(BFileDataChunk) );
							bool failedLoad = false;
							while( sizeofLeftFileData )
							{
								BFileData::DataBlock* addDataBlock = fileData.AddNewDataBlock();
								if( LoadFileDataBlockFromFile(file,*addDataBlock) )
								{
									sizeofLeftFileData -= sizeof(BFileData::DataBlock::DataBlockHeader) + addDataBlock->GetDataSize();
								}
								else
								{
									Pandu::gLogManager->WriteLog("Error occured during loading data block...");
									failedLoad = true;
									break;
								}
							}

							file.Close();

							m_DataLoaded = !failedLoad;

							if(m_DataLoaded)
							{
								ComputeFileSizeFromData();
								fileHeader.SetFileSize(GetFileSize());
							}

							return !failedLoad;
						}
						else
						{
							Pandu::gLogManager->WriteLog("File is corrupted...");
						}
					}
					else
					{
						Pandu::gLogManager->WriteLog("Wrong file format or file is corrupted...");
					}
				}
			}
			else
			{
				Pandu::gLogManager->WriteLog("File corrupted...");
			}
		}

		file.Close();

		return false;
	}

	bool BinaryDataFile::LoadFileDataBlockFromFile( File& _file, BFileData::DataBlock& _oDataBlock ) const
	{
		bool loadedHeader = _file.LoadBufferFromFile((char*)&_oDataBlock.m_DataBlockHeader,sizeof(BFileData::DataBlock::DataBlockHeader));

		if( loadedHeader && _oDataBlock.GetDataSize() > 0 )
		{
			_oDataBlock.m_Data = static_cast<char*>(malloc(_oDataBlock.GetDataSize()));
			bool loadedData = _file.LoadBufferFromFile(_oDataBlock.m_Data,_oDataBlock.GetDataSize());
			return loadedData;
		}

		return false;
	}

	BinaryDataFile::BFileData::DataBlock* BinaryDataFile::GetNextDataBlockBySemantic(const BFileData& _fileData, unsigned int _semantic, unsigned int& _inOutCurrIndex) const
	{
		const BFileData::TDataBlockArray& allDataBlocks = _fileData.GetAllDataBlocks();
		const unsigned int count = (unsigned int)allDataBlocks.size();

		for( unsigned int i = _inOutCurrIndex ; i < count ; i++ )
		{
			if( allDataBlocks[i]->m_DataBlockHeader.m_DataBlockSemantic == _semantic )
			{
				_inOutCurrIndex = i;
				return allDataBlocks[i];
			}
		}

		return NULL;
	}
}