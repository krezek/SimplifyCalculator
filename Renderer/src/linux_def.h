#ifndef _LINUX_DEF_H_
#define _LINUX_DEF_H_

int wcscpy_s(wchar_t* _Destination, size_t _SizeInWords, wchar_t const* _Source);
int wcscat_s(wchar_t* _Destination, size_t _SizeInWords, wchar_t const* _Source);
int wcsncpy_s(wchar_t* _Destination, size_t _SizeInWords, wchar_t const* _Source, size_t _MaxCount);
int _wcsicmp(wchar_t const* _String1, wchar_t const* _String2);

#endif /* _LINUX_DEF_H_ */
