#include "pch.h"
#include "platform.h"

#include "parser.h"
#include "lexer.h"
#include "rewrite.h"

///////////////////////// Stack //////////////////////////

typedef struct _StackNode
{
	Item* _data;
	struct _StackNode* _next;
} StackNode;

static StackNode* stackNode_init(Item* d)
{
	StackNode* stackNode = (StackNode*)malloc(sizeof(StackNode));
	stackNode->_data = d;
	stackNode->_next = NULL;
	return stackNode;
}

static int stack_isEmpty(StackNode* root)
{
	return !root;
}

static void stack_push(StackNode** root, Item* data)
{
	StackNode* stackNode = stackNode_init(data);
	stackNode->_next = *root;
	*root = stackNode;
}

static Item* stack_pop(StackNode** root)
{
	if (stack_isEmpty(*root))
		return NULL;
	StackNode* temp = *root;
	*root = (*root)->_next;
	Item* popped = temp->_data;
	free(temp);

	return popped;
}

///////////////////////// Stack end //////////////////////

//    list : equ {(",") equ} .
//    equ : expr {("=") expr} .
//    expr : term {("+"|"-") term} .
//    term : factor {("*"|"/"|"%") factor} .
//    factor: "-" factorial | "+" factorial | factorial .
//    factorial : power "!" | power
//    power : subscript { "^" subscript } .
//    subscript : primary { "_" primary } .
//    primary : func "(" expr ")" | "(" expr ")" | number | literal | symbol .
//    func : "Sigma" | "Integrate" | "Derivative" | CommFunc

static int list(Item** pItems, TokensQueue* tokens);
static int equ(Item** pItems, TokensQueue* tokens);
static int expr(Item** pItems, TokensQueue* tokens);
static int term(Item** pItems, TokensQueue* tokens);
static int factor(Item** pItems, TokensQueue* tokens);
static int factorial(Item** pItems, TokensQueue* tokens);
static int power(Item** pItems, TokensQueue* tokens);
static int subscript(Item** pItems, TokensQueue* tokens);
static int primary(Item** pItems, TokensQueue* tokens);
static int func(Item** pItems, TokensQueue* tokens);
static int number(Item** pItems, TokensQueue* tokens);
static int literal(Item** pItems, TokensQueue* tokens);
static int symbol(Item** pItems, TokensQueue* tokens);

int parse(Item** pItems, const wchar_t* s)
{
	Item* nodes = NULL;
	int rs = -1;
	TokensQueue* q = tokensQueue_init();

	if (lexer(s, q))
	{
		tokensQueue_free(q);
		return -1;
	}
	
	//tokensQueue_print(q);

	rs = list(&nodes, q);
	if (rs)
	{
		if (nodes)
		{
			ItemTree_free(&nodes);
		}
	}
	else
	{
		*pItems = nodes;
		apply_rewrite_rules(pItems);
	}

	tokensQueue_free(q);

	return rs;
}

//    list : equ {(",") equ} .
static int list(Item** pItems, TokensQueue* tokens)
{
	int rs;
	Item* node = NULL;

	rs = equ(&node, tokens);

	if (!rs)
	{
		if (!tokensQueue_empty(tokens))
		{
			Token* tok = tokensQueue_front(tokens);
			while (accept_tok(tok, OPERATOR, L","))
			{
				tok = tokensQueue_dequeue(tokens);
				token_free(tok);

				Item* r_node = NULL;
				rs = equ(&r_node, tokens);
				node = (Item*)ItemList_init(node, r_node);
				if (rs)
					break;
				
				if (tokensQueue_empty(tokens)) break;
				tok = tokensQueue_front(tokens);
			}
		}
	}

	*pItems = node;

	return rs;
}

