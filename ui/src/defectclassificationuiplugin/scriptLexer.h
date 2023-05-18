#ifndef  _SCRIPTLEXER_H_
#define  _SCRIPTLEXER_H_
#include "tokentype.h"
#include "MathFunction.h"
#include <vector>
#include <string>
#include <map>
#include <stack>
#include <QObject>
#include <QStack>
using namespace  std;

enum ERROR
{
    NOERROR = 0,
    DIVIDENDISZERO = 1,
    OPERATORERROR = 2,
    DOESNOTMATCHTHEPARENTHESES = 3
};

class ScriptLexer 
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

    //static ScriptLexer* getInstance();
    //static void releaseInstance();

    ScriptLexer(const vector<Token> & Tokens);

    static bool GetTokensOneLine(const string strLine,vector<Token> & Tokens,string& strErr);

    int parser();

    int evaluation(double &results);                  //��ֵ

    void init();

    int getLevel(const string &oper);      //�õ�������ȼ�

    int toPostfix();                        //ת���ɺ�׺���ʽ

    bool getVarValue(const string& strVarName,double& dValue);
    
    bool getFunValue(const string& strFunName,double& dValue);

    const string& getErrString();

    static  bool IsDigit(const char& ch);

    static  bool IsLetter(const char& ch);

    static  bool IsLetterOrDigit(const char& ch);

    static  bool isOperator( char Optch );

    static  bool isVariable( const string& strOpt );

    static  bool isFunction( const string& strOpt );

    static  bool isOperatorString( const string& strOpt );

    static  bool isOptNumber(const Token& pre);

    static  bool islegel(const Token& pre,const Token& now);

private:
    string strErrString;
    static ScriptLexer* m_pInstance;

private:
    stack<Token> m_expStack;         //��׺���ʽջ
    stack<Token> m_opStack;          //�����ջ
    vector<Token> m_postfix;
    vector<Token> m_exp;
    map<string,MathFunction*>   m_Funsmap;
    map<string,double>          m_Varmap;
    int funLable; 
};

#endif
