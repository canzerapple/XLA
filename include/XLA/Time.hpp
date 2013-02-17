#ifndef XLA_CDTIME_INCLUDE
#define XLA_CDTIME_INCLUDE

#include <time.h>
#include <XLA/TypeDefine.hpp>
#include <XLA/Exception.hpp>
namespace XLA
{
	typedef time_t TTime;
	typedef tm     TTm;


	class TimeConvertException
		: public RunTimeException
	{

	};

	template< UInt32 radix >
	class TimeRadix
		: public Object
	{
		UInt32 _times;
	public:

		XLA_REGEDIT_CLASS( TimeRadix<radix> );

		TimeRadix( CUInt32& count )
			: _times( count )
		{

		}

		static TimeRadix<radix> CreateFromTime( const TTime& time )
		{
			return TimeRadix( time / radix );
		}

		operator UInt32() const
		{
			return _times;
		}

		TTime GetSeconds() const
		{
			return _times * radix;
		}

		const static UInt32 times = radix;
	};

	typedef TimeRadix<1>                      Seconds;
	typedef TimeRadix<Seconds::times * 60 >   Minutes;
	typedef TimeRadix<Minutes::times * 60 >   Hours  ;
	typedef TimeRadix<Hours::times   * 24 >   Days   ;

	class TimeDelta
		: public Object
	{
	public:
		XLA_REGEDIT_CLASS( TimeDelta );

		TTime _time;

		TimeDelta( const TTime& time = 0 )
			: _time(time)
		{

		}

		operator TTime() const
		{
			return _time;
		}

		Days GetDays() const
		{
			return Days::CreateFromTime(_time);
		}

		Hours GetHours() const
		{
			return Hours::CreateFromTime(_time);
		}

		Minutes GetMinutes() const
		{
			return Minutes::CreateFromTime(_time);
		}

		TTime GetSeconds() const
		{
			return _time;
		}


	};



