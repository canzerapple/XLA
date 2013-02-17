#ifndef XLA_TIME_INCLUDE
#define XLA_TIME_INCLUDE

#include <XLA/TypeDefine.hpp>
#include <Windows.h>
#include <ctime>

namespace XLA
{

	typedef std::clock_t        TClock;
	typedef const std::clock_t CTClock;

	class ClockDelta
	{
		TClock _delta;

	public:
		ClockDelta( CTClock& delta )
			: _delta( delta )
		{

		}
	};


	class Clock
	{
		TClock _clock;
	public:
		Clock()
			: _clock( clock() )
		{

		}

		Clock( CTClock& clock )
			: _clock( clock )
		{

		}

		virtual String ToString() const
		{
			return XLA::ToString( (Int32)_clock );
		}
	};
}

#endif