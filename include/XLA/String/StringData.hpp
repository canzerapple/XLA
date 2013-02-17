#ifndef XLA_STRINGDATA_INCLUDE
#define XLA_STRINGDATA_INCLUDE

#include <XLA/String.hpp>

namespace XLA
{
	class StringData
		: public Object
	{
	protected:
		virtual void WriteLineData( CString& cdValue ) = 0;
		virtual void WriteData( CString& cdValue ) = 0;
	public:
		XLA_REGEDIT_CLASS( StringData );

		void WriteLine( CString& cdValue )
		{
			WriteLineData( cdValue );
		}

		void Write( CString& cdValue )
		{
			WriteData(cdValue);
		}

		template< class T0  >
		void WriteLine( CString& fmt , const T0& a0  )
		{ WriteLine( StringFormat( fmt , a0 ) ); } 

		template< class T0 ,class T1  >
		void WriteLine( CString& fmt , const T0& a0 ,const T1& a1  )
		{ WriteLine( StringFormat( fmt , a0 ,a1 ) ); } 

		template< class T0 ,class T1 ,class T2  >
		void WriteLine( CString& fmt , const T0& a0 ,const T1& a1 ,const T2& a2  )
		{ WriteLine( StringFormat( fmt , a0 ,a1 ,a2 ) ); } 

		template< class T0 ,class T1 ,class T2 ,class T3  >
		void WriteLine( CString& fmt , const T0& a0 ,const T1& a1 ,const T2& a2 ,const T3& a3  )
		{ WriteLine( StringFormat( fmt , a0 ,a1 ,a2 ,a3 ) ); } 

		template< class T0 ,class T1 ,class T2 ,class T3 ,class T4  >
		void WriteLine( CString& fmt , const T0& a0 ,const T1& a1 ,const T2& a2 ,const T3& a3 ,const T4& a4  )
		{ WriteLine( StringFormat( fmt , a0 ,a1 ,a2 ,a3 ,a4 ) ); } 

		template< class T0 ,class T1 ,class T2 ,class T3 ,class T4 ,class T5  >
		void WriteLine( CString& fmt , const T0& a0 ,const T1& a1 ,const T2& a2 ,const T3& a3 ,const T4& a4 ,const T5& a5  )
		{ WriteLine( StringFormat( fmt , a0 ,a1 ,a2 ,a3 ,a4 ,a5 ) ); } 

		template< class T0  >
		void Write( CString& fmt , const T0& a0  )
		{ WriteLine( StringFormat( fmt , a0 ) ); } 

		template< class T0 ,class T1  >
		void Write( CString& fmt , const T0& a0 ,const T1& a1  )
		{ WriteLine( StringFormat( fmt , a0 ,a1 ) ); } 

		template< class T0 ,class T1 ,class T2  >
		void Write( CString& fmt , const T0& a0 ,const T1& a1 ,const T2& a2  )
		{ WriteLine( StringFormat( fmt , a0 ,a1 ,a2 ) ); } 

		template< class T0 ,class T1 ,class T2 ,class T3  >
		void Write( CString& fmt , const T0& a0 ,const T1& a1 ,const T2& a2 ,const T3& a3  )
		{ WriteLine( StringFormat( fmt , a0 ,a1 ,a2 ,a3 ) ); } 

		template< class T0 ,class T1 ,class T2 ,class T3 ,class T4  >
		void Write( CString& fmt , const T0& a0 ,const T1& a1 ,const T2& a2 ,const T3& a3 ,const T4& a4  )
		{ WriteLine( StringFormat( fmt , a0 ,a1 ,a2 ,a3 ,a4 ) ); } 

		template< class T0 ,class T1 ,class T2 ,class T3 ,class T4 ,class T5  >
		void Write( CString& fmt , const T0& a0 ,const T1& a1 ,const T2& a2 ,const T3& a3 ,const T4& a4 ,const T5& a5  )
		{ WriteLine( StringFormat( fmt , a0 ,a1 ,a2 ,a3 ,a4 ,a5 ) ); } 

	};
}

#endif