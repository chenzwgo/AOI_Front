#include "scriptLexer.h"
#include <QDebug>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <math.h>
#include "Features.h"
#include "stringcommon.h"

double acosh (const double x);  
double atanh (const double x);  
double asinh (const double x);  
double log1p (const double x);  
double log2 (const double x); 

#define GSL_SQRT_DBL_EPSILON   1.4901161193847656e-08  
#define GSL_DBL_EPSILON        2.2204460492503131e-16  
#ifndef M_LN2  
#define M_LN2      0.69314718055994530941723212146      /* ln(2) */  
#endif 


ScriptLexer* ScriptLexer::m_pInstance = NULL;

ScriptLexer::ScriptLexer(const vector<Token> & Tokens)
{
    m_exp = Tokens;
}
//ScriptLexer* ScriptLexer::getInstance()
//{
//    if (m_pInstance == NULL)
//    {
//        m_pInstance = new ScriptLexer();
//    }
//    return m_pInstance;
//}
//
//void ScriptLexer::releaseInstance()
//{
//    if (m_pInstance != NULL)
//    {
//        delete m_pInstance ;
//        m_pInstance = NULL;
//    }
//    return ;
//}


bool ScriptLexer::GetTokensOneLine(const string strLine,vector<Token> & Tokens,string& strErr)
{
    int iSourceChar = 0;
    LexState lexState = Space;
    string strToken = "";
    bool bSourceEnd= false;
    const char* strSourceLine = strLine.c_str();

    while (iSourceChar < strLine.length() && iSourceChar >=0 && !bSourceEnd)
    {
        char ch = strSourceLine[iSourceChar++];        
        switch (lexState)
        {
        case Space:
            switch (ch)
            {
            case ' ':
            case '\t':
            case '\r':
            case '\n':
                break; // ignore whitespace
            case  '#':
                lexState =  LexState::LineComment;
                break;
            case '{':
            case '}':
            case '(':
            case ')':
            case ']':
            case '.':
            case ',': 
            case ';':
            case ':':
                {
                    strToken = "";
                    strToken += ch;
                    CTokenType::EnTokenType  eType = CTokenType::TokenStringToType(strToken);
                    Tokens.push_back(Token(eType,strToken));
                }
                break;
            case '[':
                strToken = "";
                strToken += ch;
                lexState =  ArrayLeftBracket;
                break;
            case '=':
                strToken = "";
                strToken += ch;
                lexState =  AssignOrEqual;
                break;
            case '+':
                strToken = "";
                strToken += ch;
                lexState =  PlusOrIncrementOrAssignPlus;
                break;
            case '-':
                strToken = "";
                strToken += ch;
                lexState =  MinusOrDecrementOrAssignMinus;
                break;
            case '*':
                strToken = "";
                strToken += ch;
                lexState =  MultiplyOrAssignMultiply;
                break;
            case '/':
                strToken = "";
                strToken += ch;
                lexState =  CommentOrDivideOrAssignDivide;
                break;
            case '^':
                strToken = "";
                strToken += ch;
                lexState =  PowerOrAssignPower;
                break;
            case '%':
                strToken = "";
                strToken += ch;
                lexState =  ModuloOrAssignModulo;
                break;
            case '&':
                strToken = "";
                strToken += ch;
                lexState =  And;
                break;
            case '|':
                strToken = "";
                strToken += ch;
                lexState =  Or;
                break;
            case '!':
                strToken = "";
                strToken += ch;
                lexState =  NotOrNotEqual;
                break;
            case '>':
                strToken = "";
                strToken += ch;
                lexState =  GreaterOrGreaterEqual;
                break;
            case '<':
                strToken = "";
                strToken += ch;
                lexState =  LessOrLessEqual;
                break;
            case '\"':
                strToken = "";
                //strToken += ch;
                lexState =  String;
                break;
            default:
                if (ch == '_' || IsLetter(ch))
                {
                    lexState =  IdentifierOrKeyword;
                    strToken = "";
                    strToken += ch;
                }
                else if (IsDigit(ch))
                {
                    strToken = "";
                    strToken += ch;
                    lexState =  IntegerOrFloat;
                }
                else
                    return false;
                break;
            } 
            break;
        case  CommentOrDivideOrAssignDivide:
            switch (ch)
            {
            case '/':
                lexState =  LexState::LineComment;
                break;
            case '*':
                lexState =  BlockCommentStart;
                break;
            case '=':
                Tokens.push_back( Token( CTokenType::AssignDivide, "/="));
                lexState =  Space;
                break;
            default:
                Tokens.push_back( Token( CTokenType::Divide, "/" ));
                if (iSourceChar>0) --iSourceChar;
                lexState =  Space;
                break;
            }
            break;
        case  LineComment:
            if (ch == '\n')
                lexState =  Space;
            break;
        case  BlockCommentStart:
            if (ch == '*')
                lexState =  BlockCommentEnd;
            break;
        case  BlockCommentEnd:
            if (ch == '/')
                lexState =  Space;
            else
                lexState =  BlockCommentStart;
            break;
        case  AssignOrEqual:
            if (ch == '=')
            {
                Tokens.push_back( Token( CTokenType::Equal, "=="));
                lexState =  Space;
            }
            else
            {
                Tokens.push_back( Token( CTokenType::Assign, "="));
                if (iSourceChar>0) --iSourceChar;
                lexState =  Space;
            }
            break;
        case  PlusOrIncrementOrAssignPlus:
            if (ch == '+')
            {
                Tokens.push_back( Token( CTokenType::Increment, "++" ));
                lexState =  Space;
            }
            else if (ch == '=')
            {
                Tokens.push_back( Token( CTokenType::AssignPlus, "+=" ));
                lexState =  Space;
            }
            else
            {
                Tokens.push_back( Token( CTokenType::Plus, "+" ));
                if (iSourceChar>0) --iSourceChar;
                lexState =  Space;
            }
            break;
        case  MinusOrDecrementOrAssignMinus:
            if (ch == '-')
            {
                Tokens.push_back( Token( CTokenType::Decrement, "--" ));
                lexState =  Space;
                break;
            }
            else if (ch == '=')
            {
                Tokens.push_back( Token( CTokenType::AssignMinus, "-=" ));
                lexState =  Space;
                break;
            }
            if (IsDigit(ch))
            {   
                if (Tokens.size() -1 >=0 )
                {
                    Token preToken = Tokens.at(Tokens.size() -1);
                    if (preToken.tokenType == CTokenType::Assign || 
                        preToken.tokenType == CTokenType::Plus || 
                        preToken.tokenType == CTokenType::Minus ||
                        preToken.tokenType == CTokenType::Multiply ||
                        preToken.tokenType == CTokenType::Divide ||
                        preToken.tokenType == CTokenType::LeftPar ||
                        preToken.tokenType == CTokenType::Greater ||
                        preToken.tokenType == CTokenType::GreaterOrEqual ||
                        preToken.tokenType == CTokenType::Less ||
                        preToken.tokenType == CTokenType::LessOrEqual ||
                        preToken.tokenType == CTokenType::Modulo ||
                        preToken.tokenType == CTokenType::AndBit ||
                        preToken.tokenType == CTokenType::OrBit)
                    {
                        strToken += ch;
                        lexState =  IntegerOrFloat;
                        break;
                    }
                }
            }

            Tokens.push_back( Token( CTokenType::Minus, "-" ));
            if (iSourceChar>0) --iSourceChar;
            lexState =  Space;
            
            break;
        case  MultiplyOrAssignMultiply:
            if (ch == '=')
            {
                Tokens.push_back( Token( CTokenType::AssignMultiply, "*=" ));
                lexState =  Space;
            }
            else
            {
                Tokens.push_back( Token( CTokenType::Multiply, "*" ));
                if (iSourceChar>0) --iSourceChar;
                lexState =  Space;
            }
            break;
        case  PowerOrAssignPower:
            if (ch == '=')
            {
                Tokens.push_back( Token( CTokenType::AssignPower, "^=" ));
                lexState =  Space;
            }
            else
            {
                Tokens.push_back( Token( CTokenType::Power, "^" ));
                if (iSourceChar>0) --iSourceChar;
                lexState =  Space;
            }
            break;
        case  ModuloOrAssignModulo:
            if (ch == '=')
            {
                Tokens.push_back( Token( CTokenType::AssignModulo, "%=" ));
                lexState =  Space;
            }
            else
            {
                Tokens.push_back( Token( CTokenType::Modulo, "%" ));
                if (iSourceChar>0) --iSourceChar;
                lexState =  Space;
            }
            break;
        case  And:
            if (ch == '&')
            {
                Tokens.push_back(Token( CTokenType::EnTokenType::And, "&&" ));
                lexState =  Space;
            }
            else
            {                
                Tokens.push_back( Token( CTokenType::AndBit, "&" ));
                if (iSourceChar>0) --iSourceChar;
                lexState =  Space;
            }
            break;
        case  Or:
            if (ch == '|')
            {
                Tokens.push_back( Token( CTokenType::EnTokenType::Or, "||" ));
                lexState =  Space;
            }
            else
            {
                Tokens.push_back( Token( CTokenType::OrBit, "|" ));
                if (iSourceChar>0) --iSourceChar;
                lexState =  Space;
            }
            break;
        case  NotOrNotEqual:
            if (ch == '=')
            {
                Tokens.push_back( Token( CTokenType::NotEqual, "!=" ));
                lexState =  Space;
            }
            else
            {
                Tokens.push_back( Token( CTokenType::Not, "!" ));
                if (iSourceChar>0) --iSourceChar;
                lexState =  Space;
            }
            break;
        case  GreaterOrGreaterEqual:
            if (ch == '=')
            {
                Tokens.push_back( Token( CTokenType::GreaterOrEqual, ">="));
                lexState =  Space;
            }
            else
            {
                Tokens.push_back( Token( CTokenType::Greater, ">" ));
                if (iSourceChar>0) --iSourceChar;
                lexState =  Space;
            }
            break;
        case  LessOrLessEqual:
            if (ch == '=')
            {
                Tokens.push_back( Token( CTokenType::LessOrEqual, "<=" ));
                lexState =  Space;
            }
            else
            {
                Tokens.push_back( Token( CTokenType::Less, "<" ));
                if (iSourceChar>0) --iSourceChar;
                lexState =  Space;
            }
            break;
        case  IdentifierOrKeyword:
            if (ch == '_' || ScriptLexer::IsLetterOrDigit(ch))
                strToken += ch;
            else
            {
                CTokenType::EnTokenType tokenType = CTokenType::TokenStringToType(strToken);
                if (CTokenType::NONE != tokenType)
                {   

                }   
                else if (strToken == "true" || strToken == "False" || strToken == "True"  ||  strToken == "false" ) 
                {
                    tokenType =  CTokenType::Boolean;
                }
                else if(isVariable(strToken))
                {
                    tokenType =  CTokenType::Variable;
                }
                else  if(isFunction(strToken))
                {
                    tokenType =  CTokenType::Function;
                }
                else
                {
                    tokenType =  CTokenType::Identifier;
                }

                Tokens.push_back( Token(tokenType, strToken ));
 
                if (iSourceChar>0) --iSourceChar;
                lexState =  Space;
            }
            break;
        case  ArrayLeftBracket:
            if (ch == '_' || ScriptLexer::IsLetterOrDigit(ch) || ch == ',' || ch == '\"' || ch =='.')
                strToken += ch;
            else if (ch == ']')
            {
                Tokens.push_back( Token(CTokenType::EnTokenType::Array, strToken ));
                lexState =  Space;
            }
            else if (ch == '\\')
                lexState =  StringEscape;
            else if (ch == '\r' || ch == '\n')
                {strErr =  "String literal cannot span multiple lines.";return false;}
            else
                strErr =  "Array Lexer Invalid character";
            break;
        case  String:
            if (ch == '\"')
            {
                //strToken += ch;
                Tokens.push_back( Token( CTokenType::EnTokenType::String, strToken ));
                lexState =  Space;
            }
            else if (ch == '\\')
                lexState =  StringEscape;
            else if (ch == '\r' || ch == '\n')
                {strErr = "String literal cannot span multiple lines."; return false;}
            else
                strToken += ch;
            break;
        case  StringEscape:
            if (ch == '\\' || ch == '\"')
            {
                strToken += ch;
                lexState =  String;
            }
            else if (ch == 't')
            {
                strToken += '\t';
                lexState =  String;
            }
            else if (ch == 'r')
            {
                strToken += '\r';
                lexState =  String;
            }
            else if (ch == 'n')
            {
                strToken += '\n';
                lexState =  String;
            }
            else
            {
                strErr ="Invalid string escape sequence ";
                return false;
            }
            break;
        case  IntegerOrFloat:
            if (IsDigit(ch))
                strToken += ch;
            else if (ch == '.')
            {
                strToken += ch;
                lexState =  Float;
            }
            else
            {
                int iValue = atoi(strToken.c_str());
                Tokens.push_back( Token( CTokenType::Integer, strToken ));
                if (iSourceChar>0) --iSourceChar;
                lexState =  Space;
            }
            break;
        case  Float:
            if (IsDigit(ch))
                strToken += ch;
            else
            {
                float fValue = atof(strToken.c_str());
                Tokens.push_back(Token( CTokenType::EnTokenType::Float, strToken ));
                if (iSourceChar>0) --iSourceChar;
                lexState =  Space;
            }
            break;
        default:
            {
                strErr = "Unhandled lexer state.";
                return false;
            }
        }
    }

    if (lexState !=  Space)
    {
        strErr = "Unexpected end of source reached.";
        return false;
    }

    return true;
}


