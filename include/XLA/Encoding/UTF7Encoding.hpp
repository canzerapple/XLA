#ifndef XLA_UTF7ENCODING_INCLUDE
#define XLA_UTF7ENCODING_INCLUDE

#include <XLA/Encoding/Encoding.hpp>

namespace XLA
{
	class UTF7Encoding
		: public Encoding
	{
	protected:
		void* _conv;
	public:
		XLA_REGEDIT_CLASS( UTF7Encoding );

		virtual UInt32 LocalGetBytes( CStr  charBuffer , UInt32& charSize , Byte* byteBuffer , UInt32& byteSize ) const;
		virtual UInt32 LocalGetChars( CByte* byteBuffer , UInt32& byteSize , Str   charBuffer , UInt32& charSize )const;
	};

	namespace Detail
	{
		ENCODING_DECL Encoding* CreateUTF7Encoding();
	}
}

#endif