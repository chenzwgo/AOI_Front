#ifndef _INC_EXPRESSION_H
#define _INC_EXPRESSION_H
#include <stack>
#include "token.h"
#include "../operator/operator.h"
#include "../operatordata/operatordata.h"

using namespace  std;

/*
 * Expression类是解析表达式的类
 * 表达式支持 + - * / ^(pow) -(neg)运算符
 * 表达式支持 ( ) 控制优先级
 * 表达式中的实数只能为实数加小数点，不支持科学计数法表示(不会错误提示，但得不到正确表达式)
 * SetExprStr失败后会破坏原来的表达式
 * 支持26个字母(不区分大小写)表示变量
 * 使用SetVar设置变量值
 * 默认变量值为 1
 * GetResult时，若表达式求解不合法，返回非0
 * GetExprErrorStr 获取表达式错误代码的表示字符串
 * GetResultErrorStr 获取求解错误代码的表示字符串
 */

// 表示语法单位的结构体
enum AtomType{ATOM_ERROR = 0, ATOM_LEFTPAR,ATOM_RIGHTPAR,ATOM_SYMBOL, ATOM_CONSTANT, ATOM_VARIABLE, ATOM_END};
typedef struct
{
    AtomType type;
    string value;
} Atom;


// 表达式
class Expression
{
public:
	Expression();
	~Expression();
	
	const char *GetExprErrorStr(int err);	                // 根据错误码返回对应的错误提示
	const char *GetResultErrorStr(int err);	                // 根据错误码返回对应的错误提示
	int SetExprStr(const vector<Atom*> &tokens);	        // 设置表达式，在内部生成逆波兰式表达式，成功返回0，失败返回非零错误码
    int GetResult();			                // 根据设置的变量和上一次设置的表达式获取表达式的值放入result，成功返回0，错误返回非零

private: 
    vector<Atom *> PolandStack;

};

#endif	// _INC_EXPRESSION_H
