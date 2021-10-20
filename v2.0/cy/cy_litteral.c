/*
 *	@(#)	[MB] cy_litteral.c	Version 1.1 du 19/10/13 - 
 */

#include	<stdio.h>
#include	<stdlib.h>
#include	<string.h>

#define	OP_ADD		(1)
#define	OP_SUB		(2)
#define	OP_MUL		(3)
#define	OP_DIV		(4)

#define	Z			printf("%s(%d)\n", __FILE__, __LINE__);

struct elt {
	char			*value;
	int
#if 0
				 need_paren_sub,
				 need_paren_mul,
				 need_paren_div,
#endif
				 need_parentheses;
};
typedef struct elt	 elt;

struct op {
	char			*name;
	int			 type;
};

elt *new_elt(char *value)
{
	elt			*_elt;

	if ((_elt = malloc(sizeof(*_elt))) == 0) {
		fprintf(stderr, "Malloc error !\n");
		exit(1);
	}

	_elt->value			= strdup(value);
#if 0
	_elt->need_paren_sub	= 0;
	_elt->need_paren_mul	= 0;
	_elt->need_paren_div	= 0;
#endif
	_elt->need_parentheses	= 0;

	return _elt;
}

char *parentheses(char *str)
{
	char				 _buf[1024];

	sprintf(_buf, "(%s)", str);
	return strdup(_buf);
}

elt *op(elt *a, elt *b, int op_type)
{
	elt				*_result;
	char				 _buf[1024], *_op_name, *_a, *_b;

	switch (op_type) {

	case	OP_ADD:
		_op_name		= "+";
		_a			= a->value;
		_b			= b->value;

		sprintf(_buf, "%s + %s", _a, _b);
		_result		= new_elt(strdup(_buf));
//		_result->need_paren_sub		= 1;
//		_result->need_paren_mul		= 1;
//		_result->need_paren_div		= 1;
		_result->need_parentheses	= 1;
		break;

	case	OP_SUB:
		_op_name		= "-";
		_a			= a->value;
		if (b->need_parentheses) {
			_b			= parentheses(b->value);
		}
		else {
			_b			= b->value;
		}
		sprintf(_buf, "%s - %s", _a, _b);
		_result		= new_elt(strdup(_buf));
//		_result->need_paren_sub		= 1;
//		_result->need_paren_mul		= 1;
//		_result->need_paren_div		= 1;
		_result->need_parentheses	= 1;
		break;

	case	OP_MUL:
		_op_name		= "*";
		if (a->need_parentheses) {
			_a			= parentheses(a->value);
		}
		else {
			_a			= a->value;
		}
		if (b->need_parentheses) {
			_b			= parentheses(b->value);
		}
		else {
			_b			= b->value;
		}
		sprintf(_buf, "%s.%s", _a, _b);
		_result		= new_elt(strdup(_buf));
//		_result->need_paren_sub		= 0;
//		_result->need_paren_mul		= 0;
//		_result->need_paren_div		= 0;
		_result->need_parentheses	= 0;
		_result		= new_elt(strdup(_buf));
		break;

	case	OP_DIV:
		_op_name		= "/";
		if (a->need_parentheses) {
			_a			= parentheses(a->value);
		}
		else {
			_a			= a->value;
		}
		if (b->need_parentheses) {
			_b			= parentheses(b->value);
		}
		else {
			_b			= b->value;
		}
		sprintf(_buf, "%s/%s", _a, _b);
		_result		= new_elt(strdup(_buf));
#if 0
		_result->need_paren_sub		= 1;
		_result->need_paren_mul		= 1;
		_result->need_paren_div		= 1;
#endif
		_result->need_parentheses	= 1;
		_result		= new_elt(strdup(_buf));
		break;

	default:
		fprintf(stderr, "Unknown op (%d)\n", op_type);
		exit(1);
		break;
	}

	printf("RESULT : [%s] [%s] %s  => [%s]\n", 
	       a->value, b->value, _op_name, _result->value);

	return _result;
}

int main()
{
	elt			*_a, *_b, *_r, *_c, *_d, *_r1, *_r2, *_1,
				*_a11, *_a12, *_a21, *_a22,
				*_b11, *_b12, *_b21, *_b22;

	_a			= new_elt("a");
	_b			= new_elt("b");
	_c			= new_elt("c");
	_d			= new_elt("d");

	_1			= new_elt("1");

	_a11			= new_elt("a11");
	_a12			= new_elt("a12");
	_a21			= new_elt("a21");
	_a22			= new_elt("a22");

	_b11			= new_elt("b11");
	_b12			= new_elt("b12");
	_b21			= new_elt("b21");
	_b22			= new_elt("b22");

	_r			= op(_a, _b, OP_ADD);

	_r			= op(_r, _c, OP_SUB);

	_r			= op(_d, _r, OP_SUB);

	_r1			= op(_a, _b, OP_ADD);
	_r2			= op(_c, _d, OP_SUB);
	_r			= op(_r1, _r2, OP_MUL);

	_r1			= op(_a, _b, OP_MUL);
	_r2			= op(_c, _d, OP_MUL);
	_r			= op(_r1, _r2, OP_SUB);

	_r1			= op(_a, _b, OP_MUL);
	_r2			= op(_c, _d, OP_MUL);
	_r			= op(_r1, _r2, OP_MUL);

	_r			= op(_r1, _r2, OP_DIV);

	_r1			= op(_a, _b, OP_SUB);
	_r2			= op(_c, _d, OP_ADD);
	_r			= op(_r1, _r2, OP_DIV);

	_r1			= op(_a11, _b11, OP_MUL);
	_r2			= op(_a12, _b21, OP_MUL);
	_r			= op(_r1, _r2, OP_ADD);

	_r1			= op(_a21, _b12, OP_MUL);
	_r2			= op(_a22, _b22, OP_MUL);
	_r			= op(_r1, _r2, OP_ADD);

	_r1			= op(_a, _b, OP_SUB);
	_r			= op(_1, _r1, OP_DIV);

	return 0;
}
