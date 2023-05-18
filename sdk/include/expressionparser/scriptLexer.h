#ifndef  _SCRIPTLEXER_H_
#define  _SCRIPTLEXER_H_
#include "g12commonexport.h"
#include "tokentype.h"
#include "MathFunction.h"
#include <vector>
#include <string>
#include <map>
#include <stack>
using namespace  std;

enum En_ERROR
{
    En_NOERROR = 0,
    En_DIVIDENDISZERO ,					// 除数为零
    En_OPERATORERROR ,					// 操作数错误
	En_FUNRERROR ,						// 函数错误
	En_DOESNOTMATCHTHEPARENTHESES ,		// 其他错误
};

class RESOURCE_LIBRARY_EXPORT ScriptLexer
{
public:
    enum LexState
    {
        Space,
        CommentOrDivideOrAssignDivide,
        LineComment,
        BlockCommentStart,
        BlockCommentEnd,
        AssignOrEqual,
        PlusOrIncrementOrAssignPlus,
        MinusOrDecrementOrAssignMinus,
        MultiplyOrAssignMultiply,
        PowerOrAssignPower,
        ModuloOrAssignModulo,
        And,
        Or,
        NotOrNotEqual,
        GreaterOrGreaterEqual,
        LessOrLessEqual,
        IdentifierOrKeyword,
        String,
        StringEscape,
        IntegerOrFloat,
        Float,
        ArrayLeftBracket,
        ArrayRightBracket
    };

	ScriptLexer(int iStation = 0);
	void setChannel(string strChannel);
    ScriptLexer(const string & srcString, int iStation = 0);

	string getParserString();
	void setParserString(const string & srcString);

	void reInit();

    int parser();

    int evaluation(double &results);                  //求值

	const string& getErrString();

	void addVarValue(const string& strVarName, double dValue);

	bool isParser();

	bool getFullParserString(const string & srcString, string& strFullString);
	
	 bool GetTokensOneLine(const string strLine, vector<Token> & Tokens, string& strErr);

	static  bool IsDigit(const char& ch);

	static  bool IsLetter(const char& ch);

	static  bool IsLetterOrDigit(const char& ch);

	static  bool isOperator(char Optch);

	static  bool isVariable(const string& strOpt);

	static  bool isFunction(const string& strOpt);

	static  bool isOperatorString(const string& strOpt);

	static  bool isOptNumber(const Token& pre);

	static  bool islegel(const Token& pre, const Token& now);
	
	 bool isExpressionVariable(const string& strOpt);

	 string getExpressionVariableStr(const string& strOpt);

	static bool isRightBracketsString(const string& strOpt);

private:
    int getLevel(const string &oper);      //得到运算符等级

    int toPostfix();                        //转换成后缀表达式

    bool getVarValue(const string& strVarName,double& dValue);
    
    bool getFunValue(const string& strFunName,double& dValue);

private:
    string strErrString;
    static ScriptLexer* m_pInstance;
	string m_strChannel;

private:
    stack<Token> m_expStack;         //后缀表达式栈
    stack<Token> m_opStack;          //运算符栈
    vector<Token> m_postfix;
    vector<Token> m_exp;
    string m_srcString;
    map<string,MathFunction*>   m_Funsmap;
    map<string,double>          m_Varmap;
    int funLable; 
	bool m_bParserflag;
	int  m_iStation;
};

#endif
