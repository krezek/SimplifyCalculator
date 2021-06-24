#include "platform.h"

#include "items.h"

void Item_SetBounds(Item* _this, int x, int y, int width, int height, int baseLine);
void Item_setFont(Item* _this, FontHandle hFont);
int Item_width(Item* _this, Graphics* gfx);
int Item_height(Item* _this, Graphics* gfx);
int Item_baseLine(Item* _this, Graphics* gfx);
void Item_calcCoordinate(Item* _this, Graphics* gfx, int x, int y);
void Item_drawFocusRect(Item* item, Graphics* gfx);
void Item_draw(Item* _this, Graphics* gfx);
void Item_setFocus(Item* _this, unsigned char b);
unsigned char Item_getFocus(Item* _this);

int ItemLiteral_width(Item* _this, Graphics* gfx);
int ItemLiteral_height(Item* _this, Graphics* gfx);
int ItemLiteral_baseLine(Item* _this, Graphics* gfx);
void ItemLiteral_draw(Item* _this, Graphics* gfx);

int ItemNumber_width(Item* _this, Graphics* gfx);
int ItemNumber_height(Item* _this, Graphics* gfx);
int ItemNumber_baseLine(Item* _this, Graphics* gfx);
void ItemNumber_draw(Item* _this, Graphics* gfx);

int ItemSymbol_width(Item* _this, Graphics* gfx);
int ItemSymbol_height(Item* _this, Graphics* gfx);
int ItemSymbol_baseLine(Item* _this, Graphics* gfx);
void ItemSymbol_draw(Item* _this, Graphics* gfx);

int ItemList_width(Item* _this, Graphics* gfx);
int ItemList_height(Item* _this, Graphics* gfx);
int ItemList_baseLine(Item* _this, Graphics* gfx);
void ItemList_calcCoordinate(Item* _this, Graphics* gfx, int x, int y);
void ItemList_draw(Item* _this, Graphics* gfx);

int ItemEqu_width(Item* _this, Graphics* gfx);
int ItemEqu_height(Item* _this, Graphics* gfx);
int ItemEqu_baseLine(Item* _this, Graphics* gfx);
void ItemEqu_calcCoordinate(Item* _this, Graphics* gfx, int x, int y);
void ItemEqu_draw(Item* _this, Graphics* gfx);

int ItemAdd_width(Item* _this, Graphics* gfx);
int ItemAdd_height(Item* _this, Graphics* gfx);
int ItemAdd_baseLine(Item* _this, Graphics* gfx);
void ItemAdd_calcCoordinate(Item* _this, Graphics* gfx, int x, int y);
void ItemAdd_draw(Item* _this, Graphics* gfx);

int ItemSub_width(Item* _this, Graphics* gfx);
int ItemSub_height(Item* _this, Graphics* gfx);
int ItemSub_baseLine(Item* _this, Graphics* gfx);
void ItemSub_calcCoordinate(Item* _this, Graphics* gfx, int x, int y);
void ItemSub_draw(Item* _this, Graphics* gfx);

int ItemMult_width(Item* _this, Graphics* gfx);
int ItemMult_height(Item* _this, Graphics* gfx);
int ItemMult_baseLine(Item* _this, Graphics* gfx);
void ItemMult_calcCoordinate(Item* _this, Graphics* gfx, int x, int y);
void ItemMult_draw(Item* _this, Graphics* gfx);

int ItemFrac_width(Item* _this, Graphics* gfx);
int ItemFrac_height(Item* _this, Graphics* gfx);
int ItemFrac_baseLine(Item* _this, Graphics* gfx);
void ItemFrac_calcCoordinate(Item* _this, Graphics* gfx, int x, int y);
void ItemFrac_draw(Item* _this, Graphics* gfx);

int ItemSign_width(Item* _this, Graphics* gfx);
int ItemSign_height(Item* _this, Graphics* gfx);
int ItemSign_baseLine(Item* _this, Graphics* gfx);
void ItemSign_calcCoordinate(Item* _this, Graphics* gfx, int x, int y);
void ItemSign_draw(Item* _this, Graphics* gfx);

int ItemSqrt_width(Item* _this, Graphics* gfx);
int ItemSqrt_height(Item* _this, Graphics* gfx);
int ItemSqrt_baseLine(Item* _this, Graphics* gfx);
void ItemSqrt_calcCoordinate(Item* _this, Graphics* gfx, int x, int y);
void ItemSqrt_draw(Item* _this, Graphics* gfx);

void ItemPow_setFont(Item* _this, FontHandle hFont);
int ItemPow_width(Item* _this, Graphics* gfx);
int ItemPow_height(Item* _this, Graphics* gfx);
int ItemPow_baseLine(Item* _this, Graphics* gfx);
void ItemPow_calcCoordinate(Item* _this, Graphics* gfx, int x, int y);
void ItemPow_draw(Item* _this, Graphics* gfx);

void ItemSubscript_setFont(Item* _this, FontHandle hFont);
int ItemSubscript_width(Item* _this, Graphics* gfx);
int ItemSubscript_height(Item* _this, Graphics* gfx);
int ItemSubscript_baseLine(Item* _this, Graphics* gfx);
void ItemSubscript_calcCoordinate(Item* _this, Graphics* gfx, int x, int y);
void ItemSubscript_draw(Item* _this, Graphics* gfx);

int ItemCommFunc_width(Item* _this, Graphics* gfx);
int ItemCommFunc_height(Item* _this, Graphics* gfx);
int ItemCommFunc_baseLine(Item* _this, Graphics* gfx);
void ItemCommFunc_calcCoordinate(Item* _this, Graphics* gfx, int x, int y);
void ItemCommFunc_draw(Item* _this, Graphics* gfx);

int ItemFactorial_width(Item* _this, Graphics* gfx);
int ItemFactorial_height(Item* _this, Graphics* gfx);
int ItemFactorial_baseLine(Item* _this, Graphics* gfx);
void ItemFactorial_calcCoordinate(Item* _this, Graphics* gfx, int x, int y);
void ItemFactorial_draw(Item* _this, Graphics* gfx);

int ItemParentheses_width(Item* _this, Graphics* gfx);
int ItemParentheses_height(Item* _this, Graphics* gfx);
int ItemParentheses_baseLine(Item* _this, Graphics* gfx);
void ItemParentheses_calcCoordinate(Item* _this, Graphics* gfx, int x, int y);
void ItemParentheses_draw(Item* _this, Graphics* gfx);

void ItemSigma_setFont(Item* _this, FontHandle hFont);
int ItemSigma_width(Item* _this, Graphics* gfx);
int ItemSigma_height(Item* _this, Graphics* gfx);
int ItemSigma_baseLine(Item* _this, Graphics* gfx);
void ItemSigma_calcCoordinate(Item* _this, Graphics* gfx, int x, int y);
void ItemSigma_draw(Item* _this, Graphics* gfx);

void ItemIntegrate_setFont(Item* _this, FontHandle hFont);
int ItemIntegrate_width(Item* _this, Graphics* gfx);
int ItemIntegrate_height(Item* _this, Graphics* gfx);
int ItemIntegrate_baseLine(Item* _this, Graphics* gfx);
void ItemIntegrate_calcCoordinate(Item* _this, Graphics* gfx, int x, int y);
void ItemIntegrate_draw(Item* _this, Graphics* gfx);

