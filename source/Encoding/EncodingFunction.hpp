#if !defined( CDEncoding )
#error "Use EncodingFunction.hpp must define CDEncoding First!"
#endif

#if !defined( code_mbtowc )
#error "Use EncodingFunction.hpp must define code_mbtowc First!"
#endif

#if !defined( code_wctomb )
#error "Use EncodingFunction.hpp must define code_wctomb First!"
#endif

UInt32 CDEncoding::LocalGetBytes( CStr charBuffer , UInt32& charSize , Byte* byteBuffer , UInt32& byteSize ) const
{
	conv_struct b_mbtowc;
	b_mbtowc.istate = 0;
	b_mbtowc.ostate = 0;

	conv_struct b_wctomb;
	b_wctomb.istate = 0;
	b_wctomb.ostate = 0;

	CByte* charByte = (CByte*)charBuffer;
	Byte* codeByte = ( Byte*)byteBuffer;

	CByte* charEndStream = (CByte*)(charBuffer + charSize);
	CByte* codeEndStream = (CByte*)(byteBuffer + byteSize);

	UInt32 charCount = 0;
	UInt32 codeCount = 0;
	ucs4_t unicode   = 0;

	UInt32 charFreeSize = charSize*sizeof(Char);
	UInt32 codeFreeSize = byteSize;

	Int32  code = 0;

	while( charByte !=  charEndStream )
	{		
		code = local_mbtowc( &b_mbtowc , &unicode , charByte , charFreeSize );

		if( code < 0 )
			goto GetBytesExit;

		charByte     += code;
		charCount    += code;
		charFreeSize -= code;

		code = code_wctomb( &b_wctomb , codeByte , unicode , codeFreeSize );

		if( code < 0 )
			goto GetBytesExit;

		codeByte     += code;
		codeCount    += code;
		codeFreeSize -= code;
	}

GetBytesExit:
	if( code < 0 && !CONV_BUFFERTOSMALL(code) )
		return ELocalInvalid;;

	charSize = charCount / sizeof( Char );
	byteSize = codeCount;

	if( CONV_BUFFERTOSMALL(code) )
		return ELocalBufferTooSmall;

	return ELocalSucceed;
}

UInt32 CDEncoding::LocalGetChars( CByte* byteBuffer , UInt32& byteSize , Str charBuffer , UInt32& charSize ) const
{
	conv_struct b_mbtowc;
	b_mbtowc.istate = 0;
	b_mbtowc.ostate = 0;

	conv_struct b_wctomb;
	b_wctomb.istate = 0;
	b_wctomb.ostate = 0;

	CByte* codeByte = byteBuffer;
	Byte* charByte = (Byte*)(charBuffer);

	CByte* codeEndStream = byteBuffer + byteSize;
	Byte* charEndStream = (Byte*)(charBuffer+charSize);

	UInt32 charCount = 0;
	UInt32 codeCount = 0;

	Int32  code = 0;

	UInt32 charFreeSize = charSize*sizeof(Char);
	UInt32 codeFreeSize = byteSize;

	ucs4_t unicode = 0;

	while( codeByte != codeEndStream )
	{
		code = code_mbtowc( &b_mbtowc , &unicode , codeByte , codeFreeSize );

		if(code < 0)
			goto GetCharsExit;

		codeFreeSize -= code;
		codeCount    += code;
		codeByte     += code;

		code = local_wctomb( &b_wctomb , charByte , unicode , charFreeSize );

		if(code < 0)
			goto GetCharsExit;

		charFreeSize -= code;
		charCount    += code;
		charByte     += code;


	}

GetCharsExit:
	if( code < 0 && !CONV_BUFFERTOSMALL(code) )
		return ELocalInvalid;

	byteSize = codeCount;
	charSize = charCount / sizeof(Char);

	if( !CONV_BUFFERTOSMALL( code ) )
		return ELocalBufferTooSmall;

	return ELocalSucceed;
}

#undef CDEncoding
#undef code_mbtowc
#undef code_wctomb