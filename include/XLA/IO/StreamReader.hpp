#ifndef XLA_STREAM_READER_INCLUDE
#define XLA_STREAM_READER_INCLUDE

#include <algorithm>
#include <XLA/IO/FileStream.hpp>
#include <XLA/Encoding/Encoding.hpp>

namespace XLA
{
	class StreamReader
	{

		Encoding*   _encoding;

		Array<Char> _chars;
		UInt32      _charSize;
		UInt32      _charCurrent;

		Array<Byte> _bytes;
		UInt32      _byteSize;
		UInt32      _byteCurrent;

		Stream*     _stream;
		UInt64      _streamSize;
		UInt64      _streamCurrent;

		UInt32      _byteMax;
		UInt32      _charMax;

		Bool        _useStreamSize;
		Bool        _selfStream;

	protected:

		XLA_STATIC_CONST( UInt32 , MAX_BYTES = 1024*4 );
		XLA_STATIC_CONST( Char   , RELINE    = Char('\n') );

		UInt32 GetFreeByteSize()
		{
			return _byteSize - _byteCurrent;
		}

		Byte* GetCurrentByteData()
		{
			return _bytes + _byteCurrent;
		}

		UInt32 GetFreeCharSize()
		{
			return _charSize - _charCurrent;
		}

		Byte* GetEndOfByte()
		{
			return _bytes + _byteSize;
		}

		Char* GetEndOfChar()
		{
			return _chars + _charSize;
		}


		Bool EndOfByteStream() const
		{
			if( _useStreamSize )
				return _streamCurrent == _streamSize;
			else
				return _stream->EndOfStream();
		}

		Bool EndOfChar() const
		{
			return _charCurrent >= _charSize;
		}

		Bool EndOfByte() const
		{
			return _byteCurrent == _byteCurrent;
		}



		Char* GetCurrentCharData()
		{
			return _chars + _charCurrent;
		}

		void ReadByteToBuffer()
		{
			UInt32 moveSize = 0;

			if( _byteCurrent < _byteSize )
			{
				moveSize = GetFreeByteSize();
				memcpy( _bytes , GetCurrentByteData() , moveSize );
			}

			UInt32 readSize = _byteMax - moveSize;
			UInt32 bytesize = 0; 
			
			_byteSize = moveSize;
			bytesize  = _stream->Read( _bytes + _byteSize , readSize );

			_streamCurrent+= bytesize;
			_byteSize     += bytesize;
			_byteCurrent   = 0;
		}

		void ReadCharToBuffer()
		{
			UInt32 moveSize = 0;
			if( _charCurrent < _charSize )
			{
				moveSize = GetFreeCharSize();
				memcpy( _chars , GetCurrentCharData() , moveSize*sizeof(Char) );
			}

			_charCurrent  = 0;
			_charSize     = moveSize;

			Byte* currentByte = GetCurrentByteData();
			Char* currentChar = GetCurrentCharData();



			UInt32 charSize = _charMax - moveSize;
			UInt32 byteSize = GetFreeByteSize();


			if( ! _encoding->GetChars( currentByte , byteSize , currentChar , charSize ) )
			{
				int ix=5;
				//throw Stream Error!
			}

			_charSize    += charSize;
			_byteCurrent += byteSize;
		}

		void ReadBOM()
		{
			Byte*  bom = GetCurrentByteData();
			UInt32 fb  = GetFreeByteSize(); 

			if( fb >= 4 )
			{
				if( bom[0] == 0x00 && bom[1] == 0x00 && bom[2] == 0xFE && bom[3] == 0xFF )
				{
					_encoding = Encoding::UTF32BE();
					_byteCurrent += 4;
					return;
				}
				else if( bom[0] == 0xFF && bom[1] == 0xFE && bom[2] == 0x00 && bom[3] == 0x00 )
				{
					_encoding = Encoding::UTF32();
					_byteCurrent += 4;
					return;
				}
			}

			if( fb >= 3 )
			{
				if( bom[0] == 0xEF && bom[1] == 0xBB && bom[2] == 0xBF )
				{
					_encoding = Encoding::UTF8();
					_byteCurrent += 3;
					return;
				}
			}

			if( fb >= 2 )
			{
				if( bom[0] == 0xFE && bom[1] == 0xFF )
				{
					_encoding = Encoding::UnicodeBE();
					_byteCurrent += 2;
					return;
				}
				else if( bom[0] == 0xFF && bom[1] == 0xFE )
				{
					_encoding = Encoding::Unicode();
					_byteCurrent += 2;
					return;
				}
			}
		}