	class Time
		: public Object
	{
		TTime _time;

	public:
		XLA_REGEDIT_CLASS( Time );

		Time( const TTime& time )
			: _time(time)
		{

		}

		Time( CInt32& year , CInt32& month , CInt32& day , CInt32& hour = 0 , CInt32& minute = 0 , CInt32& second = 0 )
		{
			TTm time     = {0,0,0,0,0,0,0,0,0};
			time.tm_hour = hour;
			time.tm_min  = minute;
			time.tm_mday = day;
			time.tm_mon  = month;
			time.tm_sec  = second;
			time.tm_year = year - 1900;
			
			_time = TmToTime( time );
		}

		Time( CString& fmt , CString& timeData )
		{
			_time = CreateFromString( fmt , timeData );
		}

	protected:
		TTm GetTM() const
		{
			return TimeToTm(_time);
		}
	public:

		Int32 GetYear() const
		{
			return GetTM().tm_year;
		}

		Int32 GetMonth() const
		{
			return GetTM().tm_mon;
		}

		Int32 GetDay() const
		{
			return GetTM().tm_mday;
		}

		Int32 GetDayOfYear() const
		{
			return GetTM().tm_yday;
		}

		Int32 GetDayOfWeek() const
		{
			return GetTM().tm_wday;
		}

		Int32 GetHour() const
		{
			return GetTM().tm_hour;
		}

		Int32 GetMinute() const
		{
			return GetTM().tm_min;
		}

		Int32 GetSecond() const
		{
			return GetTM().tm_sec;
		}

		Time GetDate(CInt32& hour = 0 , CInt32& minute = 0 , CInt32& second = 0) const
		{
			TTm timeStruce = GetTM();

			timeStruce.tm_hour = hour;
			timeStruce.tm_sec  = second;
			timeStruce.tm_min  = minute;

			return TmToTime(timeStruce);
		}
	public:

		static Int32 TimeStringToData( CChar* & strTime , CUInt32& fmtSize )
		{
			String cvTime;
			cvTime.reserve(4);

			for( UInt32 idx = 0 ; idx < fmtSize ; ++idx )
			{
					if( strTime[idx] == _X('\0') || !IsDigit( strTime[idx] ) )
						ExpThrow( TimeConvertException() );
			}

			cvTime.append( strTime , fmtSize );
			strTime += (fmtSize-1);

			return XLA::ToInt32( cvTime );
		}

		static TTime TmToTime( TTm& timeStruct )
		{
			TTime time = mktime( &timeStruct );

			if( time == -1 )
				ExpThrow( TimeConvertException() );

			return time;
		}

		static TTm TimeToTm( const TTime& time )
		{
			TTm    timeStruct;
			TErrno eno = localtime_s( &timeStruct , &time );

			if( eno != 0 )
				ExpThrow( StandardException( eno ) );

			return timeStruct;
		}

		static TTime Today( CInt32& hour = 0 , CInt32& minute = 0 , CInt32& second = 0 )
		{
			TTime time = Now();
			TTm   timeStruce = TimeToTm(time);
			timeStruce.tm_hour = hour;
			timeStruce.tm_sec  = second;
			timeStruce.tm_min  = minute;

			return TmToTime(timeStruce);
		}

		

		static TTime CreateFromString( CString& fmt , CString& timeData )
		{
			TTm   time = {0,0,0,0,0,0,0,0,0};
			CChar* strFormat = fmt.c_str();
			CChar* strTime   = timeData.c_str();

			Char   strCurrent = *strFormat;

			while ( strCurrent != 0 )
			{
				strCurrent = *strFormat;

				if(strCurrent == _X('%'))
				{
					strCurrent = *(++strFormat);
					switch(strCurrent)
					{
					case 'Y':
						time.tm_year = TimeStringToData(strTime,4) - 1900;
						break;
					case 'm':
						time.tm_mon  = TimeStringToData(strTime,2);
						break;
					case 'd':
						time.tm_mday = TimeStringToData(strTime,2);
						break;
					case 'H':
						time.tm_hour = TimeStringToData(strTime,2);
						break;
					case 'M':
						time.tm_min = TimeStringToData(strTime,2);
						break;
					case 'S':
						time.tm_sec = TimeStringToData(strTime,2);
						break;
					}
					int ix= 5;
				}
				else if( *strTime != *strFormat )
				{
					ExpThrow( TimeConvertException() );
				}
				strTime   ++ ;
				strFormat ++;
			}

			return TmToTime( time );
		}

		virtual String ToString( CString& fmt ) const
		{
			CChar* fmtString = fmt.c_str();
			UInt32 fmtCount  = 0;

			while (*fmtString != _X('\0'))
			{
				if( *(fmtString++) == _X('%s') )
					fmtCount++;
			}

			Array<Char> timeString( fmt.size() + 4*fmtCount );

			TTm time = TimeToTm( _time );

			wcsftime( timeString.GetData() , timeString.GetSize() , fmt.c_str() , &time );

			return timeString.GetData();
		}

		virtual String ToString() const
		{
			return ToString(_X("%Y-%m-%d %H:%M:%S"));
		}

		operator TTime() const
		{
			return _time;
		}

		Time& operator+=( const TTime& time )
		{
			_time += time;
			return *this;
		}

		static TTime Now()
		{
			return time(0);
		}

		TTime GetSeconds() const
		{
			return _time;
		}
	};



	template< UInt32 radix >
	TTime operator+( const TTime& time , const TimeRadix<radix>& times )
	{
		return time + times.GetSeconds();
	}

	template< UInt32 radix >
	TTime operator+( const TimeRadix<radix>& times , const TTime& time )
	{
		return times.GetSeconds() + time;
	}

	template< UInt32 lhsRadix , UInt32 rhsRadix >
	TTime operator+( const TimeRadix<lhsRadix>& lhs , const TimeRadix<rhsRadix>& rhs )
	{
		return lhs.GetSeconds() + rhs.GetSeconds();
	}
}

#endif // !XLA_TIME_INCLUDE
