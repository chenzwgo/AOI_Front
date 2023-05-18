#include "expression.h"

#include <stdlib.h>
#include <math.h>
#include <float.h>

// 判断符号是否合法
enum BL{F = 0, T = 1};	// T true，F false
enum CR{A = 0, B = 1, D = 2, E = 3};	// A大于，B小于，D相等，E错误

BL legel_Atom(Atom *atom1, Atom *atom2);
BL legel_Atom(const string& ch1, const string& ch2);
// 兼容矩阵
BL legel_matrix[10][10] =
{
    //	+	-	*	/	^	(	)	i	~	#
    F,	F,	F,	F,	F,	T,	F,	T,	T,	F,	// +
    F,	F,	F,	F,	F,	T,	F,	T,	T,	F,	// -
    F,	F,	F,	F,	F,	T,	F,	T,	T,	F,	// *
    F,	F,	F,	F,	F,	T,	F,	T,	T,	F,	// /
    F,	F,	F,	F,	F,	T,	F,	T,	T,	F,	// ^
    F,	F,	F,	F,	F,	T,	F,	T,	T,	F,	// (
    T,	T,	T,	T,	T,	F,	T,	F,	F,	T,	// )
    T,	T,	T,	T,	T,	F,	T,	F,	F,	T,	// i
    F,	F,	F,	F,	F,	T,	F,	T,	F,	F,	// ~
    F,	F,	F,	F,	F,	T,	F,	T,	T,	F,	// #
};

// 兼容矩阵下标值
//char legel_matrix_ch[10] = {'+', '-', '*', '/', '^', '(', ')', 'i', '~', '#', };

const char* legel_matrix_ch[10] = {"+", "-", "*", "/", "^", "(", ")", "i", "~", "#" };

// 优先级矩阵
CR cmp_matrix[9][9] =
{
    //	+	-	*	/	^	(	)	~	#
    A,	A,	B,	B,	B,	B,	A,	B,	A,	// +
    A,	A,	B,	B,	B,	B,	A,	B,	A,	// -
    A,	A,	A,	A,	B,	B,	A,	B,	A,	// *
    A,	A,	A,	A,	B,	B,	A,	B,	A,	// /
    A,	A,	A,	A,	A,	B,	A,	B,	A,	// ^
    B,	B,	B,	B,	B,	B,	D,	B,	E,	// (
    E,	E,	E,	E,	E,	E,	E,	E,	E,	// )
    A,	A,	A,	A,	A,	B,	A,	A,	A,	// ~
    B,	B,	B,	B,	B,	B,	E,	B,	D,	// #
};

// 优先级矩阵下标值
//char cmp_matrix_ch[9] = {'+', '-', '*', '/', '^', '(', ')', '~', '#', };

const char* cmp_matrix_ch[9] = {"+", "-", "*", "/", "^", "(", ")", "~", "#"};

const int expr_str_err_N = 5;
const char *expr_str_err[] = 
{
	"unknown error.",			// error code 0
	"unexcepted end.",			// error code 1
	"operator error.",			// error code 2
	"input data error.",		// error code 3
	"expression too long."		// error code 4
};

const int compute_err_N = 5;
const char *compute_err[] = 
{
	"unknown error.",			// error code 0
	"compute data error.",		// error code 1
	"input data error.",		// error code 2
	"devided by zero.",			// error code 3
	"pow return non."			// error code 4
};
const int sml_N = 7;
const char sml[] = {'+', '-', '*', '/', '^', '(', ')'};

// 获取Atom的字符表示
const string& atom_symbol(Atom *atom) ;
// 判断符号优先级
CR compare_symbol(const string& ch1, const string& ch2);

Expression::Expression()
{

}

Expression::~Expression()
{

}

int Expression::SetExprStr(const vector<Atom*> &tokens)
{
	Atom *pBak = new Atom();
    Atom* pNow = NULL;
	int symbol_stack_N = 0;
	pBak->type = ATOM_END;
	int index = 0;
	int error_code = 0;
    stack<Atom*> s2;
    for (unsigned int i = 0; i < tokens.size();i++)
    {
        pNow = tokens.at(i);
        if (!legel_Atom(pBak, pNow))	// 判断是否合法
        {
            if (pNow->type == ATOM_SYMBOL && pNow->value == "-")	// 尝试一元运算符的多义
            {
                if(!legel_Atom(pBak, pNow))
                {
                    error_code = 2;
                    return error_code;
                }
            }
            else
            {
                error_code = 2;
                return error_code;
            }
        }
        switch (pNow->type)	// 根据类别计算
        {
        case ATOM_ERROR:
            error_code = 3;
            return error_code;
            break;
        case ATOM_CONSTANT:	// 变量或常量，自己压入逆波兰式
        case ATOM_VARIABLE:
            PolandStack.push_back(pNow);
            break;
        case ATOM_LEFTPAR:
            s2.push(pNow);
            break;
        case ATOM_RIGHTPAR:
            {
                Atom* pTmp  = NULL;
                do 
                {
                    if (s2.empty())
                    {
                        break;
                    }
                    pTmp = s2.top();
                    s2.pop();
                    if (pTmp->type != ATOM_LEFTPAR)
                    {
                        PolandStack.push_back(pTmp);
                    }           
                } while (!s2.empty() && pTmp->type != ATOM_LEFTPAR);
            }

            break;
        case ATOM_SYMBOL:	
            {
                if (s2.empty() || s2.top()->type == ATOM_LEFTPAR)
                {
                    s2.push(pNow);
                    break;
                }

                
                bool poped = true;
                string ch = atom_symbol(pNow);
                while (poped && !s2.empty())
                {
                    Atom* pTmp  = s2.top();
                    string strTop= pTmp->value;
                    switch (compare_symbol(ch,strTop))	// 比较当前运算符和符号栈顶的优先级
                    {
                    case A:	// 上一个符号优先级高，先完成上一个运算符的运算
                        s2.push(pNow);
                        poped = false;
                        break;
                    case B:	// 这次的优先级高，则压入符号栈，等待处理
                    case D:	// 优先级相等，例如'('和')'，同时弹出
                        PolandStack.push_back(pTmp);
                        s2.pop();
                        poped = true;
                        break;
                    case E:	// 优先级出错
                        error_code = 2;
                        return error_code;
                    }
                }
            }
            break;
        }
        *pBak = *pNow;
    }
    
    while(!s2.empty())
    {
        PolandStack.push_back(s2.top());
        s2.pop();
    }
    return error_code;
}

