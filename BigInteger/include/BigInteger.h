#ifndef _BIGINTEGER_H_
#define _BIGINTEGER_H_

#ifdef _WIN64
typedef __int64 dtype;
typedef unsigned __int64 udtype;
#else
typedef __int32 dtype;
typedef unsigned __int32 udtype;
#endif

typedef struct
{
	dtype _sign;
	dtype _size;
	udtype* _value;
} BigInteger;

BigInteger* BigInteger_init_default();
void BigInteger_free(BigInteger* bg);

void asm_bgnt_set_size(BigInteger* bg, dtype len);


/*BigInteger* BigInteger_init_str(wchar_t* str);
void BigInteger_free(BigInteger* bg);

wchar_t* BigInteger_to_char(BigInteger* bg);

BigInteger* BigInteger_sum(BigInteger* a, BigInteger* b);
*/
#endif /* _BIGINTEGER_H_ */
