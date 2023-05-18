#ifndef _TOKEN_TYPE_H_
#define _TOKEN_TYPE_H_
#include <string>
using namespace  std;

class CTokenType
{
public:
    enum EnTokenType
    {
        NONE = 0,
        Variable,        ///    "Variable"               variable declaration.
        Function,        ///    "Function"          Function declaration keyword.
        Identifier,      ///    "Identifier"        Variable or function identifier.
        Boolean,         ///    "Boolean"           Boolean literal.
        Integer,         ///    "Integer"           Integer literal.
        Float,           ///    "Float"             Floating  literal.
        String,          ///    "String"            String literal.
        Array,           ///    "Array"             Array literal.
        Null,            ///    "Null"              Null reference keyword.
        LeftBrace,       ///    "{"                 Opening brace.
        RightBrace,      ///    "}"                 Closing brace.
        LeftPar,         ///    "("                 Opening parenthesis.    
        RightPar,        ///    ")"                 Closing parenthesis.
        LeftBracket,     ///    "["                 Opening bracket. 
        RightBracket,    ///    "]"                 Closing bracket.
        Period,          ///    "."                 Membership operator.
        Comma,           ///    ","                 Argument delimeter.
        SemiColon,       ///    ";"                 Statement delimeter.
        Increment,       ///    "++"                Pre or post increment operator (++).
        Decrement,       ///    "--"                Pre or post decrement operator (--).
        Plus,            ///    "+"                 Numeric addition / string concatentation 
        Minus,           ///    "-"                 Numeric substraction / array operator.(-)
        Multiply,        ///    "*"                 Multiplication operator. (*)
        Divide,          ///    "/"                 Division operator.        (/) 
        Power,           ///    "^"                 Exponent operator. 
        Modulo,          ///    "%"                 Modulo operator.
        AndBit,          ///    "&"                 And  operator   
        OrBit,           ///    "|"                 Or   operator
        Assign,          ///    "="                 Assignment operator.
        AssignPlus,      ///    "+="                Additive assignment operator.
        AssignMinus,     ///    "-="                Subtractive assignment operator.
        AssignMultiply,  ///    "*="                Multiplicative assignment operator.
        AssignDivide,    ///    "/="                Divisive assignment operator.
        AssignPower,     ///    "^="                Exponential assignment operator.
        AssignModulo,    ///    "%="                Modulo assignment operator.
        And,             ///    "&&"                Boolean conjunction operator.
        Or,              ///    "||"                Boolean disjunction operator. 
        Not,             ///    "!"                 Boolean negation operator.
        Equal,           ///    "=="                Equality operator.
        NotEqual,        ///    "!="                Inequality operator. 
        Greater,         ///    ">"                 Inequality operator. 
        GreaterOrEqual,  ///    ">="                Inequality operator.
        Less,            ///    "<"                 Inequality operator.
        LessOrEqual,     ///    "<="                Inequality operator.
        Colon,           ///    ":"                 Conditional expression keyword
        Sharp,          ///     "#"                 Sharp
        LineComment,      ///    "//"                Line Comment     
    };

    static string TokenTypeToString(CTokenType::EnTokenType Ttype)
    {
        switch(Ttype)
        {
            case NONE:              return   "NONE";
            case Variable:          return   "Variable";
            case Function:          return   "Function";
            case Identifier:        return   "Identifier";
            case Boolean:           return   "Boolean";
            case Null:              return   "Null";
            case LeftBrace:         return   "{";
            case RightBrace:        return   "}";
            case LeftPar:           return   "(";
            case RightPar:          return   ")";
            case LeftBracket:       return   "[";
            case RightBracket:      return   "]";
            case Period:            return   ".";
            case Comma:             return   ",";
            case SemiColon:         return   ":";
            case Increment:         return   "++";
            case Decrement:         return   "--";
            case Plus:              return   "+";
            case Minus:             return   "-";
            case Multiply:          return   "*";
            case Divide:            return   "/";
            case Power:             return   "^";
            case Modulo:            return   "%";
            case AndBit:            return   "";
            case OrBit:             return   "|"  ;              
            case Assign:            return   "="  ;              
            case AssignPlus:        return   "+=" ;              
            case AssignMinus:       return   "-=" ;              
            case AssignMultiply:    return   "*=" ;              
            case AssignDivide:      return   "/=" ;              
            case AssignPower:       return   "^=" ;              
            case AssignModulo:      return   "%=" ;              
            case And:               return   "&&" ;              
            case Or:                return   "||" ;              
            case Not:               return   "!"  ;              
            case Equal:             return   "==" ;              
            case NotEqual:          return   "!=" ;              
            case Greater:           return   ">"  ;              
            case GreaterOrEqual:    return   ">=" ;              
            case Less:              return   "<"  ;              
            case LessOrEqual:       return   "<=" ;              
            case Colon:             return   ":"  ;              
            case LineComment:       return   "//" ;              
            default:                return   "Unknow type";
        }//end switch;
    }