//    equ : expr {("=") expr} .
static int equ(Item** pItems, TokensQueue* tokens)
{
	int rs;
	Item* node = NULL;

	rs = expr(&node, tokens);

	if (!rs)
	{
		if (!tokensQueue_empty(tokens))
		{
			Token* tok = tokensQueue_front(tokens);
			if (accept_tok(tok, OPERATOR, L"=") |
				accept_tok(tok, OPERATOR, L"<") | 
				accept_tok(tok, OPERATOR, L">") | 
				accept_tok(tok, OPERATOR, L"\u2260") | 
				accept_tok(tok, OPERATOR, L"\u2264") | 
				accept_tok(tok, OPERATOR, L"\u2265"))
			{
				tok = tokensQueue_dequeue(tokens);
				Item* r_node = NULL;
				rs = expr(&r_node, tokens);
				node = (Item*)ItemEqu_init(node, r_node, tok->_str[0]);
				token_free(tok);
			}
		}
	}

	*pItems = node;

	return rs;
}

//    expr : term {("+"|"-") term} .
static int expr(Item** pItems, TokensQueue* tokens)
{
	int rs;
	Item* node = NULL;

	rs = term(&node, tokens);

	if (!rs)
	{
		if (!tokensQueue_empty(tokens))
		{
			Token* tok = tokensQueue_front(tokens);
			while (accept_tok(tok, OPERATOR, L"+") ||
				accept_tok(tok, OPERATOR, L"-"))
			{
				tok = tokensQueue_dequeue(tokens);
				if (_wcsicmp(L"+", tok->_str) == 0)
				{
					Item* l_node = NULL;
					rs = term(&l_node, tokens);
					node = (Item*)ItemAdd_init(node, l_node);
					if (rs)
					{
						token_free(tok);
						break;
					}
				}
				else if (_wcsicmp(L"-", tok->_str) == 0)
				{
					Item* l_node = NULL;
					rs = term(&l_node, tokens);
					node = (Item*)ItemSub_init(node, l_node);
					if (rs)
					{
						token_free(tok);
						break;
					}
				}

				token_free(tok);
				if (tokensQueue_empty(tokens)) break;
				tok = tokensQueue_front(tokens);
			}
		}
	}

	*pItems = node;

	return rs;
}

//    term : factor {("*"|"/"|"%") factor} .
static int term(Item** pItems, TokensQueue* tokens)
{
	int rs;
	Item* node = NULL;

	rs = factor(&node, tokens);

	if (!rs)
	{
		if (!tokensQueue_empty(tokens))
		{
			Token* tok = tokensQueue_front(tokens);
			while (accept_tok(tok, OPERATOR, L"*") ||
				accept_tok(tok, OPERATOR, L"/"))
			{
				tok = tokensQueue_dequeue(tokens);
				if (_wcsicmp(L"*", tok->_str) == 0)
				{
					Item* l_node = NULL;
					rs = factor(&l_node, tokens);
					node = (Item*)ItemMult_init(node, l_node);
					if (rs)
					{
						token_free(tok);
						break;
					}
				}
				else if (_wcsicmp(L"/", tok->_str) == 0)
				{
					Item* l_node = NULL;
					rs = factor(&l_node, tokens);
					node = (Item*)ItemFrac_init(node, l_node);
					if (rs)
					{
						token_free(tok);
						break;
					}
				}

				token_free(tok);
				if (tokensQueue_empty(tokens)) break;
				tok = tokensQueue_front(tokens);
			}
		}
	}

	*pItems = node;

	return rs;
}

//    factor: "-" factorial | "+" factorial | factorial .
static int factor(Item** pItems, TokensQueue* tokens)
{
	Item* node = NULL;
	Token* signToken = NULL;
	Token* tok = NULL;
	int rs;

	if (!tokensQueue_empty(tokens))
	{
		tok = tokensQueue_front(tokens);
		if (accept_tok(tok, OPERATOR, L"+") ||
			accept_tok(tok, OPERATOR, L"-"))
		{
			signToken = tokensQueue_dequeue(tokens);
		}
	}

	rs = factorial(&node, tokens);

	if (signToken)
	{
		node = (Item*)ItemSign_init(node, signToken->_str[0]);
		token_free(signToken);
	}
	
	*pItems = node;

	return rs;
}

