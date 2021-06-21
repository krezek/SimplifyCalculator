#ifndef _STRG_H_
#define _STRG_H_

#define EXPAND 100

typedef struct
{
	wchar_t* _str;
	size_t _len;
	size_t _capacity;
	size_t _index;
} String;

String* String_init();
void String_free(String* s);
void String_cpy(String* s, const wchar_t* ws);
void String_cat(String* s, const wchar_t* ws);
void String_insert_s(String* s, size_t idx, const wchar_t* ws);
void String_insert_c(String* s, size_t idx, const wchar_t ch);
void String_delete_s(String* s, size_t idx1, size_t idx2);
void String_delete_c(String* s, size_t idx);

#endif /* _STRG_H_ */