int ItemDerivative_width(Item* _this, Graphics* gfx);
int ItemDerivative_height(Item* _this, Graphics* gfx);
int ItemDerivative_baseLine(Item* _this, Graphics* gfx);
void ItemDerivative_draw(Item* _this, Graphics* gfx);

void ItemLimit_setFont(Item* _this, FontHandle hFont);
int ItemLimit_width(Item* _this, Graphics* gfx);
int ItemLimit_height(Item* _this, Graphics* gfx);
int ItemLimit_baseLine(Item* _this, Graphics* gfx);
void ItemLimit_calcCoordinate(Item* _this, Graphics* gfx, int x, int y);
void ItemLimit_draw(Item* _this, Graphics* gfx);

// Item

void Item_destroy(Item* i)
{

}

// Boolean retrun value
int Item_isLeaf(Item* i)
{
	if ((i->_left == NULL) && (i->_right == NULL))
		return 1;
	else
		return 0;
}

void Item_toString(Item* __this, String* s)
{
	String_cpy(s, L"");
}

void copy_item(Item* a, Item* b)
{
	a->_left = (b->_left != NULL) ? b->_left->_cloneFunc(b->_left) : NULL;
	a->_right = (b->_right != NULL) ? b->_right->_cloneFunc(b->_right) : NULL;
	
	a->_isLeafFunc = b->_isLeafFunc;
	a->_destroyFunc = b->_destroyFunc;
	a->_toStringFunc = b->_toStringFunc;
	a->_cloneFunc = b->_cloneFunc;
	
	a->_calcCoordinateFunc = b->_calcCoordinateFunc;
	a->_baseLineFunc = b->_baseLineFunc;
	a->_widthFunc = b->_widthFunc;
	a->_heightFunc = b->_heightFunc;
	a->_drawFunc = b->_drawFunc;

	a->_setFocusFunc = b->_setFocusFunc;
	a->_getFocusFunc = b->_getFocusFunc;
	
	a->_objectType = b->_objectType;
	a->_procLevel = b->_procLevel;

	a->_hasFocus = 0;

	a->_x = b->_x;
	a->_y = b->_y;
	a->_baseLine = b->_baseLine;
	a->_width = b->_width;
	a->_height = b->_height;
}

Item* Item_clone(Item* _this)
{
	if (!_this)
		return NULL;

	Item* c = (Item*)malloc(sizeof(Item));

	copy_item(c, _this);
	
	return c;
}

Item* Item_init(Item* l, Item* r)
{
	Item* i = (Item*)malloc(sizeof(Item));
	assert(i != 0);
	
	i->_left = l;
	i->_right = r;

	i->_destroyFunc = Item_destroy;
	i->_isLeafFunc = Item_isLeaf;
	i->_toStringFunc = Item_toString;
	i->_cloneFunc = Item_clone;

	i->_setFontFunc = Item_setFont;
	i->_calcCoordinateFunc = Item_calcCoordinate;
	i->_widthFunc = Item_width;
	i->_heightFunc = Item_height;
	i->_baseLineFunc = Item_baseLine;
	i->_drawFunc = Item_draw;

	i->_setFocusFunc = Item_setFocus;
	i->_getFocusFunc = Item_getFocus;

	i->_objectType = OBJ_Base;
	i->_procLevel = PROC_L_0;

	i->_hasFocus = 0;

	return i;
}

// Literal

void ItemLiteral_destroy(Item* _this)
{
	ItemLiteral* i = (ItemLiteral*)_this;

	String_free(i->_str);
}

void ItemLiteral_toString(Item* _this, String* s)
{
	ItemLiteral* i = (ItemLiteral*)_this;

	String_cpy(s, i->_str->_str);
}

Item* ItemLiteral_clone(Item* _this)
{
	if (!_this)
		return NULL;

	ItemLiteral* i = (ItemLiteral*)_this;

	ItemLiteral* c = (ItemLiteral*)malloc(sizeof(ItemLiteral));

	copy_item((Item*)c, _this);

	c->_str = String_init();
	String_cpy(c->_str, i->_str->_str);

	return (Item*)c;
}

ItemLiteral* ItemLiteral_init(const wchar_t* s)
{
	ItemLiteral* i = (ItemLiteral*)malloc(sizeof(ItemLiteral));
	assert(i != 0);

	i->_item._left = NULL;
	i->_item._right = NULL;

	i->_item._destroyFunc = ItemLiteral_destroy;
	i->_item._isLeafFunc = Item_isLeaf;
	i->_item._toStringFunc = ItemLiteral_toString;
	i->_item._cloneFunc = ItemLiteral_clone;

	i->_item._setFontFunc = Item_setFont;
	i->_item._calcCoordinateFunc = Item_calcCoordinate;
	i->_item._widthFunc = ItemLiteral_width;
	i->_item._heightFunc = ItemLiteral_height;
	i->_item._baseLineFunc = ItemLiteral_baseLine;
	i->_item._drawFunc = ItemLiteral_draw;

	i->_item._setFocusFunc = Item_setFocus;
	i->_item._getFocusFunc = Item_getFocus;

	i->_item._objectType = OBJ_Literal;
	i->_item._procLevel = PROC_L_11;

	i->_item._hasFocus = 0;

	i->_str = String_init();
	
	String_cpy(i->_str, s);
	
	return i;
}

// Number

void ItemNumber_destroy(Item* _this)
{
	ItemNumber* i = (ItemNumber*)_this;

	String_free(i->_str);
}

void ItemNumber_toString(Item* _this, String* s)
{
	ItemNumber* i = (ItemNumber*)_this;

	String_cpy(s, i->_str->_str);
}

Item* ItemNumber_clone(Item* _this)
{
	if (!_this)
		return NULL;
	
	ItemNumber* i = (ItemNumber*)_this;

	ItemNumber* c = (ItemNumber*)malloc(sizeof(ItemNumber));

	copy_item((Item*)c, _this);

	c->_str = String_init();
	String_cpy(c->_str, i->_str->_str);

	return (Item*)c;
}

ItemNumber* ItemNumber_init(const wchar_t* s, int sign)
{
	ItemNumber* i = (ItemNumber*)malloc(sizeof(ItemNumber));
	assert(i != 0);

	i->_item._left = NULL;
	i->_item._right = NULL;

	i->_item._destroyFunc = ItemNumber_destroy;
	i->_item._isLeafFunc = Item_isLeaf;
	i->_item._toStringFunc = ItemNumber_toString;
	i->_item._cloneFunc = ItemNumber_clone;

	i->_item._setFontFunc = Item_setFont; 
	i->_item._calcCoordinateFunc = Item_calcCoordinate;
	i->_item._widthFunc = ItemNumber_width;
	i->_item._heightFunc = ItemNumber_height;
	i->_item._baseLineFunc = ItemNumber_baseLine;
	i->_item._drawFunc = ItemNumber_draw;

	i->_item._setFocusFunc = Item_setFocus;
	i->_item._getFocusFunc = Item_getFocus;

	i->_item._objectType = OBJ_Number;
	i->_item._procLevel = PROC_L_11;

	i->_item._hasFocus = 0;

	i->_str = String_init();
	String_cpy(i->_str, s);

	return i;
}

