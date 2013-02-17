#include "EncodingLinkinfo.hpp"

#include "libiconv/utf32be.h"
#include "libiconv/utf32le.h"


#include <XLA/Encoding/UTF32Encoding.hpp>

#define code_mbtowc _mbtowc
#define code_wctomb _wctomb

#define CDEncoding UTF32Encoding

namespace XLA
{
	UTF32Encoding::UTF32Encoding( CBool& bigEnd )
	{
		if( bigEnd )
		{
			_mbtowc = utf32be_mbtowc;
			_wctomb = utf32be_wctomb;
		}
		else
		{
			_wctomb = utf32le_wctomb;
			_mbtowc = utf32be_mbtowc;
		}
	}

#include "EncodingFunction.hpp"

	namespace Detail
	{
		Encoding* CreateUTF32Encoding( CBool& bigEnd )
		{
			return new UTF32Encoding( bigEnd );
		}
	}
}
