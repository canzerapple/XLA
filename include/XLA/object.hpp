#ifndef XLA_OBJECT_INCLUDE
#define XLA_OBJECT_INCLUDE

#include <XLA/TypeDefine.hpp>

namespace XLA
{
	class Object
	{
	public:
		static String StaticGetClassName()
		{
			return _X("Object");
		}

		virtual String GetClassName() const
		{
			return Object::StaticGetClassName();
		}

		virtual String ToString() const
		{
			return GetClassName();
		}

		virtual ~Object()
		{

		}
	};

	inline String ToString( const Object* cdValue )
	{
		return cdValue->ToString();
	}

	inline String ToString( const Object& cdValue )
	{
		return cdValue.ToString();
	}
}

#define XLA_REGEDIT_CLASS( className ) \
	static String StaticGetClassName() \
	{\
		return _AT( className ); \
	}\
	virtual String GetClassName() const { return className##::StaticGetClassName(); }

namespace XLA
{
	template< class T , bool IsArray = false >
	struct AutoScope
	{
		const T* _object;
		
		AutoScope( const T& object ) 
			: _object( object )
		{

		}

		~AutoScope()
		{
			if(IsArray)
				delete [] _object;
			else
				delete _object;
		}
	};
}


namespace XLA
{
	template< class T >
	class Array
		:public Object
	{
		T*     _data;
		UInt32 _size;

		void Uninitalize()
		{
			if( _data != 0 )
			{
				delete [] _data;
				_size = 0;
			}
		}
	public:
		void Initalize( CUInt32& dataSize )
		{
			Uninitalize();
			_data = new T[dataSize];
			_size = dataSize;
		}

		XLA_REGEDIT_CLASS( Array<T> );

		Array( CUInt32& dataSize )
			: _data(0)
		{
			Initalize(dataSize);
		}

		Array( const T* data , CUInt32& dataSize )
			: _data(0)
		{
			Initalize( dataSize );
			for( UInt32 ix = 0 ; ix < dataSize ; ++ix )
				_data[ix] = data[ix];
		}

		Array( const Array& otherArray )
			: _data( 0 )
		{
			Initalize( otherArray.GetSize() );

			for( UInt32 idx = 0 ; idx < _size ; ++idx )
			{
				_data[idx] = otherArray[idx];
			}
		}

		Array()
		{
			_data = 0;
			_size = 0;
		}

		operator const T* () const
		{
			return _data;
		}

		operator T*()
		{
			return _data;
		}

		UInt32 GetSize() const
		{
			return _size;
		}

		~Array()
		{
			Uninitalize();
		}

		Char* GetData()
		{
			return _data;
		}
	};
}

#endif