// Symbol

void ItemSymbol_toString(Item* _this, String* s)
{
	ItemSymbol* i = (ItemSymbol*)_this;

	wchar_t str[2];
	str[0] = i->_ch;
	str[1] = 0;

	String_cpy(s, str);
}

Item* ItemSymbol_clone(Item* _this)
{
	if (!_this)
		return NULL;

	ItemSymbol* i = (ItemSymbol*)_this;

	ItemSymbol* c = (ItemSymbol*)malloc(sizeof(ItemSymbol));

	copy_item((Item*)c, _this);

	c->_ch = i->_ch;

	return (Item*)c;
}

ItemSymbol* ItemSymbol_init(const wchar_t ch)
{
	ItemSymbol* i = (ItemSymbol*)malloc(sizeof(ItemSymbol));
	assert(i != 0);

	i->_item._left = NULL;
	i->_item._right = NULL;

	i->_item._destroyFunc = Item_destroy;
	i->_item._isLeafFunc = Item_isLeaf;
	i->_item._toStringFunc = ItemSymbol_toString;
	i->_item._cloneFunc = ItemSymbol_clone;

	i->_item._setFontFunc = Item_setFont;
	i->_item._calcCoordinateFunc = Item_calcCoordinate;
	i->_item._widthFunc = ItemSymbol_width;
	i->_item._heightFunc = ItemSymbol_height;
	i->_item._baseLineFunc = ItemSymbol_baseLine;
	i->_item._drawFunc = ItemSymbol_draw;

	i->_item._setFocusFunc = Item_setFocus;
	i->_item._getFocusFunc = Item_getFocus;

	i->_item._objectType = OBJ_Symbol;
	i->_item._procLevel = PROC_L_11;

	i->_item._hasFocus = 0;

	i->_ch = ch;

	return i;
}
// List

void ItemList_toString(Item* _this, String* s)
{
	String* s1 = String_init();
	String* s2 = String_init();

	_this->_left->_toStringFunc(_this->_left, s1);
	_this->_right->_toStringFunc(_this->_right, s2);

	String_cpy(s, s1->_str);
	String_cat(s, L",");
	String_cat(s, s2->_str);

	String_free(s1);
	String_free(s2);
}

Item* ItemList_clone(Item* _this)
{
	if (!_this)
		return NULL;

	ItemList* i = (ItemList*)_this;

	ItemList* c = (ItemList*)malloc(sizeof(ItemList));

	copy_item((Item*)c, _this);

	return (Item*)c;
}

ItemList* ItemList_init(Item* l, Item* r)
{
	ItemList* i = (ItemList*)malloc(sizeof(ItemList));
	assert(i != 0);

	i->_item._left = (l != NULL) ? l : Item_init(NULL, NULL);
	i->_item._right = (r != NULL) ? r : Item_init(NULL, NULL);

	i->_item._destroyFunc = Item_destroy;
	i->_item._isLeafFunc = Item_isLeaf;
	i->_item._toStringFunc = ItemList_toString;
	i->_item._cloneFunc = ItemList_clone;

	i->_item._setFontFunc = Item_setFont; 
	i->_item._calcCoordinateFunc = ItemList_calcCoordinate;
	i->_item._widthFunc = ItemList_width;
	i->_item._heightFunc = ItemList_height;
	i->_item._baseLineFunc = ItemList_baseLine;
	i->_item._drawFunc = ItemList_draw;

	i->_item._setFocusFunc = Item_setFocus;
	i->_item._getFocusFunc = Item_getFocus;

	i->_item._objectType = OBJ_List;
	i->_item._procLevel = PROC_L_1;

	i->_item._hasFocus = 0;

	return i;
}

// Equ

void ItemEqu_toString(Item* _this, String* s)
{
	String* s1 = String_init();
	String* s2 = String_init();

	_this->_left->_toStringFunc(_this->_left, s1);
	_this->_right->_toStringFunc(_this->_right, s2);

	String_cpy(s, s1->_str);
	String_cat(s, L"=");
	String_cat(s, s2->_str);

	String_free(s1);
	String_free(s2);
}

Item* ItemEqu_clone(Item* _this)
{
	if (!_this)
		return NULL;

	ItemEqu* i = (ItemEqu*)_this;

	ItemEqu* c = (ItemEqu*)malloc(sizeof(ItemEqu));

	copy_item((Item*)c, _this);

	return (Item*)c;
}

ItemEqu* ItemEqu_init(Item* l, Item* r, wchar_t sy)
{
	ItemEqu* i = (ItemEqu*)malloc(sizeof(ItemEqu));
	assert(i != 0);

	i->_item._left = (l != NULL) ? l : Item_init(NULL, NULL);
	i->_item._right = (r != NULL) ? r : Item_init(NULL, NULL);

	i->_item._destroyFunc = Item_destroy;
	i->_item._isLeafFunc = Item_isLeaf;
	i->_item._toStringFunc = ItemEqu_toString;
	i->_item._cloneFunc = ItemEqu_clone;

	i->_item._setFontFunc = Item_setFont; 
	i->_item._calcCoordinateFunc = ItemEqu_calcCoordinate;
	i->_item._widthFunc = ItemEqu_width;
	i->_item._heightFunc = ItemEqu_height;
	i->_item._baseLineFunc = ItemEqu_baseLine;
	i->_item._drawFunc = ItemEqu_draw;

	i->_item._setFocusFunc = Item_setFocus;
	i->_item._getFocusFunc = Item_getFocus;
	
	i->_item._objectType = OBJ_Equ;
	i->_item._procLevel = PROC_L_2;

	i->_sy = sy;

	i->_item._hasFocus = 0;

	return i;
}

ItemEqu* ItemEqu_init_eq(Item* l, Item* r)
{
	return ItemEqu_init(l, r, L'=');
}

ItemEqu* ItemEqu_init_nq(Item* l, Item* r)
{
	return ItemEqu_init(l, r, L'\u2260');
}

ItemEqu* ItemEqu_init_g(Item* l, Item* r)
{
	return ItemEqu_init(l, r, L'>');
}

ItemEqu* ItemEqu_init_l(Item* l, Item* r)
{
	return ItemEqu_init(l, r, L'<');
}

ItemEqu* ItemEqu_init_eg(Item* l, Item* r)
{
	return ItemEqu_init(l, r, L'\u2265');
}

ItemEqu* ItemEqu_init_el(Item* l, Item* r)
{
	return ItemEqu_init(l, r, L'\u2264');
}

// Add

void ItemAdd_toString(Item* _this, String* s)
{
	String* s1 = String_init();
	String* s2 = String_init();

	_this->_left->_toStringFunc(_this->_left, s1);
	_this->_right->_toStringFunc(_this->_right, s2);
	
	String_cat(s, s1->_str);
	String_cat(s, L"+");
	String_cat(s, s2->_str);

	String_free(s1);
	String_free(s2);
}

