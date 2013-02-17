#include "EncodingLinkinfo.hpp"

#include "libiconv/utf7.h"

#include <XLA/Encoding/UTF7Encoding.hpp>

#define code_mbtowc utf7_mbtowc
#define code_wctomb utf7_wctomb

#define CDEncoding UTF7Encoding

namespace XLA
{

#include "EncodingFunction.hpp"

	namespace Detail
	{
		Encoding* CreateUTF7Encoding()
		{
			return new UTF7Encoding();
		}
	}
}