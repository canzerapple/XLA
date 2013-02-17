#ifndef XLA_UTF8ENCODING_INCLUDE
#define XLA_UTF8ENCODING_INCLUDE

#include <XLA/Encoding/Encoding.hpp>


namespace XLA
{
	class UTF8Encoding 
		: public Encoding
	{
	public:
		XLA_REGEDIT_CLASS( UTF8Encoding );

		virtual UInt32 LocalGetBytes( CStr charBuffer , UInt32& charSize , Byte* byteBuffer , UInt32& byteSize ) const;
		virtual UInt32 LocalGetChars( CByte* byteBuffer , UInt32& byteSize , Str charBuffer , UInt32& charSize ) const;

	};

	namespace Detail
	{
		ENCODING_DECL Encoding* CreateUTF8Encoding();
	}
}

#endif