Item* ItemAdd_clone(Item* _this)
{
	if (!_this)
		return NULL;

	ItemAdd* i = (ItemAdd*)_this;

	ItemAdd* c = (ItemAdd*)malloc(sizeof(ItemAdd));

	copy_item((Item*)c, _this);

	return (Item*)c;
}

ItemAdd* ItemAdd_init(Item* l, Item* r)
{
	ItemAdd* i = (ItemAdd*)malloc(sizeof(ItemAdd));
	assert(i != 0);

	i->_item._left = (l != NULL) ? l : Item_init(NULL, NULL);
	i->_item._right = (r != NULL) ? r : Item_init(NULL, NULL);

	i->_item._destroyFunc = Item_destroy; 
	i->_item._isLeafFunc = Item_isLeaf;
	i->_item._toStringFunc = ItemAdd_toString;
	i->_item._cloneFunc = ItemAdd_clone;

	i->_item._setFontFunc = Item_setFont; 
	i->_item._calcCoordinateFunc = ItemAdd_calcCoordinate;
	i->_item._widthFunc = ItemAdd_width;
	i->_item._heightFunc = ItemAdd_height;
	i->_item._baseLineFunc = ItemAdd_baseLine;
	i->_item._drawFunc = ItemAdd_draw;

	i->_item._setFocusFunc = Item_setFocus;
	i->_item._getFocusFunc = Item_getFocus;
	
	i->_item._objectType = OBJ_Add;
	i->_item._procLevel = PROC_L_3;

	i->_item._hasFocus = 0;

	return i;
}

// Sub

void ItemSub_toString(Item* _this, String* s)
{
	String* s1 = String_init();
	String* s2 = String_init();

	_this->_left->_toStringFunc(_this->_left, s1);
	_this->_right->_toStringFunc(_this->_right, s2);

	String_cat(s, s1->_str);
	String_cat(s, L"-");
	String_cat(s, s2->_str);

	String_free(s1);
	String_free(s2);
}

Item* ItemSub_clone(Item* _this)
{
	if (!_this)
		return NULL;

	ItemSub* i = (ItemSub*)_this;

	ItemSub* c = (ItemSub*)malloc(sizeof(ItemSub));

	copy_item((Item*)c, _this);

	return (Item*)c;
}

ItemSub* ItemSub_init(Item* l, Item* r)
{
	ItemSub* i = (ItemSub*)malloc(sizeof(ItemSub));
	assert(i != 0);

	i->_item._left = (l != NULL) ? l : Item_init(NULL, NULL);
	i->_item._right = (r != NULL) ? r : Item_init(NULL, NULL);

	i->_item._destroyFunc = Item_destroy; 
	i->_item._isLeafFunc = Item_isLeaf;
	i->_item._toStringFunc = ItemSub_toString;
	i->_item._cloneFunc = ItemSub_clone;

	i->_item._setFontFunc = Item_setFont; 
	i->_item._calcCoordinateFunc = ItemSub_calcCoordinate;
	i->_item._widthFunc = ItemSub_width;
	i->_item._heightFunc = ItemSub_height;
	i->_item._baseLineFunc = ItemSub_baseLine;
	i->_item._drawFunc = ItemSub_draw;

	i->_item._setFocusFunc = Item_setFocus;
	i->_item._getFocusFunc = Item_getFocus;
	
	i->_item._objectType = OBJ_Sub;
	i->_item._procLevel = PROC_L_3;

	i->_item._hasFocus = 0;

	return i;
}

// Mult

void ItemMult_toString(Item* _this, String* s)
{
	String* s1 = String_init();
	String* s2 = String_init();

	_this->_left->_toStringFunc(_this->_left, s1);
	_this->_right->_toStringFunc(_this->_right, s2);

	String_cat(s, s1->_str);
	String_cat(s, L"*");
	String_cat(s, s2->_str);

	String_free(s1);
	String_free(s2);
}

Item* ItemMult_clone(Item* _this)
{
	if (!_this)
		return NULL;

	ItemMult* i = (ItemMult*)_this;

	ItemMult* c = (ItemMult*)malloc(sizeof(ItemMult));

	copy_item((Item*)c, _this);

	return (Item*)c;
}

ItemMult* ItemMult_init(Item* l, Item* r)
{
	ItemMult* i = (ItemMult*)malloc(sizeof(ItemMult));
	assert(i != 0);

	i->_item._left = (l != NULL) ? l : Item_init(NULL, NULL);
	i->_item._right = (r != NULL) ? r : Item_init(NULL, NULL);

	i->_item._destroyFunc = Item_destroy; 
	i->_item._isLeafFunc = Item_isLeaf;
	i->_item._toStringFunc = ItemMult_toString;
	i->_item._cloneFunc = ItemMult_clone;

	i->_item._setFontFunc = Item_setFont; 
	i->_item._calcCoordinateFunc = ItemMult_calcCoordinate;
	i->_item._widthFunc = ItemMult_width;
	i->_item._heightFunc = ItemMult_height;
	i->_item._baseLineFunc = ItemMult_baseLine;
	i->_item._drawFunc = ItemMult_draw;

	i->_item._setFocusFunc = Item_setFocus;
	i->_item._getFocusFunc = Item_getFocus;
	
	i->_item._objectType = OBJ_Mult;
	i->_item._procLevel = PROC_L_4;

	i->_item._hasFocus = 0;

	return i;
}

// Frac

void ItemFrac_toString(Item* _this, String* s)
{
	String* s1 = String_init();
	String* s2 = String_init();

	_this->_left->_toStringFunc(_this->_left, s1);
	_this->_right->_toStringFunc(_this->_right, s2);

	if(!_this->_left->_isLeafFunc(_this->_left))
		String_cat(s, L"(");
	String_cat(s, s1->_str);
	if (!_this->_left->_isLeafFunc(_this->_left))
		String_cat(s, L")");

	String_cat(s, L"/");

	if (!_this->_right->_isLeafFunc(_this->_right))
		String_cat(s, L"(");
	String_cat(s, s2->_str);
	if (!_this->_right->_isLeafFunc(_this->_right))
		String_cat(s, L")");

	String_free(s1);
	String_free(s2);
}

Item* ItemFrac_clone(Item* _this)
{
	if (!_this)
		return NULL;

	ItemFrac* i = (ItemFrac*)_this;

	ItemFrac* c = (ItemFrac*)malloc(sizeof(ItemFrac));

	copy_item((Item*)c, _this);

	c->_fHeight = i->_fHeight;

	return (Item*)c;
}

ItemFrac* ItemFrac_init(Item* l, Item* r)
{
	ItemFrac* i = (ItemFrac*)malloc(sizeof(ItemFrac));
	assert(i != 0);

	i->_item._left = (l != NULL) ? l : Item_init(NULL, NULL);
	i->_item._right = (r != NULL) ? r : Item_init(NULL, NULL);

	i->_item._destroyFunc = Item_destroy; 
	i->_item._isLeafFunc = Item_isLeaf;
	i->_item._toStringFunc = ItemFrac_toString;
	i->_item._cloneFunc = ItemFrac_clone;

	i->_item._setFontFunc = Item_setFont; 
	i->_item._calcCoordinateFunc = ItemFrac_calcCoordinate;
	i->_item._widthFunc = ItemFrac_width;
	i->_item._heightFunc = ItemFrac_height;
	i->_item._baseLineFunc = ItemFrac_baseLine;
	i->_item._drawFunc = ItemFrac_draw;

	i->_item._setFocusFunc = Item_setFocus;
	i->_item._getFocusFunc = Item_getFocus;
	
	i->_item._objectType = OBJ_Frac;
	i->_item._procLevel = PROC_L_4;

	i->_item._hasFocus = 0;

	i->_fHeight = 8;

	return i;
}

