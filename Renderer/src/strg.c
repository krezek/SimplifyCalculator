#include "pch.h"
#include "platform.h"

#include "strg.h"

String* String_init()
{
	String* s = (String*)malloc(sizeof(String));
	assert(s != NULL);

	s->_len = wcslen(L"");
	s->_capacity = EXPAND + 1;

	s->_str = (wchar_t*)malloc(sizeof(wchar_t) * (s->_capacity));
	assert(s->_str != NULL);

	wcscpy_s(s->_str, s->_capacity, L"");

	return s;
}

void String_free(String* s)
{
	if (s->_str != NULL)
		free(s->_str);
	free(s);
}

void String_cpy(String* s, const wchar_t* ws)
{
	size_t len = wcslen(ws) + s->_len;
	size_t cap = (len / EXPAND + ((len % EXPAND > 0) ? 1 : 0)) * EXPAND + 1;

	s->_len = len;


	if (cap > s->_capacity)
	{
		s->_capacity = cap;

		wchar_t* str = (wchar_t*)realloc(s->_str, sizeof(wchar_t) * s->_capacity);
		assert(str != NULL);
		s->_str = str;
	}

	wcscpy_s(s->_str, s->_capacity, ws);
}

void String_cat(String* s, const wchar_t* ws)
{
	size_t len = wcslen(ws) + s->_len;
	size_t cap = (len / EXPAND + ((len % EXPAND > 0) ? 1 : 0)) * EXPAND + 1;

	s->_len = len;


	if (cap > s->_capacity)
	{
		s->_capacity = cap;

		wchar_t* str = (wchar_t*)realloc(s->_str, sizeof(wchar_t) * s->_capacity);
		assert(str != NULL);
		s->_str = str;
	}
	
	wcscat_s(s->_str, s->_capacity, ws);
}

void String_insert_s(String* s, int idx, const wchar_t* ws)
{
	assert((idx >= 0) && (idx <= s->_len));

	size_t len = s->_len + wcslen(ws);
	size_t cap = (len / EXPAND + ((len % EXPAND > 0) ? 1 : 0)) * EXPAND + 1;

	s->_len = len;

	wchar_t* str = (wchar_t*)malloc(sizeof(wchar_t) * cap);
	assert(str != NULL);

	wcsncpy_s(str, cap, s->_str, idx);
	wcscat_s(str, cap, ws);
	wcscat_s(str, cap, s->_str + idx);

	free(s->_str);

	s->_capacity = cap;
	s->_str = str;
}

void String_insert_c(String* s, int idx, const wchar_t ch)
{
	wchar_t str[2];
	str[0] = ch;
	str[1] = 0;

	String_insert_s(s, idx, str);
}

void String_delete_s(String* s, int idx1, int idx2)
{
	assert((idx1 >= 0) && (idx1 <= s->_len));
	assert((idx2 >= 0) && (idx2 <= s->_len));
	assert(idx1 < idx2);

	size_t len = s->_len - (idx2 - idx1);
	size_t cap = (len / EXPAND + ((len % EXPAND > 0) ? 1 : 0)) * EXPAND + 1;

	wchar_t* str = (wchar_t*)malloc(sizeof(wchar_t) * cap);
	assert(str != NULL);

	wcsncpy_s(str, cap, s->_str, idx1);
	wcscat_s(str, cap, s->_str + idx2);

	free(s->_str);
	s->_capacity = cap;
	s->_str = str;
	s->_len = len;
}

void String_delete_c(String* s, int idx)
{
	String_delete_s(s, idx, idx + 1);
}

