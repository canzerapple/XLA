#ifndef XLA_ENCODINGINFO_INCLUDE
#define XLA_ENCODINGINFO_INCLUDE

#include <XLA/TypeDefine.hpp>

namespace XLA
{
	class EncodingInfo 
		: public Object
	{
	protected:
		String _name;
		String _displayName;
		UInt32 _codePage;
	public:
		XLA_REGEDIT_CLASS( EncodingInfo );

		EncodingInfo( CString& name , CString& displayName , CUInt32& codePage )
			: _name( name )
			, _displayName( displayName )
			, _codePage( codePage )
		{

		}

		EncodingInfo()
			: _codePage( 0xFFFFFFFF )
		{

		}

		String GetName() const
		{
			return _name;
		}

		String GetDisplayName() const
		{
			return _displayName;
		}

		UInt32 GetCodePage() const
		{
			return _codePage;
		}

		virtual String ToString() const
		{
			return GetDisplayName();
		}
	};
}

#endif