// Sign

void ItemSign_toString(Item* _this, String* s)
{
	String* s1 = String_init();

	ItemSign* i = (ItemSign*)_this; 

	wchar_t str[2];
	str[0] = i->_sgn;
	str[1] = 0;

	String_cat(s, str);
	
	_this->_left->_toStringFunc(_this->_left, s1);

	String_cat(s, s1->_str);

	String_free(s1);
}

Item* ItemSign_clone(Item* _this)
{
	if (!_this)
		return NULL;

	ItemSign* i = (ItemSign*)_this;

	ItemSign* c = (ItemSign*)malloc(sizeof(ItemSign));

	copy_item((Item*)c, _this);

	c->_sgn = i->_sgn;

	return (Item*)c;
}

ItemSign* ItemSign_init(Item* l, const wchar_t sgn)
{
	ItemSign* i = (ItemSign*)malloc(sizeof(ItemSign));
	assert(i != 0);

	i->_item._left = (l != NULL) ? l : Item_init(NULL, NULL);
	i->_item._right = NULL;

	i->_item._destroyFunc = Item_destroy; 
	i->_item._isLeafFunc = Item_isLeaf;
	i->_item._toStringFunc = ItemSign_toString;
	i->_item._cloneFunc = ItemSign_clone;

	i->_item._setFontFunc = Item_setFont; 
	i->_item._calcCoordinateFunc = ItemSign_calcCoordinate;
	i->_item._widthFunc = ItemSign_width;
	i->_item._heightFunc = ItemSign_height;
	i->_item._baseLineFunc = ItemSign_baseLine;
	i->_item._drawFunc = ItemSign_draw;

	i->_item._setFocusFunc = Item_setFocus;
	i->_item._getFocusFunc = Item_getFocus;
	
	i->_item._objectType = OBJ_Sign;
	i->_item._procLevel = PROC_L_5;

	i->_item._hasFocus = 0;

	i->_sgn = sgn;

	return i;
}

ItemSign* ItemSignMinus_init(Item* l)
{
	return ItemSign_init(l, L'-');
}

ItemSign* ItemSignPlus_init(Item* l)
{
	return ItemSign_init(l, L'+');
}

// Sqrt

void ItemSqrt_toString(Item* _this, String* s)
{
	String* s1 = String_init();

	_this->_left->_toStringFunc(_this->_left, s1);

	String_cpy(s, L"Sqrt(");
	String_cat(s, s1->_str);
	String_cat(s, L")");

	String_free(s1);
}

Item* ItemSqrt_clone(Item* _this)
{
	if (!_this)
		return NULL;

	ItemSqrt* i = (ItemSqrt*)_this;

	ItemSqrt* c = (ItemSqrt*)malloc(sizeof(ItemSqrt));

	copy_item((Item*)c, _this);

	c->_sHeight = i->_sHeight;
	c->_sPadding = i->_sPadding;

	return (Item*)c;
}

ItemSqrt* ItemSqrt_init(Item* l)
{
	ItemSqrt* i = (ItemSqrt*)malloc(sizeof(ItemSqrt));
	assert(i != 0);

	i->_item._left = (l != NULL) ? l : Item_init(NULL, NULL);
	i->_item._right = NULL;

	i->_item._destroyFunc = Item_destroy; 
	i->_item._isLeafFunc = Item_isLeaf;
	i->_item._toStringFunc = ItemSqrt_toString;
	i->_item._cloneFunc = ItemSqrt_clone;

	i->_item._setFontFunc = Item_setFont; 
	i->_item._calcCoordinateFunc = ItemSqrt_calcCoordinate;
	i->_item._widthFunc = ItemSqrt_width;
	i->_item._heightFunc = ItemSqrt_height;
	i->_item._baseLineFunc = ItemSqrt_baseLine;
	i->_item._drawFunc = ItemSqrt_draw;

	i->_item._setFocusFunc = Item_setFocus;
	i->_item._getFocusFunc = Item_getFocus;
	
	i->_item._objectType = OBJ_Sqrt;
	i->_item._procLevel = PROC_L_9;

	i->_item._hasFocus = 0;

	i->_sHeight = 0;
	i->_sPadding = 20;

	return i;
}

// Pow

void ItemPow_toString(Item* _this, String* s)
{
	String* s1 = String_init();
	String* s2 = String_init();

	_this->_left->_toStringFunc(_this->_left, s1);
	_this->_right->_toStringFunc(_this->_right, s2);

	String_cpy(s, s1->_str);
	String_cat(s, L"^");

	if (!_this->_right->_isLeafFunc(_this->_right))
		String_cat(s, L"(");
	String_cat(s, s2->_str);
	if (!_this->_right->_isLeafFunc(_this->_right))
		String_cat(s, L")");

	String_free(s1);
	String_free(s2);
}

Item* ItemPow_clone(Item* _this)
{
	if (!_this)
		return NULL;

	ItemPow* i = (ItemPow*)_this;

	ItemPow* c = (ItemPow*)malloc(sizeof(ItemPow));

	copy_item((Item*)c, _this);

	return (Item*)c;
}

ItemPow* ItemPow_init(Item* l, Item* r)
{
	ItemPow* i = (ItemPow*)malloc(sizeof(ItemPow));
	assert(i != 0);

	i->_item._left = (l != NULL) ? l : Item_init(NULL, NULL);
	i->_item._right = (r != NULL) ? r : Item_init(NULL, NULL);

	i->_item._destroyFunc = Item_destroy; 
	i->_item._isLeafFunc = Item_isLeaf;
	i->_item._toStringFunc = ItemPow_toString;
	i->_item._cloneFunc = ItemPow_clone;

	i->_item._setFontFunc = ItemPow_setFont; 
	i->_item._calcCoordinateFunc = ItemPow_calcCoordinate;
	i->_item._widthFunc = ItemPow_width;
	i->_item._heightFunc = ItemPow_height;
	i->_item._baseLineFunc = ItemPow_baseLine;
	i->_item._drawFunc = ItemPow_draw;

	i->_item._setFocusFunc = Item_setFocus;
	i->_item._getFocusFunc = Item_getFocus;
	
	i->_item._objectType = OBJ_Pow;
	i->_item._procLevel = PROC_R_7;

	i->_item._hasFocus = 0;

	return i;
}

// Subscript