bool ScriptLexer::IsDigit(const char& ch)
{
    return (ch >= '0') && (ch <= '9');
}

bool ScriptLexer::IsLetter(const char& ch)
{
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A'&& ch <= 'Z');
}

bool ScriptLexer::IsLetterOrDigit(const char& ch)
{
    return (ch >= '0' && ch <= '9')
        || (ch >= 'a' && ch <= 'z')
        || (ch >= 'A'&& ch <= 'Z');
}

bool ScriptLexer::isOperator( char Optch )
{
    char Opt[] = {'{', '}','(', ')', '[', ']', '.', ',',';',':', '=','+', '-','*', '/','^','%','&','|','!','>','<'};

    for ( int index = 0 ;index < sizeof(Opt)/sizeof(Opt[0]);index++)
    {
        if (Opt[index] == Optch)
        {
            return true;
        }
    }
    return false;
}


bool ScriptLexer::isVariable( const string& strOpt )
{
    return CFeatures::getInstance()->isBaseFeatureExist(strOpt) 
        || CFeatures::getInstance()->isDerivedFeatureExist(strOpt);
}

bool ScriptLexer::isOperatorString( const string& strOpt )
{
    const char* Opt[] = {"+","-","*","/","|","&","=","!=",">",">=","<","<=","%","!"};

    for ( int index = 0 ;index < sizeof(Opt)/sizeof(Opt[0]);index++)
    {
        if (strOpt.compare(Opt[index]) == 0)
        {
            return true;
        }
    }
    return false;
}

