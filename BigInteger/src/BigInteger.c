#include "pch.h"
#include "framework.h"

#include "BigInteger.h"

BigInteger* BigInteger_init_default()
{
	BigInteger* bg = (BigInteger*)malloc(sizeof(BigInteger));
	assert(bg != NULL);

	bg->_sign = 0;
	bg->_size = 0;
	bg->_value = NULL;

	return bg;
}

void BigInteger_free(BigInteger* bg)
{
	if(bg->_value)
		free(bg->_value);
	free(bg);
}

/*extern int ASM_BigInteger_get_size(__int64 strLen);
extern void ASM_BigInteger_fill(BigInteger* bg, wchar_t* str, __int64 strLen);
extern void ASM_BigInteger_bcd_to_bin(BigInteger* bg, __int64* value);
extern void ASM_BigInteger_bin_to_bcd(BigInteger* bg, __int64* value);
extern void ASM_BigInteger_to_char(__int64* value, wchar_t* str, __int64 size);

extern void ASM_BigInteger_sum(BigInteger* a, BigInteger* b, BigInteger* c);

BigInteger* BigInteger_init_str(wchar_t* str)
{
	BigInteger* bg = (BigInteger*)malloc(sizeof(BigInteger));
	assert(bg != NULL);

	bg->_sign = 0;
	bg->_size = 0;
	bg->_value = NULL;

	/*wchar_t* str2 = str;
	if (str2[0] == L'-')
	{
		bg->_sign = 1;
		str2++;
	}
	else if (str2[0] == L'+')
	{
		str2++;
	}

	do {
		if (str2[0] == L'0')
		{
			str2++;
		}
	} while (str2[0] == L'0');

	size_t length = wcslen(str2);
	bg->_size = ASM_BigInteger_get_size(length);

	bg->_value = (__int64*)malloc(sizeof(__int64) * bg->_size);
	assert(bg->_value != NULL);

	ASM_BigInteger_fill(bg, str2, length);

	__int64* value = (__int64*)malloc(sizeof(__int64) * bg->_size);
	assert(value != NULL);

	ASM_BigInteger_bcd_to_bin(bg, value);
	
	free(bg->_value);

	bg->_value = value;
	
	return bg;
}

void BigInteger_free(BigInteger* bg)
{
	//free(bg->_value);
	free(bg);
}


wchar_t* BigInteger_to_char(BigInteger* bg)
{
	// clone bg
	BigInteger* bg2 = (BigInteger*)malloc(sizeof(BigInteger));
	assert(bg2 != NULL);

	bg2->_sign = bg->_sign;
	bg2->_size = bg->_size;
	bg2->_value = (__int64*)malloc(sizeof(__int64) * bg2->_size);
	assert(bg2->_value != NULL);

	memcpy_s(bg2->_value, bg2->_size * sizeof(__int64), bg->_value, bg->_size * sizeof(__int64));

	__int64* value = (__int64*)malloc(sizeof(__int64) * bg->_size);
	assert(value != NULL);

	memset(value, 0, sizeof(__int64) * bg->_size);

	ASM_BigInteger_bin_to_bcd(bg2, value);

	free(bg2->_value);
	free(bg2);

	wchar_t* str = (wchar_t*)malloc(sizeof(wchar_t) * (bg->_size * sizeof(__int64) * 2 + 1));
	assert(str != NULL);

	ASM_BigInteger_to_char(value, str, bg->_size);

	free(value);

	return str;
}

BigInteger* BigInteger_sum(BigInteger* a, BigInteger* b)
{
	BigInteger* c = (BigInteger*)malloc(sizeof(BigInteger));
	assert(c != NULL);

	c->_size = max(a->_size, b->_size) + 1;

	c->_value = (__int64*)malloc(sizeof(__int64) * c->_size);
	assert(c->_value != NULL);

	memset(c->_value, 0, sizeof(__int64) * c->_size);

	ASM_BigInteger_sum(a, b, c);
	
	return c;
}
*/