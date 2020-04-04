#include "PANDUFile.h"

namespace Pandu
{
	File::File(const String& _filename)
		: m_FileName(_filename)
		, m_FileLength(0)
		, m_IsFileOpen(false)
	{

	}

	File::~File()
	{
		Close();
	}

	bool File::LoadStringToBuffer(char* _buffer, unsigned int _bufferSize )
	{
		if(!m_IsFileOpen)
		{
			if(gLogManager)
			{
				gLogManager->WriteLog( "Can't Load to buffer, file " + m_FileName + " is not open" );
			}
			return false;
		}

		_buffer[m_FileLength] = '\0';  
		unsigned long i = 0;
		while (m_FileStream.good())
		{
			_buffer[i] = m_FileStream.get();     //get each character.
			if (!m_FileStream.eof())
			{
				i++;
				if( i >= _bufferSize )
				{
					_buffer[i-1] = '\0';
					if(gLogManager)
					{
						gLogManager->WriteLog( "Buffer size is not enough to load entire file to string" );
					}
				}
			}
		}

		_buffer[i] = '\0';

		return true;
	}

	bool File::LoadBufferFromFile( char* _oBuffer, unsigned int _bufferSize )
	{
		if(!m_IsFileOpen)
		{
			if(gLogManager)
			{
				gLogManager->WriteLog( "Can't Load to buffer, file " + m_FileName + " is not open" );
			}
			return false;
		}

		m_FileStream.read(_oBuffer,(std::streamsize)_bufferSize);

		return !IsError(true);
	}

	bool File::WriteBufferToFile( const char* _buffer, unsigned int _bufferSize )
	{
		if(!m_IsFileOpen)
		{
			if(gLogManager)
			{
				gLogManager->WriteLog( "Can't Load to buffer, file " + m_FileName + " is not open" );
			}
			return false;
		}

		m_FileStream.write(_buffer,(std::streamsize)_bufferSize);

		return !IsError(false);
	}

	bool File::Open(const TFlag32& _modes)
	{
		if(m_IsFileOpen)
		{
			if(gLogManager)
			{
				gLogManager->WriteLog( "File already open" );
			}
			return false;
		}


		m_FileStream.open(m_FileName.CString(), GetOpenMode(_modes) );
		if(!m_FileStream) 
		{
			if(gLogManager)
			{
				gLogManager->WriteLog( "File " + m_FileName + " could not open" );
			}
			Close();
			return false;
		}

		m_FileLength = GetFileLength(m_FileStream);
		m_IsFileOpen = true;
		
		return true;
	}

	bool File::Close()
	{
		if(!m_IsFileOpen)
		{
			return false;
		}
		//m_FileLength = GetFileLength(m_FileStream);
		m_FileStream.flush();
		m_FileStream.close();
		m_IsFileOpen = false;

		return true;
	}

	unsigned long File::GetFileLength(std::fstream& _file)
	{
		if( !_file.good() ) 
		{
			return 0;
		}

		//unsigned long pos = _file.tellg();
		_file.seekg(0,std::ios::end);
		unsigned long len = _file.tellg();
		_file.seekg(std::ios::beg);

		return len;
	}

	std::ios_base::open_mode File::GetOpenMode(const TFlag32& _modes)
	{
		std::ios_base::open_mode mode = 0;
		if( _modes.CheckFlag(eFM_Read) )
		{
			mode = mode | std::ios_base::in;
		}

		if( _modes.CheckFlag(eFM_Write) )
		{
			mode = mode | std::ios_base::out;
		}

		if( _modes.CheckFlag(eFM_Binary) )
		{
			mode = mode | std::ios_base::binary;
		}

		return mode;
	}

	bool File::IsError( bool _read ) const
	{
		if( m_FileStream.fail() )
		{
			if( gLogManager )
			{
				if( _read )
				{
					gLogManager->WriteLog( "Fail bit occurred during reading file '" + m_FileName + "'");
				}
				else
				{
					gLogManager->WriteLog( "Fail bit occurred during writing file '" + m_FileName + "'");
				}
			}

			return true;
		}

		if( m_FileStream.bad() )
		{
			if( gLogManager )
			{
				if( _read )
				{
					gLogManager->WriteLog( "Bad bit occurred during reading file '" + m_FileName + "'");
				}
				else
				{
					gLogManager->WriteLog( "Bad bit occurred during writing file '" + m_FileName + "'");
				}
			}

			return true;
		}

		if( m_FileStream.eof() )
		{
			if( gLogManager )
			{
				if( _read )
				{
					gLogManager->WriteLog( "Eof occurred during reading file '" + m_FileName + "'");
				}
				else
				{
					gLogManager->WriteLog( "Eof occurred during writing file '" + m_FileName + "'");
				}
			}

			return true;
		}

		return false;
	}

	String File::GetFileName(const String& _filePath)
	{
		String programName =  StringUtils::GetLastString(_filePath,'/');
		programName =  StringUtils::GetLastString(programName,'\\');
		return programName;
	}
}