//    factorial : power "!" | power
static int factorial(Item** pItems, TokensQueue* tokens)
{
	Item* node = NULL;
	int rs;

	rs = power(&node, tokens);

	if (!rs)
	{
		if (!tokensQueue_empty(tokens))
		{
			Token* tok = tokensQueue_front(tokens);
			if (accept_tok(tok, OPERATOR, L"!"))
			{
				tok = tokensQueue_dequeue(tokens);
				node = (Item*)ItemFactorial_init(node);
				token_free(tok);
			}
		}
	}

	*pItems = node;

	return rs;
}

//    power : subscript { "^" subscript } .
static int power(Item** pItems, TokensQueue* tokens)
{
	Item* node = NULL;
	int rs;
	StackNode* root = NULL;

	rs = subscript(&node, tokens);

	if (!rs)
	{
		stack_push(&root, node);

		if (!tokensQueue_empty(tokens))
		{
			Token* tok = tokensQueue_front(tokens);
			while (accept_tok(tok, OPERATOR, L"^"))
			{
				tok = tokensQueue_dequeue(tokens);

				Item* l_node = NULL;
				rs = subscript(&l_node, tokens);
				stack_push(&root, l_node);
				if (rs)
				{
					token_free(tok);
					break;
				}

				token_free(tok);
				if (tokensQueue_empty(tokens)) break;
				tok = tokensQueue_front(tokens);
			}
		}

		node = stack_pop(&root);

		while (!stack_isEmpty(root))
		{
			Item* tmp = stack_pop(&root);
			node = (Item*)ItemPow_init(tmp, node);
		}
	}

	*pItems = node;

	return rs;
}

//    subscript : primary { "_" primary } .
static int subscript(Item** pItems, TokensQueue* tokens)
{
	Item* node = NULL;
	int rs;
	StackNode* root = NULL;

	rs = primary(&node, tokens);

	if (!rs)
	{
		stack_push(&root, node);

		if (!tokensQueue_empty(tokens))
		{
			Token* tok = tokensQueue_front(tokens);
			while (accept_tok(tok, OPERATOR, L"_"))
			{
				tok = tokensQueue_dequeue(tokens);

				Item* l_node = NULL;
				rs = primary(&l_node, tokens);
				stack_push(&root, l_node);
				if (rs)
				{
					token_free(tok);
					break;
				}

				token_free(tok);
				if (tokensQueue_empty(tokens)) break;
				tok = tokensQueue_front(tokens);
			}
		}

		node = stack_pop(&root);

		while (!stack_isEmpty(root))
		{
			Item* tmp = stack_pop(&root);
			node = (Item*)ItemSubscript_init(tmp, node);
		}
	}
	
	*pItems = node;

	return rs;
}

