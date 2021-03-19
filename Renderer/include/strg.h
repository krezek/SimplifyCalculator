#ifndef _STRG_H_
#define _STRG_H_

#define EXPAND 10

typedef struct
{
	wchar_t* _str;
	size_t _len;
	size_t _capacity;
} String;

String* String_init();
void String_free(String* s);
void String_cpy(String* s, const wchar_t* ws);
void String_cat(String* s, const wchar_t* ws);
void String_insert_s(String* s, int idx, const wchar_t* ws);
void String_insert_c(String* s, int idx, const wchar_t ch);
void String_delete_s(String* s, int idx1, int idx2);
void String_delete_c(String* s, int idx);

#endif /* _STRG_H_ */