bool ScriptLexer::isFunction( const string& strOpt )
{
    const char* Opt[] = {"abs","acos","acosh","asin","asinh","atan","atan2","atanh",
                         "cbrt","ceil","cos","cosh","cot","csc","exp","floor","int","log","log2",
                         "log10","max","min","pow","sec","sin","sinh","sqrt","tan","tanh","trunc"};

    for ( int index = 0 ;index < sizeof(Opt)/sizeof(Opt[0]);index++)
    {
        if (strOpt.compare(Opt[index]) == 0)
        {
            return true;
        }
    }
    return false;
}

const string& ScriptLexer::getErrString()
{
    return strErrString;
}


void ScriptLexer::init()
{
    while(!m_expStack.empty())
    {
        m_expStack.pop();
    }
    while(!m_opStack.empty())
    {
        m_opStack.pop();
    }
    for (map<string,MathFunction*>::iterator it = m_Funsmap.begin(); it != m_Funsmap.end(); ++it)
    {
        if (( *it ).second != NULL)
        {
            delete( ( *it ).second );
        }     
    }
    m_Funsmap.clear();
    funLable = 0;
    if(!m_postfix.empty())
        m_postfix.clear();
    
    m_opStack.push(Token(CTokenType::Sharp,"#"));
}


int ScriptLexer::getLevel(const string &oper)
{
    //运算符由低到高：
    if (oper == "#" || oper == "("){return 0;}   
    else if (oper == "|" || oper == "&"){return 1;}        
    else if (oper == "=" || oper == "!="){return 2;}      
    else if (oper == ">" || oper == ">=" || oper == "<" || oper == "<="){return 3;}      
    else if (oper == "+" || oper == "-"){return 4;}        
    else if (oper == "*" || oper == "/" || oper == "%"){return 5;}     
    else if (oper == "^" ){return 6;}  
    else if (oper == "!"){return 7;}       
    else {return 0;};
}

