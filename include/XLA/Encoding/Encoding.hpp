#ifndef XLA_BASEENCODING_INCLUDE
#define XLA_BASEENCODING_INCLUDE

#include <XLA/TypeDefine.hpp>
#include <XLA/Exception.hpp>

#if defined(ENCODING_DYNLIBRARY)
#	define ENCODING_DECL __declspec( dllexport )
#else
#	define ENCODING_DECL __declspec( dllimport )
#endif

#if !defined(ENCODING_DYNLIBRARY)

#if defined(_DEBUG)
#	pragma comment(lib,"EncodingDbg.lib")
#else
#	pragma comment(lib,"Encoding.lib")
#endif

#endif

struct conv_struct;

namespace XLA
{

	namespace Detail
	{
		typedef int (*func_mbtowc)( conv_struct* conv, unsigned int *pwc, const unsigned char *s, int n );
		typedef int (*func_wctomb)( conv_struct* conv, unsigned char *r, unsigned int wc, int n);
	}

	class Encoding;

	namespace Detail
	{
		enum ELoadEncoding
		{
			EEncodingLoad = 0,
			EEncodingNameNotFound,
			EEncodingCodePageNotFound
		};
		ENCODING_DECL UInt32 GetEncoding( CUInt32& codePage , Encoding** encoding );
		ENCODING_DECL UInt32 GetEncodingCodePage( CString& name , UInt32& codePage );
	}

	class EncodingDecodeException 
		: public RunTimeException
	{
		const Encoding* _encoding;
		CByte*    _bytePosition;
	public:
		XLA_REGEDIT_CLASS( EncodingDecodeException );
		EncodingDecodeException( const Encoding* encoding , CByte* bytePosition )
			: _encoding( encoding )
			, _bytePosition( bytePosition )
		{

		}

		virtual String ToString() const;
	};

	class EncodingNoSupportException 
		: public RunTimeException
	{
		String _name;
		String _message;
	public:
		XLA_REGEDIT_CLASS(EncodingNoSupportException);
		EncodingNoSupportException( CString& name , CString& message )
			: _name( name )
			, _message(message)
		{

		}

		virtual String ToString() const
		{
			return _message + _X("[") + _name + _X("]");
		}
	};

	class Encoding
		: public Object
	{

	public:
		typedef UInt32    CodePage;
		XLA_STATIC_CONST( CodePage , CodePageUTF7      = 65000  );
		XLA_STATIC_CONST( CodePage , CodePageUTF8      = 65001  );
		XLA_STATIC_CONST( CodePage , CodePageUTF32     = 12000  );
		XLA_STATIC_CONST( CodePage , CodePageUTF32BE   = 12001  );
		XLA_STATIC_CONST( CodePage , CodePageUnicode   = 1200   );
		XLA_STATIC_CONST( CodePage , CodePageUnicodeBE = 1201   );
	protected:
		XLA_STATIC_CONST( UInt32 , EMaxCharSize = 512 );
		XLA_STATIC_CONST( UInt32 , ECharSize    = sizeof(char) );

		XLA_STATIC_CONST( UInt32 , ELocalSucceed        = 0 );
		XLA_STATIC_CONST( UInt32 , ELocalBufferTooSmall = 1 );
		XLA_STATIC_CONST( UInt32 , ELocalInvalid        = 2 );

		virtual UInt32 LocalGetBytes( CStr  charBuffer , UInt32& charSize , Byte* byteBuffer , UInt32& byteSize ) const = 0;
		virtual UInt32 LocalGetChars( CByte* byteBuffer , UInt32& byteSize , Str   charBuffer , UInt32& charSize ) const = 0;

	public:
		XLA_REGEDIT_CLASS( Encoding );

		virtual Bool GetBytes( CStr charBuffer , UInt32& charSize , Byte* byteBuffer , UInt32& byteSize ) const
		{
			if( charBuffer == 0 )
				ExpThrow( NullArgumentException( _X("charBuffer") ) );
			if( byteBuffer == 0 )
				ExpThrow( NullArgumentException( _X("byteBuffer") ) );

			return LocalGetBytes( charBuffer , charSize , byteBuffer , byteSize ) != ELocalInvalid;
		}

		Bool GetChars( CByte* byteBuffer , UInt32& byteSize , Str   charBuffer , UInt32& charSize ) const
		{
			if( charBuffer == 0 )
				ExpThrow( NullArgumentException( _X("charBuffer") ) );
			if( byteBuffer == 0 )
				ExpThrow( NullArgumentException( _X("byteBuffer") ) );
			
			return LocalGetChars( byteBuffer , byteSize , charBuffer , charSize ) != ELocalInvalid;
		}

		String GetString( const Array<Byte>& byteBuffer )
		{
			return GetString( byteBuffer , byteBuffer.GetSize() );
		}

		static inline Encoding* GetEncoding( const CodePage& codePage )
		{
			Encoding* cvEncoding = 0;

			UInt32 code = Detail::GetEncoding( codePage , &cvEncoding );

			if( code != Detail::EEncodingLoad )
			{
				ExpThrow(
					(EncodingNoSupportException( 
						XLA::ToString( codePage ),
						Environment::GetResourceString(_X("Encoding_CodePageNoSupport"))
					)));
			}

			return cvEncoding;
		}

		static inline CodePage GetEncodingCodePage( CString& name )
		{
			Encoding* cvEncoding = 0;
			CodePage  codePage   = 0;

			UInt32 code = Detail::GetEncodingCodePage( name , codePage );

			if( code != Detail::EEncodingLoad )
			{
				ExpThrow( (EncodingNoSupportException( 
					name , 
					Environment::GetResourceString(_X("Encoding_NameNotSupport")) )) );
			}

			return codePage;

		}

		static inline Encoding* GetEncoding( CString& name )
		{
			return GetEncoding( GetEncodingCodePage(name) );
		}

		String GetString( CByte* byteBuffer , CUInt32& byteSize ) const
		{
			String cdString;
			cdString.reserve( byteSize / ECharSize );
			
			Array<Char> charBuffer( EMaxCharSize );

			UInt32 freeByteSize = byteSize;

			while( freeByteSize > 0  )
			{
				UInt32 charSize     = charBuffer.GetSize();
				UInt32 codeSize     = freeByteSize;

				if( !GetChars( byteBuffer , codeSize , charBuffer , charSize ) )
				{
					ExpThrow( (EncodingDecodeException( this , byteBuffer )) );
				}

				cdString.append( charBuffer , charSize );

				freeByteSize -= codeSize;
				byteBuffer   += codeSize;
			}

			return cdString;
		}

		inline static Encoding* UTF8()
		{
			return Encoding::GetEncoding( CodePageUTF8 );
		}

		inline static Encoding* Unicode()
		{
			return Encoding::GetEncoding( CodePageUnicode );
		}

		inline static Encoding* UnicodeBE()
		{
			return Encoding::GetEncoding( CodePageUnicodeBE );
		}

		inline static Encoding* UTF32()
		{
			return Encoding::GetEncoding( CodePageUTF32 );
		}

		inline static Encoding* UTF32BE()
		{
			return Encoding::GetEncoding( CodePageUTF32BE );
		}

		inline static Encoding* UTF7()
		{
			return Encoding::GetEncoding( CodePageUTF7 );
		}
	};

	inline String EncodingDecodeException::ToString() const
	{
		return _encoding->GetClassName();
	}
}

#endif