const char *Expression::GetExprErrorStr(int err)
{
	return err < 1 ?
		expr_str_err[0] :
		err >= expr_str_err_N ?
			expr_str_err[0] :
			expr_str_err[err];
}

const char *Expression::GetResultErrorStr(int err)
{
	return err < 1 ?
		compute_err[0] :
		err >= compute_err_N ?
			compute_err[0] :
			compute_err[err];
}

int Expression::GetResult()
{
	int index = 0;
	int error_code = 0;
    int iLength = PolandStack.size();
    stack<Operatordata *> rstData;
	
	for (index = 0; index < iLength; index++)
	{	
        Atom *pAtom = PolandStack.at(index);
        AtomType aType =pAtom->type;

		switch(aType)
		{
		case ATOM_ERROR:
			error_code = 2;
			return error_code;
			break;
		case ATOM_SYMBOL:
            {
                string stropr = pAtom->value;
                if (rstData.size() < 2)
                {
                    error_code = 1;
                    return error_code;
                }
                Operator* operacion = pAtom->value.operacion;
                Operatordata* oper1 = rstData.top(); rstData.pop();
                Operatordata* oper2 = rstData.top(); rstData.pop();
                Operatordata*  pReValue = NULL;
                if ( oper1->getTip() == "Integer" )
                {
                    pReValue =  new OpInteger( ) ;
                } 
                else if ( oper1->getTip() == "String" )
                {
                    pReValue =  new OpString();
                } 
                else if ( oper1->getTip() == "Boolean" )
                {
                    pReValue =  new OpBoolean( );
                } 
                else if ( oper1->getTip() == "Float" )
                {
                    pReValue =  new OpFloat() ;
                }
                if (pReValue == NULL)
                {
                    error_code = 1;
                    return error_code;
                }
                //createVarOperatorData(OPdata);            
                pReValue->setValue(operacion->excute(oper1,oper2));
                rstData.push(pReValue);
            }
			break;
		case ATOM_CONSTANT:
		case ATOM_VARIABLE:
			rstData.push(pAtom->value.oper);
			break;
		case ATOM_END:
			break;
        default:
            break;
		}
	}
	
	if (rstData.size() != 1)
	{
		error_code = 1;
	}
	else
	{
		result = rstData.top();
        rstData.pop();
		error_code =  0;
	}

	return error_code;
}

BL legel_Atom(Atom *atom1, Atom *atom2)
{
    string ch1 = atom_symbol(atom1);
    if (ch1.length() ==  0) return F;
    string ch2 = atom_symbol(atom2);
    if (ch2.length() ==  0) return F;
    return legel_Atom(ch1, ch2);
}

BL legel_Atom(const string& ch1, const string& ch2)
{
	int index1 = 0;
	while (strcmp(legel_matrix_ch[index1],ch1.c_str()) !=0  && index1 < 10) index1++;
	if (index1 == 10) return F;
	int index2 = 0;
	while (strcmp(legel_matrix_ch[index2],ch1.c_str()) !=0 && index2 < 10) index2++;
	if (index2 == 10) return F;
	return legel_matrix[index1][index2];
}

const string& atom_symbol(Atom *atom)
{
	switch (atom->type)
	{
	case ATOM_ERROR:
		return "";
	case ATOM_SYMBOL:
		return atom->value.operacion->getSimbolo();
	case ATOM_CONSTANT:
	case ATOM_VARIABLE:
		return "i";
	case ATOM_END:
		return "#";
	}
	return "";
}


CR compare_symbol (const string& ch1, const string& ch2)
{
	int index1 = 0;
	while (strcmp(cmp_matrix_ch[index1],ch1.c_str()) != 0 && index1 < 9) index1++;
	if (index1 == 9) return E;
	int index2 = 0;
	while (strcmp(cmp_matrix_ch[index2],ch2.c_str()) != 0 && index2 < 9) index2++;
	if (index2 == 9) return E;
	return cmp_matrix[index1][index2];
}
