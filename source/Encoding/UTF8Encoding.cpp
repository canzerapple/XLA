#include "EncodingLinkinfo.hpp"

#include "libiconv/utf8.h"

#include <XLA/Encoding/UTF8Encoding.hpp>

#define code_mbtowc utf8_mbtowc
#define code_wctomb utf8_wctomb

#define CDEncoding UTF8Encoding

namespace XLA
{

	#include "EncodingFunction.hpp"

	namespace Detail
	{
		Encoding* CreateUTF8Encoding()
		{
			return new UTF8Encoding();
		}
	}
}