void ItemSubscript_toString(Item* _this, String* s)
{
	String* s1 = String_init();
	String* s2 = String_init();

	_this->_left->_toStringFunc(_this->_left, s1);
	_this->_right->_toStringFunc(_this->_right, s2);

	String_cpy(s, s1->_str);
	String_cat(s, L"_");

	if (!_this->_right->_isLeafFunc(_this->_right))
		String_cat(s, L"(");
	String_cat(s, s2->_str);
	if (!_this->_right->_isLeafFunc(_this->_right))
		String_cat(s, L")");

	String_free(s1);
	String_free(s2);
}

Item* ItemSubscript_clone(Item* _this)
{
	if (!_this)
		return NULL;

	ItemSubscript* i = (ItemSubscript*)_this;

	ItemSubscript* c = (ItemSubscript*)malloc(sizeof(ItemSubscript));

	copy_item((Item*)c, _this);

	return (Item*)c;
}

ItemSubscript* ItemSubscript_init(Item* l, Item* r)
{
	ItemSubscript* i = (ItemSubscript*)malloc(sizeof(ItemSubscript));
	assert(i != 0);

	i->_item._left = (l != NULL) ? l : Item_init(NULL, NULL);
	i->_item._right = (r != NULL) ? r : Item_init(NULL, NULL);

	i->_item._destroyFunc = Item_destroy;
	i->_item._isLeafFunc = Item_isLeaf;
	i->_item._toStringFunc = ItemSubscript_toString;
	i->_item._cloneFunc = ItemSubscript_clone;

	i->_item._setFontFunc = ItemSubscript_setFont; 
	i->_item._calcCoordinateFunc = ItemSubscript_calcCoordinate;
	i->_item._widthFunc = ItemSubscript_width;
	i->_item._heightFunc = ItemSubscript_height;
	i->_item._baseLineFunc = ItemSubscript_baseLine;
	i->_item._drawFunc = ItemSubscript_draw;

	i->_item._setFocusFunc = Item_setFocus;
	i->_item._getFocusFunc = Item_getFocus;
	
	i->_item._objectType = OBJ_Subscript;
	i->_item._procLevel = PROC_R_8;

	i->_item._hasFocus = 0;

	return i;
}

// CommFunc

void ItemCommFunc_toString(Item* _this, String* s)
{
	String* s1 = String_init();
	ItemCommFunc* i = (ItemCommFunc*)_this;

	_this->_left->_toStringFunc(_this->_left, s1);

	String_cpy(s, i->_str);
	String_cat(s, L"(");
	String_cat(s, s1->_str);
	String_cat(s, L")");

	String_free(s1);
}

Item* ItemCommFunc_clone(Item* _this)
{
	if (!_this)
		return NULL;

	ItemCommFunc* i = (ItemCommFunc*)_this;

	ItemCommFunc* c = (ItemCommFunc*)malloc(sizeof(ItemCommFunc));

	copy_item((Item*)c, _this);

	c->_len = i->_len;
	c->_str = (wchar_t*)malloc(sizeof(wchar_t) * (c->_len + 1));
	assert(c->_str != NULL);
	wcscpy_s(c->_str, c->_len + 1, i->_str);

	return (Item*)c;
}

void ItemCommFunc_destroy(Item* _this)
{
	ItemCommFunc* i = (ItemCommFunc*)_this;
	free(i->_str);
}

ItemCommFunc* ItemCommFunc_init(Item* l, Item* r, const wchar_t* s)
{
	ItemCommFunc* i = (ItemCommFunc*)malloc(sizeof(ItemCommFunc));
	assert(i != 0);

	i->_item._left = (l != NULL) ? l : Item_init(NULL, NULL);
	i->_item._right = r;

	i->_item._destroyFunc = ItemCommFunc_destroy;
	i->_item._isLeafFunc = Item_isLeaf;
	i->_item._toStringFunc = ItemCommFunc_toString;
	i->_item._cloneFunc = ItemCommFunc_clone;

	i->_item._setFontFunc = Item_setFont; 
	i->_item._calcCoordinateFunc = ItemCommFunc_calcCoordinate;
	i->_item._widthFunc = ItemCommFunc_width;
	i->_item._heightFunc = ItemCommFunc_height;
	i->_item._baseLineFunc = ItemCommFunc_baseLine;
	i->_item._drawFunc = ItemCommFunc_draw;

	i->_item._setFocusFunc = Item_setFocus;
	i->_item._getFocusFunc = Item_getFocus;
	
	i->_item._objectType = OBJ_CommFunc;
	i->_item._procLevel = PROC_L_9;

	i->_item._hasFocus = 0;

	i->_len = wcslen(s);
	i->_str = (wchar_t*)malloc(sizeof(wchar_t) * (i->_len + 1));
	assert(i->_str != 0);

	memset(i->_str, 0, i->_len + 1);
	wcscpy_s(i->_str, i->_len + 1, s);

	return i;
}

ItemCommFunc* ItemSinFunc_init(Item* l)
{
	return ItemCommFunc_init(l, NULL, L"Sin");
}

ItemCommFunc* ItemCosFunc_init(Item* l)
{
	return ItemCommFunc_init(l, NULL, L"Cos");
}

ItemCommFunc* ItemTanFunc_init(Item* l)
{
	return ItemCommFunc_init(l, NULL, L"Tan");
}

ItemCommFunc* ItemLogFunc_init(Item* l)
{
	return ItemCommFunc_init(l, NULL, L"Log");
}

ItemCommFunc* ItemExpFunc_init(Item* l)
{
	return ItemCommFunc_init(l, NULL, L"Exp");
}

ItemCommFunc* ItemAsinFunc_init(Item* l)
{
	return ItemCommFunc_init(l, NULL, L"Asin");
}

ItemCommFunc* ItemAcosFunc_init(Item* l)
{
	return ItemCommFunc_init(l, NULL, L"Acos");
}

ItemCommFunc* ItemAtanFunc_init(Item* l)
{
	return ItemCommFunc_init(l, NULL, L"Atan");
}

ItemCommFunc* ItemLnFunc_init(Item* l)
{
	return ItemCommFunc_init(l, NULL, L"Ln");
}

// Factorial

void ItemFactorial_toString(Item* _this, String* s)
{
	String* s1 = String_init();

	_this->_left->_toStringFunc(_this->_left, s1);

	String_cpy(s, s1->_str);
	String_cat(s, L"!");

	String_free(s1);
}

Item* ItemFactorial_clone(Item* _this)
{
	if (!_this)
		return NULL;

	ItemFactorial* i = (ItemFactorial*)_this;

	ItemFactorial* c = (ItemFactorial*)malloc(sizeof(ItemFactorial));

	copy_item((Item*)c, _this);

	return (Item*)c;
}

ItemFactorial* ItemFactorial_init(Item* l)
{
	ItemFactorial* i = (ItemFactorial*)malloc(sizeof(ItemFactorial));
	assert(i != 0);

	i->_item._left = (l != NULL) ? l : Item_init(NULL, NULL);
	i->_item._right = NULL;

	i->_item._destroyFunc = Item_destroy; 
	i->_item._isLeafFunc = Item_isLeaf;
	i->_item._toStringFunc = ItemFactorial_toString;
	i->_item._cloneFunc = ItemFactorial_clone;

	i->_item._setFontFunc = Item_setFont; 
	i->_item._calcCoordinateFunc = ItemFactorial_calcCoordinate;
	i->_item._widthFunc = ItemFactorial_width;
	i->_item._heightFunc = ItemFactorial_height;
	i->_item._baseLineFunc = ItemFactorial_baseLine;
	i->_item._drawFunc = ItemFactorial_draw;

	i->_item._setFocusFunc = Item_setFocus;
	i->_item._getFocusFunc = Item_getFocus;
	
	i->_item._objectType = OBJ_Factorial;
	i->_item._procLevel = PROC_L_6;

	i->_item._hasFocus = 0;

	return i;
}

