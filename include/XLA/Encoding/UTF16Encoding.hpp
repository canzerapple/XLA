#ifndef XLA_UTF16ENCODING_INCLUDE
#define XLA_UTF16ENCODING_INCLUDE

#include <XLA/Encoding/Encoding.hpp>

namespace XLA
{
	class UTF16Encoding
		: public Encoding
	{
	protected:
		Detail::func_mbtowc _mbtowc;
		Detail::func_wctomb _wctomb;
	public:
		XLA_REGEDIT_CLASS( UTF16Encoding );

		UTF16Encoding( CBool& bigEnd  );

		virtual UInt32 LocalGetBytes( CStr  charBuffer , UInt32& charSize , Byte* byteBuffer , UInt32& byteSize ) const;
		virtual UInt32 LocalGetChars( CByte* byteBuffer , UInt32& byteSize , Str   charBuffer , UInt32& charSize ) const;

	};

	namespace Detail
	{
		Encoding* CreateUTF16Encoding( CBool& bigEnd );
	}
}

#endif