int ScriptLexer::toPostfix()
{
    Token pre,Now;
    int ParCnt = 0;
    for(int i = 0; i < m_exp.size(); i++)
    {
        Now = m_exp[i];
        if (!islegel(pre,Now))
        {
            return OPERATORERROR;
        }
        //是数字
        if(m_exp[i].tokenType == CTokenType::Integer || m_exp[i].tokenType == CTokenType::Float)
        {
            m_postfix.push_back(m_exp[i]);
        }
        else if(isFunction(m_exp[i].strSourceToken))  // 函数
        {
            string strFunName = m_exp[i].strSourceToken;
            vector<Token> funarg;
            int j = i+1;
            if (j >= m_exp.size() || m_exp[j].tokenType !=  CTokenType::LeftPar)
            {
                return OPERATORERROR;
            }
            j = i+2;
            while(j < m_exp.size() && m_exp[j].tokenType != CTokenType::RightPar )
            {
                if (isOptNumber(m_exp[j]))
                {
                    funarg.push_back(m_exp[j]);
                }           
                j++;
            }

            ostringstream convert;
            convert << funLable;
            funLable++;
            string lable = strFunName + convert.str();
            MathFunction* pFun = new MathFunction(strFunName,funarg);
            if (!pFun->parser())
            {
                delete pFun;
                return OPERATORERROR;
            }
            m_Funsmap.insert(pair<string,MathFunction*>(lable,pFun));
            pre = m_exp[j];
            i = j;
            m_postfix.push_back(Token(CTokenType::Function,lable));
            continue;
        }
        else if (isVariable(m_exp[i].strSourceToken))  // 变量
        {
            m_postfix.push_back(m_exp[i]);
        }
        else if(m_exp[i].tokenType == CTokenType::LeftPar)
        {
            ParCnt++;
            m_opStack.push(m_exp[i]);
        }
        else if(m_exp[i].tokenType == CTokenType::RightPar)
        {
            ParCnt--;
            while(m_opStack.top().tokenType != CTokenType::LeftPar)
            {
                m_postfix.push_back(m_opStack.top());
                m_opStack.pop();
            }
            m_opStack.pop();
        }
        else if (isOperatorString(m_exp[i].strSourceToken))
        {
            if (m_opStack.empty())
            {
                m_opStack.push(m_exp[i]);
            }
            else if (m_opStack.top().tokenType == CTokenType::LeftPar)
            {
                 m_opStack.push(m_exp[i]);   
            }
            else if(getLevel(m_exp[i].strSourceToken) > getLevel(m_opStack.top().strSourceToken))
            {
                m_opStack.push(m_exp[i]);
            }
            else if(getLevel(m_exp[i].strSourceToken) <= getLevel(m_opStack.top().strSourceToken))
            {
                m_postfix.push_back(m_opStack.top());
                m_opStack.pop();
                pre = m_exp[i-1];
                i--;
                continue;
            }
        }
        else
        {
            strErrString = "toPostfix Error is  :  " + Now.strSourceToken; 
            return DOESNOTMATCHTHEPARENTHESES;
        } 
        pre = Now;
    }
    while(m_opStack.top().strSourceToken != "#")
    {
        m_postfix.push_back(m_opStack.top());
        m_opStack.pop();
    }
    if (ParCnt != 0)
    {
        return OPERATORERROR;
    }
    return NOERROR;
}