// Parentheses

void ItemParentheses_toString(Item* _this, String* s)
{
	String* s1 = String_init();

	_this->_left->_toStringFunc(_this->_left, s1);

	String_cpy(s, L"(");
	String_cat(s, s1->_str);
	String_cat(s, L")");

	String_free(s1);
}

Item* ItemParentheses_clone(Item* _this)
{
	if (!_this)
		return NULL;

	ItemParentheses* i = (ItemParentheses*)_this;

	ItemParentheses* c = (ItemParentheses*)malloc(sizeof(ItemFactorial));

	copy_item((Item*)c, _this);

	return (Item*)c;
}

ItemParentheses* ItemParentheses_init(Item* l)
{
	ItemParentheses* i = (ItemParentheses*)malloc(sizeof(ItemParentheses));
	assert(i != 0);

	i->_item._left = (l != NULL) ? l : Item_init(NULL, NULL);
	i->_item._right = NULL;

	i->_item._destroyFunc = Item_destroy;
	i->_item._isLeafFunc = Item_isLeaf;
	i->_item._toStringFunc = ItemParentheses_toString;
	i->_item._cloneFunc = ItemParentheses_clone;

	i->_item._setFontFunc = Item_setFont; 
	i->_item._calcCoordinateFunc = ItemParentheses_calcCoordinate;
	i->_item._widthFunc = ItemParentheses_width;
	i->_item._heightFunc = ItemParentheses_height;
	i->_item._baseLineFunc = ItemParentheses_baseLine;
	i->_item._drawFunc = ItemParentheses_draw;

	i->_item._setFocusFunc = Item_setFocus;
	i->_item._getFocusFunc = Item_getFocus;

	i->_item._objectType = OBJ_Parentheses;
	i->_item._procLevel = PROC_L_10;

	i->_item._hasFocus = 0;

	return i;
}

// Sigma

void ItemSigma_destroy(Item* _this)
{
	ItemSigma* i = (ItemSigma*)_this;

	if (i->_bottom)
	{
		ItemTree_free(&(i->_bottom));
	}

	if (i->_top)
	{
		ItemTree_free(&(i->_top));
	}
}

void ItemSigma_toString(Item* _this, String* s)
{
	ItemSigma* i = (ItemSigma*)_this;

	String* s1 = String_init();
	String* s2 = String_init();
	String* s3 = String_init();

	_this->_left->_toStringFunc(_this->_left, s1);

	String_cpy(s, L"Sigma(");
	String_cat(s, s1->_str);

	if (i->_bottom)
	{
		i->_bottom->_toStringFunc(i->_bottom, s2);
		String_cat(s, L";");
		String_cat(s, s2->_str);
	}

	if (i->_top)
	{
		i->_top->_toStringFunc(i->_top, s3);
		String_cat(s, L";");
		String_cat(s, s3->_str);
	}

	String_cat(s, L")");

	String_free(s1);
	String_free(s2);
	String_free(s3);
}

Item* ItemSigma_clone(Item* _this)
{
	if (!_this)
		return NULL;

	ItemSigma* i = (ItemSigma*)_this;

	ItemSigma* c = (ItemSigma*)malloc(sizeof(ItemSigma));

	copy_item((Item*)c, _this);

	c->_bottom = (i->_bottom != NULL) ? i->_bottom->_cloneFunc(i->_bottom) : NULL;
	c->_top = (i->_top != NULL) ? i->_top->_cloneFunc(i->_top) : NULL;

	return (Item*)c;
}

ItemSigma* ItemSigma_init(Item* l, Item* b, Item* t)
{
	ItemSigma* i = (ItemSigma*)malloc(sizeof(ItemSigma));
	assert(i != 0);

	i->_item._left = (l != NULL) ? l : Item_init(NULL, NULL);
	i->_item._right = NULL;

	i->_bottom = b;
	i->_top = t;

	i->_item._destroyFunc = ItemSigma_destroy;
	i->_item._isLeafFunc = Item_isLeaf;
	i->_item._toStringFunc = ItemSigma_toString;
	i->_item._cloneFunc = ItemSigma_clone;

	i->_item._setFontFunc = ItemSigma_setFont; 
	i->_item._calcCoordinateFunc = ItemSigma_calcCoordinate;
	i->_item._widthFunc = ItemSigma_width;
	i->_item._heightFunc = ItemSigma_height;
	i->_item._baseLineFunc = ItemSigma_baseLine;
	i->_item._drawFunc = ItemSigma_draw;

	i->_item._setFocusFunc = Item_setFocus;
	i->_item._getFocusFunc = Item_getFocus;
	
	i->_item._objectType = OBJ_Sigma;
	i->_item._procLevel = PROC_L_9;

	i->_item._hasFocus = 0;

	return i;
}

// Integrate

void ItemIntegrate_destroy(Item* _this)
{
	ItemIntegrate* i = (ItemIntegrate*)_this;

	if (i->_bottom)
	{
		ItemTree_free(&(i->_bottom));
	}

	if (i->_top)
	{
		ItemTree_free(&(i->_top));
	}
}

void ItemIntegrate_toString(Item* _this, String* s)
{
	ItemSigma* i = (ItemSigma*)_this;

	String* s1 = String_init();
	String* s2 = String_init();
	String* s3 = String_init();
	String* s4 = String_init();

	_this->_left->_toStringFunc(_this->_left, s1);

	String_cpy(s, L"Integrate(");
	String_cat(s, s1->_str);

	if (i->_item._right)
	{
		i->_item._right->_toStringFunc(i->_item._right, s4);
		String_cat(s, L";");
		String_cat(s, s4->_str);
	}

	if (i->_bottom)
	{
		i->_bottom->_toStringFunc(i->_bottom, s2);
		String_cat(s, L";");
		String_cat(s, s2->_str);
	}

	if (i->_top)
	{
		i->_top->_toStringFunc(i->_top, s3);
		String_cat(s, L";");
		String_cat(s, s3->_str);
	}

	String_cat(s, L")");

	String_free(s1);
	String_free(s2);
	String_free(s3);
	String_free(s4);
}

Item* ItemIntegrate_clone(Item* _this)
{
	if (!_this)
		return NULL;

	ItemIntegrate* i = (ItemIntegrate*)_this;

	ItemIntegrate* c = (ItemIntegrate*)malloc(sizeof(ItemIntegrate));

	copy_item((Item*)c, _this);

	c->_bottom = (i->_bottom != NULL) ? i->_bottom->_cloneFunc(i->_bottom) : NULL;
	c->_top = (i->_top != NULL) ? i->_top->_cloneFunc(i->_top) : NULL;

	return (Item*)c;
}

