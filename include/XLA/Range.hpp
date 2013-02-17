#ifndef XLA_RANGE_INCLUDE
#define XLA_RANGE_INCLUDE

namespace XLA
{
	template< class T >
	class RangeIterator
	{
	protected:
		Bool _end;
		T    _current;
		T   _step;

	public:
		RangeIterator( const T& current , const T& step , CBool& end )
			: _end( end )
			, _current(current)
			, _step(step)
		{

		}

		T operator*() const
		{
			return _current;
		}

		T& operator*()
		{
			return _current;
		}

		T operator++()
		{
			if( _end )
				return _current;
			_current += _step;
			return _current;
		}

		Bool operator!=( const RangeIterator& rhs ) const
		{
			if(_current > rhs._current)
				return false;
			return true;
		}
	};

	template< class T >
	class _Range
	{
	protected:
		T _begin;
		T _end;
		T _step;
	public:
		typedef T ValueType;
		_Range( const T& begin , const T& end , const T& step = 1 )
			: _begin( begin )
			, _end( end ) 
			, _step(step)
		{

		}

		RangeIterator<T> begin() const
		{
			return RangeIterator<T>( _begin , _step , false );
		}

		RangeIterator<T> end() const
		{
			return RangeIterator<T>( _end , _step , true );
		}
	};

	template< class T >
	_Range<T> Range( const T& begin , const T& end , const T& setp = 1 )
	{
		return _Range<T>( begin , end , setp );
	}

	template< class T >
	_Range< T > Range( const T& end )
	{
		return _Range< T >( T(0) , end );
	}
}

#endif // !XLA_RANGE_INCLUDE
