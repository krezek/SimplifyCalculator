#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <wchar.h>

#include "linux_def.h"

int wcscpy_s(wchar_t* _Destination, size_t _SizeInWords, wchar_t const* _Source)
{
	wcscpy(_Destination, _Source);
	return 0;
}

int wcscat_s(wchar_t* _Destination, size_t _SizeInWords, wchar_t const* _Source)
{
	wcscat(_Destination, _Source);
	return 0;
}

int wcsncpy_s(wchar_t* _Destination, size_t _SizeInWords, wchar_t const* _Source, size_t _MaxCount)
{
	wcsncpy(_Destination, _Source, _MaxCount);
	return 0;
}

int _wcsicmp(wchar_t const* _String1, wchar_t const* _String2)
{
	return wcscmp(_String1, _String2);
}