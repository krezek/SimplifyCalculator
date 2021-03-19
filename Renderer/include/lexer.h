#ifndef _LEXER_H_
#define _LEXER_H_

#include <wchar.h>

typedef enum { UNDEFINED = 0, NUMBER, LITERAL, OPERATOR, PARENTHESE, SYMBOL } TokenTyp;

typedef struct
{
	int _index;
	TokenTyp _typ;
	wchar_t* _str;
} Token;

typedef struct _TNode
{
	Token* _val;
	struct _TNode* _next;
} TNode;

typedef struct
{
	TNode* _front, * _rear;
} TokensQueue;

Token* token_init(int i, TokenTyp t, const wchar_t* s, size_t len);
void token_free(Token* tok);
void token_print(const Token* tok);

TokensQueue* tokensQueue_init();
void tokensQueue_enqueue(TokensQueue* q, Token* v);
Token* tokensQueue_dequeue(TokensQueue* q);
Token* tokensQueue_front(TokensQueue* q);
Token* tokensQueue_next(TokensQueue* q);
int tokensQueue_empty(TokensQueue* q);
void tokensQueue_print(TokensQueue* q);
void tokensQueue_free(TokensQueue* q);

int lexer(const wchar_t* expr, TokensQueue* queue);

int accept_tok(Token* tok, TokenTyp typ, const wchar_t* str);

#endif /* _LEXER_H_ */

