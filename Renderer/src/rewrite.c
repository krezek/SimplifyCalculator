#include "pch.h"
#include "platform.h"

#include "rewrite.h"

typedef int (*RuleFunc) (Item* parent, int operand, Item** pItems);

int frac_parentheses_rl(Item* parent, int operand, Item** pItems);
int pow_parentheses_rl(Item* parent, int operand, Item** pItems);
int subscript_parentheses_rl(Item* parent, int operand, Item** pItems);

int apply_rule(Item* parent, int operand, Item** pItems, RuleFunc rf)
{
	if ((*pItems)->_isLeafFunc(*pItems)) return 0;

	int b = rf(parent, operand, pItems);

	Item** left = &((*pItems)->_left);
	Item** right = &((*pItems)->_right);

	if (*left && !(*left)->_isLeafFunc(*left))
		b = b | apply_rule(*pItems, 1, left, rf); // 1 for left

	if (*right && !(*right)->_isLeafFunc(*right))
		b = b | apply_rule(*pItems, 2, right, rf);  // 2 for right

	return b;
}

int apply_rewrite_rules(Item** pItems)
{
	apply_rule(NULL, 0, pItems, frac_parentheses_rl);
	apply_rule(NULL, 0, pItems, pow_parentheses_rl);
	apply_rule(NULL, 0, pItems, subscript_parentheses_rl);

	return 1;
}

int frac_parentheses_rl(Item* parent, int operand, Item** pItems)
{
	if ((*pItems)->_objectType == OBJ_Frac)
	{
		if ((*pItems)->_left && (*pItems)->_left->_objectType == OBJ_Parentheses)
		{
			Item* l = (*pItems)->_left;
			(*pItems)->_left = (*pItems)->_left->_left;
			l->_destroyFunc(l);
			free(l);
		}
		
		if ((*pItems)->_right && (*pItems)->_right->_objectType == OBJ_Parentheses)
		{
			Item* r = (*pItems)->_right;
			(*pItems)->_right = (*pItems)->_right->_left;
			r->_destroyFunc(r);
			free(r);
		}
	}

	return 0;
}

int pow_parentheses_rl(Item* parent, int operand, Item** pItems)
{
	if ((*pItems)->_objectType == OBJ_Pow)
	{
		if ((*pItems)->_right && (*pItems)->_right->_objectType == OBJ_Parentheses)
		{
			Item* r = (*pItems)->_right;
			(*pItems)->_right = (*pItems)->_right->_left;
			r->_destroyFunc(r);
			free(r);
		}
	}

	return 0;
}

int subscript_parentheses_rl(Item* parent, int operand, Item** pItems)
{
	if ((*pItems)->_objectType == OBJ_Subscript)
	{
		if ((*pItems)->_right && (*pItems)->_right->_objectType == OBJ_Parentheses)
		{
			Item* r = (*pItems)->_right;
			(*pItems)->_right = (*pItems)->_right->_left;
			r->_destroyFunc(r);
			free(r);
		}
	}

	return 0;
}


