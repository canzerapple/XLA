#include "EncodingLinkinfo.hpp"

#include "libiconv/utf16be.h"


#include <XLA/Encoding/UTF16Encoding.hpp>

#define code_mbtowc _mbtowc
#define code_wctomb _wctomb

#define CDEncoding UTF16Encoding

namespace XLA
{
	UTF16Encoding::UTF16Encoding( CBool& bigEnd )
	{
		if( bigEnd )
		{
			_mbtowc = utf16be_mbtowc;
			_wctomb = utf16be_wctomb;
		}
		else
		{
			_mbtowc = utf16le_mbtowc;
			_wctomb = utf16le_wctomb;
		}
	}

#include "EncodingFunction.hpp"

	namespace Detail
	{
		Encoding* CreateUTF16Encoding( CBool& bigEnd )
		{
			return new UTF16Encoding( bigEnd );
		}
	}
}