int ScriptLexer::evaluation(double &results)
{
    QString tem;
    stack<double> ans;

    for(int i = 0; i < m_postfix.size(); i++)
    {
        Token tokentmp = m_postfix[i];
        if(m_postfix[i].tokenType == CTokenType::Integer || m_postfix[i].tokenType == CTokenType::Float)
        {
            ans.push(strtod(m_postfix[i].strSourceToken.c_str(),NULL));
        }
        else if ( isVariable(m_postfix[i].strSourceToken))
        {
            double dvaule;
            if (getVarValue(m_postfix[i].strSourceToken,dvaule))
            {
                ans.push(dvaule);
            }
            else
            {
                return OPERATORERROR;
            }
        }
        else if (m_postfix[i].tokenType == CTokenType::Function)
        {
            double dvaule;
            if (getFunValue(m_postfix[i].strSourceToken,dvaule))
            {
                ans.push(dvaule);
            }
            else
            {
                return OPERATORERROR;
            }
            
        }
        else if(isOperatorString(m_postfix[i].strSourceToken) && !ans.empty())
        {
            double a, b;
            if (!ans.empty())
            {
                a = ans.top();
                ans.pop();
            }
            
            if (m_postfix[i].strSourceToken != "!")
            {
                if (!ans.empty())
                {
                    b = ans.top();
                    ans.pop();
                }
                //qDebug() << "b:" << b ;
            }
            if (m_postfix[i].strSourceToken  == "+"){
                ans.push(b + a);}
            else if (m_postfix[i].strSourceToken  == "-"){
                ans.push(b - a);}
            else if (m_postfix[i].strSourceToken  == "*"){
                ans.push(b * a);}
            else if (m_postfix[i].strSourceToken  == "/"){
                if (a == 0)
                    return DIVIDENDISZERO;
                else
                    ans.push(b / a);}
            else if (m_postfix[i].strSourceToken  == "%") {
                ans.push((int)b % (int)a);}
            else if (m_postfix[i].strSourceToken  == "!") {
                ans.push(!a);}
            else if (m_postfix[i].strSourceToken  == "|") {
                ans.push(a || b);}
            else if (m_postfix[i].strSourceToken  == "&") {
                ans.push(a && b);}
            if (m_postfix[i].strSourceToken  == "=") {
                ans.push(a == b);}
            else if (m_postfix[i].strSourceToken  == "!=") {
                ans.push(a != b);}
            else if (m_postfix[i].strSourceToken  == ">") {
                ans.push(b > a);}
            else if (m_postfix[i].strSourceToken  == ">="){
                ans.push(b >= a);}
            else if (m_postfix[i].strSourceToken  == "<") {
                ans.push(b < a);}
            else if (m_postfix[i].strSourceToken  == "<=") {
                ans.push(b <= a);}
            else if (m_postfix[i].strSourceToken  == "||") {
                ans.push(b || a);}
            else if (m_postfix[i].strSourceToken  == "&&") {
                ans.push(b && a);}
            else if (m_postfix[i].strSourceToken  == "^") {
                ans.push(pow(b,a));}
            else {}            
        }
        else
        {
            strErrString = "evaluation Error is  :  " + tokentmp.strSourceToken; 
            return DOESNOTMATCHTHEPARENTHESES;
        }
        results = ans.top();
    } 
    return NOERROR;
}