    static CTokenType::EnTokenType  TokenStringToType(const string Tokentypestring)
    {

      if(Tokentypestring ==  "NONE")                 return NONE;              
      else if(Tokentypestring ==  "Variable")        return Variable;
      else if(Tokentypestring ==  "Function")        return Function;
      else if(Tokentypestring ==  "Identifier")      return Identifier;
      else if(Tokentypestring ==  "Boolean")         return Boolean;
      else if(Tokentypestring ==  "Null")            return Null;              
      else if(Tokentypestring ==  "{")               return LeftBrace;         
      else if(Tokentypestring ==  "}")               return RightBrace;        
      else if(Tokentypestring ==  "(")               return LeftPar;           
      else if(Tokentypestring ==  ")")               return RightPar;          
      else if(Tokentypestring ==  "[")               return LeftBracket;       
      else if(Tokentypestring ==  "]")               return RightBracket;      
      else if(Tokentypestring ==  ".")               return Period;            
      else if(Tokentypestring ==  ",")               return Comma;             
      else if(Tokentypestring ==  ":")               return SemiColon;         
      else if(Tokentypestring ==  "++")              return Increment;         
      else if(Tokentypestring ==  "--")              return Decrement;         
      else if(Tokentypestring ==  "+")               return Plus;              
      else if(Tokentypestring ==  "-")               return Minus;             
      else if(Tokentypestring ==  "*")               return Multiply;          
      else if(Tokentypestring ==  "/")               return Divide;            
      else if(Tokentypestring ==  "^")               return Power;             
      else if(Tokentypestring ==  "%")               return Modulo;            
      else if(Tokentypestring ==  "")                return AndBit;            
      else if(Tokentypestring ==  "|"  )             return OrBit;                   
      else if(Tokentypestring ==  "="  )             return Assign;                  
      else if(Tokentypestring ==  "+=" )             return AssignPlus;              
      else if(Tokentypestring ==  "-=" )             return AssignMinus;             
      else if(Tokentypestring ==  "*=" )             return AssignMultiply;          
      else if(Tokentypestring ==  "/=" )             return AssignDivide;            
      else if(Tokentypestring ==  "^=" )             return AssignPower;             
      else if(Tokentypestring ==  "%=" )             return AssignModulo;            
      else if(Tokentypestring ==  "&&" )             return And;                     
      else if(Tokentypestring ==  "||" )             return Or;                      
      else if(Tokentypestring ==  "!"  )             return Not;                     
      else if(Tokentypestring ==  "==" )             return Equal;                   
      else if(Tokentypestring ==  "!=" )             return NotEqual;                
      else if(Tokentypestring ==  ">"  )             return Greater;                 
      else if(Tokentypestring ==  ">=" )             return GreaterOrEqual;          
      else if(Tokentypestring ==  "<"  )             return Less;                    
      else if(Tokentypestring ==  "<=" )             return LessOrEqual;             
      else if(Tokentypestring ==  ":"  )             return Colon;                   
      else if(Tokentypestring ==  "//" )             return LineComment;             
      else                                           return NONE;                
    }
};

typedef struct Token
{
public:
    CTokenType::EnTokenType     tokenType;
    string strSourceToken;
    Token()
    {
        tokenType = CTokenType::EnTokenType ::NONE;
    }

    Token(CTokenType::EnTokenType etokenType,string strSource)
    {
        tokenType = etokenType;
        strSourceToken = strSource;
    }


    Token& operator=(const Token& other)
    {
        if (&other == this)
        {
            return *this;
        }
        tokenType = other.tokenType;
        strSourceToken = other.strSourceToken;
        return *this;
    }
}stToken;

#endif
