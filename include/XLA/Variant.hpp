#ifndef XLA_VARIANT_INCLUDE
#define XLA_VARIANT_INCLUDE

#include <XLA/String.hpp>

namespace XLA
{

	struct UnknowVariant 
		: public Object
	{
	public:
		XLA_REGEDIT_CLASS( UnknowVariant );
	};

	String ToString( const UnknowVariant* unkVariant )
	{
		return UnknowVariant::StaticGetClassName();
	}



	XLA_STATIC_CONST( UnknowVariant* , nullVariant = 0 );

	class Variant :
		public Object
	{
	public:
		enum VarinatType
		{
			VARIANT_INT16,
			VARIANT_UINT16,
			VARIANT_INT32,
			VARIANT_UINT32,
			VARIANT_INT64,
			VARIANT_UINT64,
			VARIANT_FLOAT32,
			VARIANT_FLOAT64,
			VARIANT_OBJECT,
			VARIANT_UNKNOW
		};

	protected:
		union VariantValue
		{
			Int16     int16    ;
			UInt16    uint16   ;
			Int32     int32    ;
			UInt32    uint32   ;
			Int64     int64    ;
			UInt64    uint64   ;
			Float32   float32  ;
			Float64   float64  ;
			Object*   object   ;
		};

		VarinatType  _type;
		VariantValue _value;

		struct VariantToStringFunctor
		{
			template< class ValueT >
			String operator()( const ValueT& cdValue ) const
			{
				return XLA::ToString( cdValue );
			}
		};

	public:
		XLA_REGEDIT_CLASS( Variant );

		Variant()
		{
			_type         = VARIANT_UNKNOW;
			_value.uint64 = 0;
		}
		Variant( CInt16& cdValue )
		{
			_type        = VARIANT_INT16;
			_value.int16 = cdValue;
		}


		Variant( CUInt16& cdValue )
		{
			_type         = VARIANT_UINT16;
			_value.uint16 = cdValue;
		}


		Variant( CInt32& cdValue )
		{
			_type        = VARIANT_INT32;
			_value.int32 = cdValue;
		}


		Variant( CUInt32& cdValue )
		{
			_type         = VARIANT_UINT32;
			_value.uint32 = cdValue;
		}


		Variant( CInt64& cdValue )
		{
			_type        = VARIANT_INT64;
			_value.int64 = cdValue;
		}


		Variant( CUInt64& cdValue )
		{
			_type         = VARIANT_UINT64;
			_value.uint64 = cdValue;
		}


		Variant( CFloat32& cdValue )
		{
			_type          = VARIANT_FLOAT32;
			_value.float32 = cdValue;
		}


		Variant( CFloat64& cdValue )
		{
			_type          = VARIANT_FLOAT64;
			_value.float64 = cdValue;
		}

		Variant( Object* cdValue )
		{
			_type         = VARIANT_OBJECT;
			_value.object = cdValue;
		}

		Variant( const UnknowVariant* cdValue )
		{
			_type         = VARIANT_UNKNOW;
			_value.uint64 = 0;
		}

		VarinatType GetType() const
		{
			return _type;
		}

		template< class R , class CalcT >
		R ValueCalculation( const CalcT& calculation ) const
		{
			switch( _type )
			{
			case VARIANT_INT16:
				return calculation( _value.int16    );
			case VARIANT_UINT16:
				return calculation( _value.uint16   );
			case VARIANT_INT32:
				return calculation( _value.int32    );
			case VARIANT_UINT32:
				return calculation( _value.uint32   );
			case VARIANT_INT64:
				return calculation( _value.int64    );
			case VARIANT_UINT64:
				return calculation( _value.uint64   );
			case VARIANT_FLOAT32:
				return calculation( _value.float32  );
			case VARIANT_FLOAT64:
				return calculation( _value.float64  );
			case VARIANT_OBJECT:
				return calculation( _value.object   );
			default:
				return R();//Throw Exp
			}

			return R();//T
		}

		virtual String ToString() const
		{
			return ValueCalculation<String>( VariantToStringFunctor() );
		}
	};

	namespace VariantCalculation
	{

		template< class ValueT , class F >
		struct VariantLeftHand
		{
			const ValueT& _lhsValue;

			inline VariantLeftHand( const ValueT& refValue )
				: _lhsValue( refValue )
			{

			}

			template< class RightValueT >
			inline Variant operator()( const RightValueT& rhsValue ) const
			{
				return F::DoCalculation( _lhsValue , rhsValue );
			}

			inline Variant operator()( Object* object ) const
			{
				return Variant();
			}
		};

		template<  class F >
		struct VariantLeftHand< Object* , F >
		{
			inline VariantLeftHand( const Object* refValue )
			{

			}

			template< class RightValueT >
			inline Variant operator()( const RightValueT& rhsValue ) const
			{
				return Variant();
			}
		};


		template< class F >
		struct VariantRightHand
		{
			const Variant& _rhsValue;

			inline VariantRightHand( const Variant& refVariant ) 
				: _rhsValue( refVariant )
			{

			}

			template< class ValueT >
			inline Variant operator()( const ValueT& cdValue ) const
			{
				return _rhsValue.ValueCalculation<Variant>( VariantLeftHand<ValueT,F>(cdValue) );
			}
		};

		template< class TagL , class TagR >
		struct VariantProcess
		{

		};

		template< class Dev >
		struct VariantCalc
		{
			template< class ValueL , class ValueR > inline
			static Variant DoCalculation( const ValueL& lhsValue , const ValueR& rhsValue )
			{
				return Dev::Calculation( lhsValue , rhsValue);
			}
		};

		struct CalcAdd :
			public VariantCalc< CalcAdd >
		{
			template< class ValueL , class ValueR > inline
			static Variant Calculation( const ValueL& lhsValue , const ValueR rhsValue )
			{
				return (lhsValue + rhsValue);
			}
		};
	}

	inline Variant operator+( const Variant& lhsVariant , const Variant& rhsVariant )
	{
		return lhsVariant.ValueCalculation<Variant>(  
			VariantCalculation::VariantRightHand< 
							VariantCalculation::CalcAdd>( rhsVariant )  );
	}


}

#endif