//返回值为错误提示(枚举中)，参数results保存计算的最终结果
int ScriptLexer::parser()
{
    init();
    int iResult = toPostfix();
    return iResult;
}

bool ScriptLexer::islegel(const Token& pre,const Token& now)
{
    if (isOperatorString(pre.strSourceToken) && isOperatorString(now.strSourceToken))
    {
        return false;
    }
    else if (isOptNumber(pre) && isOptNumber(now))
    {
        return false;
    }
    return true;
}

bool ScriptLexer::isOptNumber(const Token& pre )
{
    if (isVariable(pre.strSourceToken) || 
        pre.tokenType == CTokenType::Integer  ||
        pre.tokenType == CTokenType::Float)
    {
        return true;
    }
    return false;
}

bool ScriptLexer::getVarValue( const string& strVarName,double& dValue )
{
    if (CFeatures::getInstance()->isBaseFeatureExist(strVarName))
    {
        dValue = SString::strtoDouble(CFeatures::getInstance()->getBaseFeature(strVarName).strFeaturesDefaultValue);
    }
    else if (CFeatures::getInstance()->isDerivedFeatureExist(strVarName))
    {
        dValue = SString::strtoDouble(CFeatures::getInstance()->getDerivedFeature(strVarName).strFeaturesDefaultValue);
    }
    else
    {
        return false;
    }
    return true;
}

bool ScriptLexer::getFunValue( const string& strFunName,double& dValue )
{
    map<string,MathFunction*>::iterator it = m_Funsmap.find(strFunName);
    if (it == m_Funsmap.end())
    {
        return false;
    }
    if (it->second == NULL)
    {
        return false;
    }
    if (!it->second->evaluation(dValue))
    {
        return false;
    }
    return true;
}