//    primary : func "(" expr ")" | "(" expr ")" | number | literal | symbol.
static int primary(Item** pItems, TokensQueue* tokens)
{
	Item* node = NULL;
	int rs = -1;

	if (!tokensQueue_empty(tokens))
	{
		Token* tok = tokensQueue_front(tokens);
		Token* ntok = tokensQueue_next(tokens);

		if (tok->_typ == LITERAL && ntok && 
			(accept_tok(ntok, PARENTHESE, L"(") || accept_tok(ntok, PARENTHESE, L"[") || accept_tok(ntok, PARENTHESE, L"{")))
		{
			rs = func(&node, tokens);
			*pItems = node;
			return rs;
		}
		else if (accept_tok(tok, PARENTHESE, L"(")) // (expr)
		{
			tok = tokensQueue_dequeue(tokens);
			token_free(tok);

			rs = expr(&node, tokens);
			*pItems = node;

			if (tokensQueue_empty(tokens)) // expect ")"
			{
				wprintf(L"Expected: \")\"\n");
				return -1;
			}

			tok = tokensQueue_front(tokens);
			if (!accept_tok(tok, PARENTHESE, L")"))
			{
				wprintf(L"Expected: \")\"\n");
				return -1;
			}

			*pItems = (Item*)ItemParentheses_init(*pItems);

			tok = tokensQueue_dequeue(tokens);
			token_free(tok);

			return rs;
		}
		else if (tok->_typ == NUMBER) // Number
		{
			rs = number(&node, tokens);
			*pItems = node;
			return rs;
		}
		else if (tok->_typ == LITERAL) // Literal
		{
			rs = literal(&node, tokens);
			*pItems = node;
			return rs;
		}
		else if (tok->_typ == SYMBOL) // SYMBOL
		{
			rs = symbol(&node, tokens);
			*pItems = node;
			return rs;
		}
	}
	else
		rs = -1;

	*pItems = node;

	return rs;
}