		void Initalize( UInt32 maxbytes , Encoding* encoding , Stream* stream , Bool autoReadBom , Bool selfStream )
		{
			_byteMax     = maxbytes;
			_charMax     = maxbytes*2;
			_encoding    = encoding;
			_stream      = stream;
			_selfStream  = selfStream;

			if( stream == 0 )
				ExpThrow( NullArgumentException(_X("stream")));
			if( encoding == 0 )
				ExpThrow( NullArgumentException(_X("encoding")));

			if( !_stream->CanRead() )
				ExpThrow( (
					ArgumentException( 
						Environment::GetResourceString(_X("Argument_StreamCanRead")) ,
						_X("stream") ))
						);

			if(_stream->CanSize())
			{
				_streamSize    = _stream->GetSize();
				_useStreamSize  = true;
			}
			else
			{
				_streamSize    = 0;
				_useStreamSize  = false;
			}

			_streamCurrent = _stream->GetPosition();

			_chars.Initalize( _charMax ); 
			_bytes.Initalize( _byteMax );

			_charSize = 0;
			_byteSize = 0;

			_byteCurrent = 0;
			_charCurrent = 0;

			ReadByteToBuffer();

			if( autoReadBom )
				ReadBOM();

			ReadCharToBuffer();

		}

	public:

		StreamReader( CString& name , Bool bom = true )
		{
			Stream*   stream   = new FileStream( name );
			Encoding* encoding = Encoding::UTF8();
			Initalize( MAX_BYTES , encoding , stream , bom , true );
		}

		StreamReader( CString& name , Encoding* encoding , Bool bom = false )
		{
			Stream* stream = new FileStream( name );
			Initalize( MAX_BYTES , encoding , stream , bom , true );
		}

		StreamReader( CString& name , Encoding* encoding , UInt32 maxbytes , Bool bom = false )
		{
			Stream* stream = new FileStream( name );
			Initalize( maxbytes , encoding , stream , bom , true );
		}

		StreamReader( Stream* stream , Encoding* encoding )
		{
			Initalize( MAX_BYTES , encoding , stream , true , false );
		}

		StreamReader( Stream* stream , Encoding* encoding , UInt32 maxbytes )
		{
			Initalize( maxbytes , encoding , stream , true , false );
		}

		Bool EndOfStream()
		{
			return EndOfChar() && EndOfByte() && EndOfByteStream();
		}

		void ReadToEnd(String& s_read )
		{

			s_read.clear();
			if( GetFreeCharSize() > 0 )
			{
				s_read.append( GetCurrentCharData() , GetFreeCharSize() );
				_charCurrent += GetFreeCharSize();
			}

			while( !EndOfByteStream() || !EndOfByte() )
			{
				ReadByteToBuffer();
				ReadCharToBuffer();
				s_read.append( GetCurrentCharData() , GetFreeCharSize() );
				_charCurrent += GetFreeCharSize();
			}
		}

		String ReadToEnd()
		{
			String s_read;
			ReadToEnd( s_read );
			return s_read;
		}

		void ReadLine( String& s_read )
		{
			s_read.clear();
			while(true)
			{
				Char* token = std::find( GetCurrentCharData() , GetEndOfChar() , RELINE );
				UInt32 s_size = static_cast< UInt32 >( token - GetCurrentCharData() );
				s_read.append( GetCurrentCharData() , s_size );
				_charCurrent += s_size + 1;

				if( token != GetEndOfChar() )
				{
					return;
				}
				else
				{
					if( !EndOfByteStream() || !EndOfByte() || !EndOfChar() )
					{
						ReadByteToBuffer();
						ReadCharToBuffer();
					}
					else
					{
						return;
					}
				}
			}

		}

		String ReadLine()
		{
			String s_read;
			ReadLine( s_read );
			return s_read;

		}

		UInt64 GetSize()
		{
			return _streamSize;
		}

		UInt64 GetPosition()
		{
			return _streamCurrent;
		}

		Stream* GetStream()
		{
			return _stream;
		}

		void Close()
		{
			if( _stream != 0 )
				_stream->Close();
		}

	};
}

#endif