ItemIntegrate* ItemIntegrate_init(Item* l, Item* r, Item* b, Item* t)
{
	ItemIntegrate* i = (ItemIntegrate*)malloc(sizeof(ItemIntegrate));
	assert(i != 0);

	i->_item._left = (l != NULL) ? l : Item_init(NULL, NULL);
	i->_item._right = (r != NULL) ? r : Item_init(NULL, NULL);

	i->_bottom = b;
	i->_top = t;

	i->_item._destroyFunc = ItemIntegrate_destroy;
	i->_item._isLeafFunc = Item_isLeaf;
	i->_item._toStringFunc = ItemIntegrate_toString;
	i->_item._cloneFunc = ItemIntegrate_clone;

	i->_item._setFontFunc = ItemIntegrate_setFont; 
	i->_item._calcCoordinateFunc = ItemIntegrate_calcCoordinate;
	i->_item._widthFunc = ItemIntegrate_width;
	i->_item._heightFunc = ItemIntegrate_height;
	i->_item._baseLineFunc = ItemIntegrate_baseLine;
	i->_item._drawFunc = ItemIntegrate_draw;

	i->_item._setFocusFunc = Item_setFocus;
	i->_item._getFocusFunc = Item_getFocus;
	
	i->_item._objectType = OBJ_Integrate;
	i->_item._procLevel = PROC_L_9;

	i->_item._hasFocus = 0;

	return i;
}

// Derivative

void ItemDerivative_toString(Item* _this, String* s)
{
	String* s1 = String_init();

	_this->_left->_toStringFunc(_this->_left, s1);

	String_cpy(s, L"Derivative(");
	String_cat(s, s1->_str);
	String_cat(s, L")");

	String_free(s1);
}

Item* ItemDerivative_clone(Item* _this)
{
	if (!_this)
		return NULL;

	ItemDerivative* i = (ItemDerivative*)_this;

	ItemDerivative* c = (ItemDerivative*)malloc(sizeof(ItemDerivative));

	copy_item((Item*)c, _this);

	return (Item*)c;
}

ItemDerivative* ItemDerivative_init(Item* l, Item* r)
{
	ItemDerivative* i = (ItemDerivative*)malloc(sizeof(ItemDerivative));
	assert(i != 0);

	i->_item._left = (l != NULL) ? l : Item_init(NULL, NULL);
	i->_item._right = (r != NULL) ? r : Item_init(NULL, NULL);

	i->_item._destroyFunc = Item_destroy;
	i->_item._isLeafFunc = Item_isLeaf;
	i->_item._toStringFunc = ItemDerivative_toString;
	i->_item._cloneFunc = ItemDerivative_clone;

	i->_item._setFontFunc = Item_setFont; 
	i->_item._calcCoordinateFunc = Item_calcCoordinate;
	i->_item._widthFunc = ItemDerivative_width;
	i->_item._heightFunc = ItemDerivative_height;
	i->_item._baseLineFunc = ItemDerivative_baseLine;
	i->_item._drawFunc = ItemDerivative_draw;

	i->_item._setFocusFunc = Item_setFocus;
	i->_item._getFocusFunc = Item_getFocus;
	
	i->_item._objectType = OBJ_Derivative;
	i->_item._procLevel = PROC_L_9;

	i->_item._hasFocus = 0;

	return i;
}

// Limit

void ItemLimit_destroy(Item* _this)
{
	ItemLimit* i = (ItemLimit*)_this;

	if (i->_t1)
	{
		ItemTree_free(&(i->_t1));
	}

	if (i->_t2)
	{
		ItemTree_free(&(i->_t2));
	}
}

void ItemLimit_toString(Item* _this, String* s)
{
	ItemLimit* i = (ItemLimit*)_this;

	String* s1 = String_init();
	String* s2 = String_init();
	String* s3 = String_init();

	_this->_left->_toStringFunc(_this->_left, s1);

	String_cpy(s, L"Limit(");
	String_cat(s, s1->_str);

	if (i->_t1)
	{
		i->_t1->_toStringFunc(i->_t1, s2);
		String_cat(s, L";");
		String_cat(s, s2->_str);
	}

	if (i->_t2)
	{
		i->_t2->_toStringFunc(i->_t2, s3);
		String_cat(s, L";");
		String_cat(s, s3->_str);
	}

	String_cat(s, L")");

	String_free(s1);
	String_free(s2);
	String_free(s3);
}

Item* ItemLimit_clone(Item* _this)
{
	if (!_this)
		return NULL;

	ItemLimit* i = (ItemLimit*)_this;

	ItemLimit* c = (ItemLimit*)malloc(sizeof(ItemSigma));

	copy_item((Item*)c, _this);

	c->_t1 = (i->_t1 != NULL) ? i->_t1->_cloneFunc(i->_t1) : NULL;
	c->_t2 = (i->_t2 != NULL) ? i->_t2->_cloneFunc(i->_t2) : NULL;

	return (Item*)c;
}

ItemLimit* ItemLimit_init(Item* l, Item* t1, Item* t2)
{
	ItemLimit* i = (ItemLimit*)malloc(sizeof(ItemLimit));
	assert(i != 0);

	i->_item._left = (l != NULL) ? l : Item_init(NULL, NULL);
	i->_item._right = NULL;

	i->_t1 = t1;
	i->_t2 = t2;

	i->_item._destroyFunc = ItemLimit_destroy;
	i->_item._isLeafFunc = Item_isLeaf;
	i->_item._toStringFunc = ItemLimit_toString;
	i->_item._cloneFunc = ItemLimit_clone;

	i->_item._setFontFunc = ItemLimit_setFont;
	i->_item._calcCoordinateFunc = ItemLimit_calcCoordinate;
	i->_item._widthFunc = ItemLimit_width;
	i->_item._heightFunc = ItemLimit_height;
	i->_item._baseLineFunc = ItemLimit_baseLine;
	i->_item._drawFunc = ItemLimit_draw;

	i->_item._setFocusFunc = Item_setFocus;
	i->_item._getFocusFunc = Item_getFocus;

	i->_item._objectType = OBJ_Sigma;
	i->_item._procLevel = PROC_L_9;

	i->_item._hasFocus = 0;

	return i;
}

void ItemTreeChild_free(Item* item)
{
	Item* left = item->_left;
	Item* right = item->_right;

	if (left)
	{
		ItemTreeChild_free(left);
		left->_destroyFunc(left);
		free(left);
	}

	if (right)
	{
		ItemTreeChild_free(right);
		right->_destroyFunc(right); 
		free(right);
	}
}

void ItemTree_free(Item** Item)
{
	ItemTreeChild_free(*Item);
	(*Item)->_destroyFunc(*Item);
	free(*Item);
}

Item* Item_getParent(Item* items, Item* i)
{
	Item* left = items->_left;
	Item* right = items->_right;

	if (items == i)
		return NULL;
	
	if (left)
	{
		if (left == i)
			return items;

		Item* r = Item_getParent(left, i);
		if (r)
			return r;
	}

	if (right)
	{
		if (right == i)
			return items;

		Item* r = Item_getParent(right, i);
		if (r)
			return r;
	}

	return NULL;
}