//    func : "Sigma" | "Integrate" | "Derivative" | CommFunc
static int func(Item** pItems, TokensQueue* tokens)
{
	Item* node = NULL;
	int rs = -1;

	Token* tok_func_name = tokensQueue_dequeue(tokens); // Function name

	Token* tok_par = tokensQueue_dequeue(tokens); // Parenthes
	token_free(tok_par);

	if (accept_tok(tok_func_name, LITERAL, L"Sigma"))
	{
		rs = expr(&node, tokens);
		if (!rs)
		{
			Item* bNode = NULL;
			Item* tNode = NULL;

			Token* bTok = tokensQueue_front(tokens);
			if (accept_tok(bTok, OPERATOR, L";"))
			{
				bTok = tokensQueue_dequeue(tokens);
				token_free(bTok);

				rs = list(&bNode, tokens);

				if (!rs)
				{
					Token* tTok = tokensQueue_front(tokens);
					if (accept_tok(tTok, OPERATOR, L";"))
					{
						tTok = tokensQueue_dequeue(tokens);
						token_free(tTok);

						rs = list(&tNode, tokens);
					}
				}
			}

			node = (Item*)ItemSigma_init(node, bNode, tNode);
		}

		*pItems = node;
	}
	else if (accept_tok(tok_func_name, LITERAL, L"Integrate"))
	{
		rs = expr(&node, tokens);
		if (!rs)
		{
			Item* rNode = NULL;
			Item* bNode = NULL;
			Item* tNode = NULL;

			Token* rTok = tokensQueue_front(tokens);

			if (accept_tok(rTok, OPERATOR, L";"))
			{
				rTok = tokensQueue_dequeue(tokens);
				token_free(rTok);

				rs = literal(&rNode, tokens);

				if (!rs)
				{
					Token* bTok = tokensQueue_front(tokens);

					if (accept_tok(bTok, OPERATOR, L";"))
					{
						bTok = tokensQueue_dequeue(tokens);
						token_free(bTok);

						rs = list(&bNode, tokens);

						if (!rs)
						{
							Token* tTok = tokensQueue_front(tokens);
							if (accept_tok(tTok, OPERATOR, L";"))
							{
								tTok = tokensQueue_dequeue(tokens);
								token_free(tTok);

								rs = list(&tNode, tokens);
							}
						}
					}
				}
			}

			node = (Item*)ItemIntegrate_init(node, rNode, bNode, tNode);
		}

		*pItems = node;
	}
	else if (accept_tok(tok_func_name, LITERAL, L"Derivative"))
	{
		rs = expr(&node, tokens);
		if (!rs)
		{
			Item* rNode = NULL;
			//Item* tNode = NULL;

			Token* rTok = tokensQueue_front(tokens);
			if (accept_tok(rTok, OPERATOR, L";"))
			{
				rTok = tokensQueue_dequeue(tokens);
				token_free(rTok);

				rs = literal(&rNode, tokens);
				rNode = (Item*)ItemFrac_init(
					(Item*)ItemLiteral_init(L"d"),
					rNode
				);

				/*if (!rs)
				{
					Token* tTok = tokensQueue_front(tokens);
					if (accept_tok(tTok, OPERATOR, L";"))
					{
						tTok = tokensQueue_dequeue(tokens);
						token_free(tTok);

						rs = list(&tNode, tokens);
					}
				}*/
			}

			node = (Item*)ItemDerivative_init(node, rNode);
		}

		*pItems = node;
	}
	else if (accept_tok(tok_func_name, LITERAL, L"Limit"))
	{
		rs = expr(&node, tokens);
		if (!rs)
		{
			Item* t1Node = NULL;
			Item* t2Node = NULL;

			Token* bTok = tokensQueue_front(tokens);
			if (accept_tok(bTok, OPERATOR, L";"))
			{
				bTok = tokensQueue_dequeue(tokens);
				token_free(bTok);

				rs = list(&t1Node, tokens);

				if (!rs)
				{
					Token* tTok = tokensQueue_front(tokens);
					if (accept_tok(tTok, OPERATOR, L";"))
					{
						tTok = tokensQueue_dequeue(tokens);
						token_free(tTok);

						rs = expr(&t2Node, tokens);
					}
				}
			}

			node = (Item*)ItemLimit_init(node, t1Node, t2Node);
		}

		*pItems = node;
	}
	else if (accept_tok(tok_func_name, LITERAL, L"Sqrt"))
	{
		rs = expr(&node, tokens);
		node = (Item*)ItemSqrt_init(node);
		*pItems = node;
	}
	else
	{
		Item* rNode = NULL;

		rs = expr(&node, tokens);
		if (!rs)
		{
			Token* rTok = tokensQueue_front(tokens);
			if (accept_tok(rTok, OPERATOR, L";"))
			{
				rTok = tokensQueue_dequeue(tokens);
				token_free(rTok);

				rs = expr(&rNode, tokens);
			}
		}

		node = (Item*)ItemCommFunc_init(node, rNode, tok_func_name->_str);
		*pItems = node;
	}

	if (tokensQueue_empty(tokens)) // expect "Parenthes"
	{
		wprintf(L"Expected: \"Parenthes\"\n");
		token_free(tok_func_name);
		return -1;
	}

	tok_par = tokensQueue_front(tokens);
	if (!accept_tok(tok_par, PARENTHESE, L")") &&
		!accept_tok(tok_par, PARENTHESE, L"]") &&
		!accept_tok(tok_par, PARENTHESE, L"}"))
	{
		wprintf(L"Expected: \"Parenthes\"\n");
		token_free(tok_func_name);
		return -1;
	}

	tok_par = tokensQueue_dequeue(tokens); // Parenthes
	token_free(tok_par);

	token_free(tok_func_name);

	return rs;
}

//    number .
static int number(Item** pItems, TokensQueue* tokens)
{
	Item* node = NULL;
	int rs = -1;

	Token* tok = tokensQueue_dequeue(tokens);
	node = (Item*)ItemNumber_init(tok->_str, 0);
	token_free(tok);

	*pItems = node;
	return (rs = 0);
}

//    symbol .
static int symbol(Item** pItems, TokensQueue* tokens)
{
	Item* node = NULL;
	int rs = -1;

	Token* tok = tokensQueue_dequeue(tokens);
	node = (Item*)ItemSymbol_init(tok->_str[0]);
	token_free(tok);

	*pItems = node;
	return (rs = 0);
}

//    literal .
static int literal(Item** pItems, TokensQueue* tokens)
{
	Item* node = NULL;
	int rs = -1;

	Token* tok = tokensQueue_dequeue(tokens);
	node = (Item*)ItemLiteral_init(tok->_str);
	token_free(tok);

	*pItems = node;
	return (rs = 0);
}
