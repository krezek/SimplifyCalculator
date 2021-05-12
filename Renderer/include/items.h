#ifndef ItemS_H_
#define ItemS_H_

#include <wchar.h>

#include "strg.h"

#ifdef _WIN32
#include "graphics.h"
#elif __linux__
#include "graphics_linux.h"
#endif


typedef enum {OBJ_Base = 0, OBJ_Literal, OBJ_Number, OBJ_Symbol, OBJ_List, OBJ_Equ, OBJ_Add, OBJ_Sub, OBJ_Mult, OBJ_Frac,
				OBJ_Sign, OBJ_Sqrt, OBJ_CommFunc, OBJ_Pow, OBJ_Subscript, OBJ_Factorial, OBJ_Parentheses, OBJ_Sigma,
				OBJ_Integrate, OBJ_Derivative} ObjectType;
typedef enum { PROC_L_0 = 0, PROC_L_1, PROC_L_2, PROC_L_3, PROC_L_4, PROC_L_5, PROC_L_6, PROC_R_7, PROC_R_8, PROC_L_9} ProcLevel;

typedef struct _Item Item;

typedef void (*destroyFunc) (Item* _this);
typedef int (*isLeafFunc) (Item* _this);
typedef void (*toStringFunc) (Item* _this, String* s);
typedef Item* (*cloneFunc) (Item* _this);

typedef void (*setFontFunc) (Item* _this, FontHandle hFont);
typedef void (*calcCoordinateFunc) (Item* _this, Graphics* gfx, int x, int y);
typedef int (*widthFunc) (Item* _this, Graphics* gfx);
typedef int (*heightFunc) (Item* _this, Graphics* gfx);
typedef int (*baseLineFunc) (Item* _this, Graphics* gfx);
typedef void (*drawFunc) (Item* _this, Graphics* gfx);

typedef void (*setFocusFunc)(Item* _this, unsigned char b);
typedef unsigned char (*getFocusFunc)(Item* _this);

typedef struct _Item
{
	struct _Item* _left;
	struct _Item* _right;

	destroyFunc _destroyFunc;
	isLeafFunc _isLeafFunc;
	toStringFunc _toStringFunc;
	cloneFunc _cloneFunc;

	setFontFunc _setFontFunc;
	calcCoordinateFunc _calcCoordinateFunc;
	widthFunc _widthFunc;
	heightFunc _heightFunc;
	baseLineFunc _baseLineFunc;
	drawFunc _drawFunc;

	setFocusFunc _setFocusFunc;
	getFocusFunc _getFocusFunc;

	ObjectType _objectType;
	ProcLevel _procLevel;

	FontHandle _hFont;
	unsigned char _hasFocus;
	int _x, _y, _width, _height, _baseLine;
} Item;

typedef struct
{
	Item _item;

	String* _str;
} ItemLiteral;

typedef struct
{
	Item _item;

	String* _str;
} ItemNumber;

typedef struct
{
	Item _item;

	wchar_t _ch;
} ItemSymbol;

typedef struct
{
	Item _item;
} ItemList;

typedef struct
{
	Item _item;
	wchar_t _sy;
} ItemEqu;

typedef struct
{
	Item _item;
} ItemAdd;

typedef struct
{
	Item _item;
} ItemSub;

typedef struct
{
	Item _item;
} ItemMult;

typedef struct
{
	Item _item;

	int _fHeight;
} ItemFrac;

typedef struct
{
	Item _item;

	wchar_t _sgn;
} ItemSign;

typedef struct
{
	Item _item;

	int _sHeight;
	int _sPadding;
} ItemSqrt;

typedef struct
{
	Item _item;

	wchar_t* _str;
	size_t _len;
} ItemCommFunc;

typedef struct
{
	Item _item;
} ItemPow;

typedef struct
{
	Item _item;
} ItemSubscript;

typedef struct
{
	Item _item;
} ItemFactorial;

typedef struct
{
	Item _item;
} ItemParentheses;

typedef struct _ItemSigma
{
	Item _item;

	struct _Item* _bottom;
	struct _Item* _top;
} ItemSigma;

typedef struct _ItemIntegrate
{
	Item _item;

	struct _Item* _bottom;
	struct _Item* _top;
} ItemIntegrate;

typedef struct _ItemDerivative
{
	Item _item;
} ItemDerivative;

typedef struct _ItemLimit
{
	Item _item;

	struct _Item* _t1;
	struct _Item* _t2;
} ItemLimit;

Item* Item_init(Item* l, Item* r);

ItemLiteral* ItemLiteral_init(const wchar_t* s);
ItemNumber* ItemNumber_init(const wchar_t* s, int sign);
ItemSymbol* ItemSymbol_init(const wchar_t ch);

ItemList* ItemList_init(Item* l, Item* r);
ItemEqu* ItemEqu_init(Item* l, Item* r, wchar_t sy);
ItemEqu* ItemEqu_init_eq(Item* l, Item* r);
ItemEqu* ItemEqu_init_nq(Item* l, Item* r);
ItemEqu* ItemEqu_init_g(Item* l, Item* r);
ItemEqu* ItemEqu_init_l(Item* l, Item* r);
ItemEqu* ItemEqu_init_eg(Item* l, Item* r);
ItemEqu* ItemEqu_init_el(Item* l, Item* r);


ItemAdd* ItemAdd_init(Item* l, Item* r);
ItemSub* ItemSub_init(Item* l, Item* r);

ItemMult* ItemMult_init(Item* l, Item* r);
ItemFrac* ItemFrac_init(Item* l, Item* r);

ItemSign* ItemSign_init(Item* l, const wchar_t sgn);
ItemSign* ItemSignMinus_init(Item* l);
ItemSign* ItemSignPlus_init(Item* l);

ItemSqrt* ItemSqrt_init(Item* l);
ItemPow* ItemPow_init(Item* l, Item* r);
ItemSubscript* ItemSubscript_init(Item* l, Item* r);
ItemCommFunc* ItemCommFunc_init(Item* l, Item* r, const wchar_t* s);

ItemCommFunc* ItemSinFunc_init(Item* l);
ItemCommFunc* ItemCosFunc_init(Item* l);
ItemCommFunc* ItemTanFunc_init(Item* l);
ItemCommFunc* ItemLogFunc_init(Item* l);
ItemCommFunc* ItemExpFunc_init(Item* l);
ItemCommFunc* ItemAsinFunc_init(Item* l);
ItemCommFunc* ItemAcosFunc_init(Item* l);
ItemCommFunc* ItemAtanFunc_init(Item* l);
ItemCommFunc* ItemLnFunc_init(Item* l);

ItemFactorial* ItemFactorial_init(Item* l);

ItemParentheses* ItemParentheses_init(Item* l);

ItemSigma* ItemSigma_init(Item* l, Item* b, Item* t);
ItemIntegrate* ItemIntegrate_init(Item* l, Item* r, Item* b, Item* t);
ItemDerivative* ItemDerivative_init(Item* l, Item* r);
ItemLimit* ItemLimit_init(Item* l, Item* t1, Item* t2);

void ItemTree_free(Item** Item);
Item* Item_getParent(Item* items, Item* i);

#endif /* ItemS_H_ */

