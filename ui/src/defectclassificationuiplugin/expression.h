#ifndef _INC_EXPRESSION_H
#define _INC_EXPRESSION_H
#include <stack>
#include "token.h"
#include "../operator/operator.h"
#include "../operatordata/operatordata.h"

using namespace  std;

/*
 * Expression���ǽ������ʽ����
 * ���ʽ֧�� + - * / ^(pow) -(neg)�����
 * ���ʽ֧�� ( ) �������ȼ�
 * ���ʽ�е�ʵ��ֻ��Ϊʵ����С���㣬��֧�ֿ�ѧ��������ʾ(���������ʾ�����ò�����ȷ���ʽ)
 * SetExprStrʧ�ܺ���ƻ�ԭ���ı��ʽ
 * ֧��26����ĸ(�����ִ�Сд)��ʾ����
 * ʹ��SetVar���ñ���ֵ
 * Ĭ�ϱ���ֵΪ 1
 * GetResultʱ�������ʽ��ⲻ�Ϸ������ط�0
 * GetExprErrorStr ��ȡ���ʽ�������ı�ʾ�ַ���
 * GetResultErrorStr ��ȡ���������ı�ʾ�ַ���
 */

// ��ʾ�﷨��λ�Ľṹ��
enum AtomType{ATOM_ERROR = 0, ATOM_LEFTPAR,ATOM_RIGHTPAR,ATOM_SYMBOL, ATOM_CONSTANT, ATOM_VARIABLE, ATOM_END};
typedef struct
{
    AtomType type;
    string value;
} Atom;


// ���ʽ
class Expression
{
public:
	Expression();
	~Expression();
	
	const char *GetExprErrorStr(int err);	                // ���ݴ����뷵�ض�Ӧ�Ĵ�����ʾ
	const char *GetResultErrorStr(int err);	                // ���ݴ����뷵�ض�Ӧ�Ĵ�����ʾ
	int SetExprStr(const vector<Atom*> &tokens);	        // ���ñ��ʽ�����ڲ������沨��ʽ���ʽ���ɹ�����0��ʧ�ܷ��ط��������
    int GetResult();			                // �������õı�������һ�����õı��ʽ��ȡ���ʽ��ֵ����result���ɹ�����0�����󷵻ط���

private: 
    vector<Atom *> PolandStack;

};

#endif	// _INC_EXPRESSION_H
