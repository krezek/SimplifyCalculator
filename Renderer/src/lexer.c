#include "pch.h"
#include "platform.h"

#include "lexer.h"

Token* token_init(int i, TokenTyp t, const wchar_t* s, size_t len)
{
	Token* tok = (Token*)malloc(sizeof(Token));
	assert(tok != 0);

	tok->_index = i;
	tok->_typ = t;

	tok->_str = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
	assert(tok->_str != 0);

	memset(tok->_str, 0, (len + 1) * sizeof(wchar_t));
	wcsncpy_s(tok->_str, len + 1, s, len);

	return tok;
}


void token_free(Token* tok)
{
	free(tok->_str);
	free(tok);
}

void token_print(const Token* tok)
{
	wprintf(L"(%d,%d,\"%s\")", tok->_index, tok->_typ, tok->_str);
}

TNode* tNode_init(Token* v)
{
	TNode* n = (TNode*)malloc(sizeof(TNode));
	assert(n != 0);

	n->_val = v;
	n->_next = NULL;

	return n;
}

void tNode_free(TNode* n)
{
	free(n);
}

TokensQueue* tokensQueue_init()
{
	TokensQueue* q = (TokensQueue*)malloc(sizeof(TokensQueue));
	assert(q != 0);

	q->_front = q->_rear = NULL;
	return q;
}

void tokensQueue_enqueue(TokensQueue* q, Token* v)
{
	TNode* tmp = tNode_init(v);

	if (q->_rear == NULL) {
		q->_front = q->_rear = tmp;
		return;
	}

	q->_rear->_next = tmp;
	q->_rear = tmp;
}

Token* tokensQueue_dequeue(TokensQueue* q)
{
	if (q->_front == NULL)
		return NULL;

	TNode* tmp = q->_front;

	q->_front = q->_front->_next;

	if (q->_front == NULL)
		q->_rear = NULL;

	Token* v = tmp->_val;

	tNode_free(tmp);

	return v;
}

Token* tokensQueue_front(TokensQueue* q)
{
	return q->_front->_val;
}

Token* tokensQueue_next(TokensQueue* q)
{
	if (q->_front && q->_front->_next)
	{
		return q->_front->_next->_val;
	}

	return NULL;
}

// Boolean return value
int tokensQueue_empty(TokensQueue* q)
{
	return (q->_front) ? 0 : 1;
}

void tokensQueue_print(TokensQueue* q)
{
	TNode* t = q->_front;
	while (t)
	{
		token_print(t->_val);
		t = t->_next;
	}
}

void tokensQueue_free(TokensQueue* q)
{
	while (!tokensQueue_empty(q))
	{
		Token* tok = tokensQueue_dequeue(q);
		token_free(tok);
	}

	free(q);
}

int lexer(const wchar_t* expr, TokensQueue* queue) {
	const static int lexT[8][6] = {
		//                     num    alpha  op     pas    smbl   blank
		//                     0      1      2      3      4      5
		/*0  new state*/    {  1,     3,     5,     6,     7,     0 },
		/*1  number*/       {  1,     1,     2,     2,     2,     2 },
		/*2  number final*/ {  0,     0,     0,     0,     0,     0 },
		/*3  alpha*/        {  3,     3,     4,     4,     4,     4 },
		/*4  alpha final*/  {  0,     0,     0,     0,     0,     0 },
		/*5  op final*/     {  0,     0,     0,     0,     0,     5 },
		/*6  pas final*/    {  0,     0,     0,     0,     0,     6 },
		/*7  smbl final*/   {  0,     0,     0,     0,     0,     7 }
	};

	static const wchar_t op[] = L"+-*/^!<>=|&%,;~:_\u2260\u2264\u2265";
	static const wchar_t pas[] = L"()[]{}";
	static const wchar_t smbl[] = L"\u221E\u03B1\u03B2\u03B3\u03B4\u03B5\u03B6\u03B7\u03B8\u03B9\u03BA"
									L"\u039B\u03BC\u03BD\u03BE\u03BF\u03C0\u03C1\u03C3\u03C4\u03C5\u03C6\u03C7\u03C8\u03C9";

	int ep = 0, cp = 0;
	size_t len;
	int col, lex_state = 0;
	wchar_t c;
	Token* tok;

	len = wcslen(expr);

	while (cp <= len) {
		c = expr[cp];

		if (iswdigit(c) || c == L'.')
			col = 0;
		else if (iswalpha(c) || c == L'\'')
			col = 1;
		else if (c && wcschr(op, c))
			col = 2;
		else if (c && wcschr(pas, c))
			col = 3;
		else if (c && wcschr(smbl, c))
			col = 4;
		else if (!c || iswblank(c))
			col = 5;
		else
		{
			wprintf(L"Unknown character '%c' '%d'\n", c, c);
			return -1;
		}

		lex_state = lexT[lex_state][col];

		switch (lex_state) {
		case 2: // Number
			tok = token_init(ep, NUMBER, expr + ep, cp - ep);
			tokensQueue_enqueue(queue, tok);

			--cp;
			lex_state = 0;
			break;

		case 4: // Alphabetic
			tok = token_init(ep, LITERAL, expr + ep, cp - ep);
			tokensQueue_enqueue(queue, tok);

			--cp;
			lex_state = 0;
			break;

		case 5: // operator
			tok = token_init(ep, OPERATOR, expr + ep, 1);
			tokensQueue_enqueue(queue, tok);

			lex_state = 0;
			break;

		case 6: // parentheses
			tok = token_init(ep, PARENTHESE, expr + ep, 1);
			tokensQueue_enqueue(queue, tok);

			lex_state = 0;
			break;

		case 7: // symbols
			tok = token_init(ep, SYMBOL, expr + ep, 1);
			tokensQueue_enqueue(queue, tok);

			lex_state = 0;
			break;
		}

		++cp;
		if (lex_state == 0)
			ep = cp;
	}

	return 0;
}

// Boolean return value
int accept_tok(Token* tok, TokenTyp typ, const wchar_t* str)
{
	if ((tok->_typ == typ) && (_wcsicmp(tok->_str, str) == 0))
		return 1;

	return 0;
}
