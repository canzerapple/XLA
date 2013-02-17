#ifndef XLA_FILE_STREAM_INCLUDE
#define XLA_FILE_STREAM_INCLUDE

#include <XLA/Exception.hpp>
#include <XLA/IO/Stream.hpp>
#include <XLA/IO/Core/PlatformApis.hpp>


namespace XLA
{
		enum EFileStream
		{
			errFileOpen,
			errFileRead,
			errFileWrite,
			errFileSeek,
			errFileSize
		};

		typedef const EFileStream CEFileStream;

		class FileStreamException :
			public SystemException
		{
			String      _fileName;
			EFileStream _eFileStream;
			
		public:
			XLA_REGEDIT_CLASS( FileStreamException );

			FileStreamException( CString& fileName , CEFileStream& ec )
				: _fileName( fileName )
				, _eFileStream( ec )
			{

			}

			virtual String ToString()
			{
				String errorMessage = SystemException::ToString();
				if( !_fileName.empty() )
					errorMessage = errorMessage + _X("(") + _fileName + _X(")");
				return errorMessage;
			}
		};
}

#define FileStreamThrow( ec ) ExpThrow( FileStreamException( _fileName , ec ) )


namespace XLA
{


		class FileStream :
			public Stream
		{
			String   _fileName;
			TFile    _fileHandle;
			UInt32   _openMask;
		protected:
			Bool Open( CString& name , UInt32 mode )
			{
				_fileName   = name;
				_fileHandle = ERROR_FD;
				_openMask   = mode;

				_fileHandle = OpenFile( _fileName , mode );
				
				return (_fileHandle != ERROR_FD);
			}

		public:
			XLA_REGEDIT_CLASS( FileStream );

			FileStream() 
				: _fileHandle( ERROR_FD ) , _openMask(0) 
			{
			}

			FileStream( CString& name , UInt32 mode = OPEN_READ )
			{
				Bool Succeed = Open( name , mode );

				if( !Succeed )
					FileStreamThrow( errFileOpen );
			}

			Void Close()
			{
				Bool Succeed = CloseFile( _fileHandle );
				_fileName   = String();
				_fileHandle = ERROR_FD;
			}

			Bool CanRead() const
			{
				return ( _openMask & OPEN_READ ) == OPEN_READ;
			}

			Bool CanWrite() const
			{
				return ( _openMask & OPEN_WRITE ) == OPEN_WRITE;
			}

			Bool CanSeek() const
			{
				return true;
			}

			Bool CanSize() const
			{
				return true;
			}

			UInt64 GetSize() const
			{
				UInt64 fileSize = 0;
				Bool   Succeed  = 
					SizeFile( 
						GetDescriptor() , 
						fileSize        
						);

				if(!Succeed)
					FileStreamThrow(errFileSize);

				return fileSize;
			}

			UInt32 ReadBlock( Byte* byteBuffer , UInt32 byteToRead )
			{
				UInt32 byteRead = 0;
				Bool   Succeed  = 
					ReadFile( 
						GetDescriptor() ,
						byteBuffer      ,
						byteToRead      ,
						byteRead
					);

				if(!Succeed)
					FileStreamThrow( errFileRead );

				return byteRead;
			}

			UInt32 WriteBlock( const Byte* byteBuffer , UInt32 byteToWrite )
			{
				UInt32 byteWrite = 0;
				Bool   Succeed   = 
					WriteFile( 
						GetDescriptor(),
						byteBuffer     ,
						byteToWrite    ,
						byteWrite
					);

				if(!Succeed)
					FileStreamThrow( errFileWrite );

				return byteWrite;
			}

			UInt64 Seek( UInt64 toMove , UInt32 moveMethod = BEGIN ) const
			{
				UInt64 newPosition = 0;
				Bool   Succeed     =
					SeekFile( 
						GetDescriptor() ,
						toMove          ,
						newPosition     ,
						moveMethod
					);

				if( !Succeed )
					FileStreamThrow( errFileSeek );

				return newPosition;
			}

			UInt64 GetPosition() const
			{
				return Seek( 0 , CURRENT );
			}

			TFile GetDescriptor() const
			{
				return _fileHandle;
			}

			String GetFileName() const
			{
				return _fileName;
			}

			Bool IsFileOpen() const
			{
				return _fileHandle != ERROR_FD;
			}

			Bool EndOfStream() const
			{
				return GetPosition() == GetSize();
			}

			virtual String ToString() const
			{
				if(_fileName.empty())
					return GetClassName();
				return _fileName;
			}

			~FileStream()
			{
				if( IsFileOpen() )
					Close();
			}
		};
}

#endif