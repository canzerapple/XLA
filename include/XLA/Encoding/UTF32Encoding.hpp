#ifndef XLA_UTF32ENCODING_INCLUDE
#define XLA_UTF32ENCODING_INCLUDE

#include <XLA/Encoding/Encoding.hpp>

namespace XLA
{
	class UTF32Encoding
		: public Encoding
	{
	protected:
		Detail::func_mbtowc _mbtowc;
		Detail::func_wctomb _wctomb;
	public:
		XLA_REGEDIT_CLASS( UTF32Encoding );

		UTF32Encoding( CBool& bigEnd );

		virtual UInt32 LocalGetBytes( CStr  charBuffer , UInt32& charSize , Byte* byteBuffer , UInt32& byteSize ) const;
		virtual UInt32 LocalGetChars( CByte* byteBuffer , UInt32& byteSize , Str   charBuffer , UInt32& charSize ) const;

	};

	namespace Detail
	{
		ENCODING_DECL Encoding* CreateUTF32Encoding( CBool& bigEnd );
	}

}

#endif