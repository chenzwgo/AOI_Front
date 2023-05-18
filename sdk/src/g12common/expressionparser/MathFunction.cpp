#include "MathFunction.h"
#include "scriptLexer.h"
#include "stringcommon.h"

//double acosh (const double x);  
//double atanh (const double x);  
//double asinh (const double x);  
//double log1p (const double x);  
//double log2 (const double x); 

#define GSL_SQRT_DBL_EPSILON   1.4901161193847656e-08  
#define GSL_DBL_EPSILON        2.2204460492503131e-16  
#ifndef M_LN2  
#define M_LN2      0.69314718055994530941723212146      /* ln(2) */  
#endif 


MathFunction::MathFunction(string strFunType,const vector<Token>& args)
{
    m_FunType = strFunType;
    m_argsToken = args;
}


MathFunction::~MathFunction(void)
{
}

bool MathFunction::parser()
{

    if(m_FunType.compare("abs") == 0 || m_FunType.compare("acos") == 0 ||
        m_FunType.compare("acosh") == 0 || m_FunType.compare("asin") == 0 ||
        m_FunType.compare("asinh") == 0 || m_FunType.compare("atan") == 0 ||
        m_FunType.compare("atanh") == 0 || m_FunType.compare("sec") == 0 ||
        m_FunType.compare("cbrt") == 0 || m_FunType.compare("ceil") == 0 ||
        m_FunType.compare("cos") == 0 || m_FunType.compare("cosh") == 0 ||
        m_FunType.compare("cot") == 0 || m_FunType.compare("csc") == 0 ||
        m_FunType.compare("exp") == 0 || m_FunType.compare("floor") == 0 ||
        m_FunType.compare("int") == 0 || m_FunType.compare("log") == 0 ||
        m_FunType.compare("log2") == 0 || m_FunType.compare("log10") == 0 ||
        m_FunType.compare("sin") == 0 || m_FunType.compare("sinh") == 0 ||
        m_FunType.compare("sqrt") == 0 || m_FunType.compare("sqrt") == 0 ||
        m_FunType.compare("tan") == 0 || m_FunType.compare("tanh") == 0 ||
        m_FunType.compare("trunc") == 0)
    {
        if (m_argsToken.size() != 1 || !ScriptLexer::isOptNumber(m_argsToken[0])){return false;}
        m_args.clear();
        m_args.push_back(SString::strtoDouble(m_argsToken[0].strSourceToken));
    }   
    else if (m_FunType.compare("atan2") == 0 || m_FunType.compare("max") == 0 ||  
        m_FunType.compare("min") == 0 || m_FunType.compare("pow") == 0 )
    {
        if (m_argsToken.size() != 2|| !ScriptLexer::isOptNumber(m_argsToken[0]) || !ScriptLexer::isOptNumber(m_argsToken[1])){return false;}
        m_args.clear();
        m_args.push_back(SString::strtoDouble(m_argsToken[0].strSourceToken));
        m_args.push_back(SString::strtoDouble(m_argsToken[1].strSourceToken));
    }
    else {return false;}
    return true;

}

bool MathFunction::evaluation( double &results )
{

    if (m_args.size() ==0)
    {
        return false;
    }
    if(m_FunType.compare("abs") == 0){
        results = abs(m_args[0]);
    }
    else if (m_FunType.compare("acos") == 0){       // 计算 arg 的弧（反）余弦主值。
        results = std::acos(m_args[0]);
    }
    else if (m_FunType.compare("acosh") == 0){      // 计算 arg 的反双曲余弦。
        results = acosh(m_args[0]); 
    }
    else if (m_FunType.compare("asin") == 0){       // 计算 arg 的弧（反）正弦主值。
        results = std::asin(m_args[0]); 
    }
    else if (m_FunType.compare("asinh") == 0){      // 计算 arg 的反双曲正弦。
        results = asinh(m_args[0]); 
    }
    else if (m_FunType.compare("atan") == 0){      // 计算 arg 的弧（反）正切主值。
        results = std::atan(m_args[0]); 
    }
    else if (m_FunType.compare("atan2") == 0){     //计算 y/x 的弧（反）正切，以参数符号确定正确的象限。
        results = std::atan2(m_args[0],m_args[1]); 
    }
    else if (m_FunType.compare("atanh") == 0){    //计算 arg 的反正切。
        results = atanh(m_args[0]);
    }
    else if (m_FunType.compare("cbrt") == 0){   // 计算 arg 的立方根
        //results = std::cbrt(m_args[0]);  
        results = pow((double)m_args[0],double(1/3)); 
    }
    else if (m_FunType.compare("ceil") == 0){   // 计算不小于 arg 的最小整数值
        results = std::ceil(m_args[0]);  
    }
    else if (m_FunType.compare("cos") == 0){    //计算 arg （以弧度计量）的余弦
        results = cos(m_args[0]);           
    }
    else if (m_FunType.compare("cosh") == 0){   // 计算 arg 的双曲余弦。
        results = cosh(m_args[0]); 
    }
    else if (m_FunType.compare("cot") == 0){    //余切函数 
        results = 1 / std::tan(m_args[0]);
    }
    else if (m_FunType.compare("csc") == 0){
        results = 1 / std::sin(m_args[0]);
    }
    else if (m_FunType.compare("exp") == 0){    //计算 e （欧拉数， 2.7182818 ）的 arg 次幂
        results = std::exp(m_args[0]); 
    }
    else if (m_FunType.compare("floor") == 0){  //计算不大于 arg 的最大整数值。
        results = std::floor(m_args[0]); 
    }
    else if (m_FunType.compare("int") == 0){
        int b = 0; double val ;
        b = (int)m_args[0];
        val = m_args[0] - b;
        if (val - 0.5 >= 0.000001) {
            results = b+1;
        }
        else{
            results = b;
        }
    }
    else if (m_FunType.compare("log") == 0){    // 计算 arg 的自然（底 e ）对数
        results = std::log(m_args[0]); 
    }
    else if (m_FunType.compare("log2") == 0){   // 计算 arg 的底 2 对数
        results = log2(m_args[0]); 
    }
    else if (m_FunType.compare("log10") == 0){  // 计算 arg 的底 2 对数
        results = std::log10(m_args[0]); 
    }
    else if (m_FunType.compare("max") == 0){    // 返回 a 与 b 的较大者
        results = max(m_args[0],m_args[1]);
    }
    else if (m_FunType.compare("min") == 0){     // 返回 a 与 b 的较小者
        results = min(m_args[0],m_args[1]);
    }
    else if (m_FunType.compare("pow") == 0){    // 计算 base 的 exp 或 iexp 次幂。
        results = std::pow(m_args[0],m_args[1]);
    }
    else if (m_FunType.compare("sec") == 0){
        results = 1 / std::cos(m_args[0]);
    }
    else if (m_FunType.compare("sin") == 0){    // 计算 arg （以弧度度量）的正弦
        results = sin(m_args[0]);
    }
    else if (m_FunType.compare("sinh") == 0){   // 计算 arg 的双曲正弦
        results = std::sinh(m_args[0]);
    }
    else if (m_FunType.compare("sqrt") == 0){   // 计算 arg 的平方根。
        results = std::sqrt(m_args[0]);
    }
    else if (m_FunType.compare("tan") == 0){    // 计算 arg （以弧度度量）的正切。
        results = std::tan(m_args[0]);
    }
    else if (m_FunType.compare("tanh") == 0){   // 计算 arg 的双曲正切。
        results = std::tanh(m_args[0]);
    }
    else if (m_FunType.compare("trunc") == 0){  // 计算绝对值 不大于 arg 的最接近整数。
        //results = std::trunc(m_args[0]);
        int b = 0; double val ;
        b = (int)m_args[0];
        val = m_args[0] - b;
        results =  m_args[0] - val;
    }
    else {return false;}
    return true;
}

/***

inline double fdiv(double a, double b)  
{  
    return a / b;  
}  

inline double nan (void)  
{  
    return fdiv (0.0, 0.0);  
}  

inline double posinf (void)  
{  
    return fdiv (+1.0, 0.0);  
}  

inline double neginf (void)  
{  
    return fdiv (-1.0, 0.0);  
}  

double log1p (const double x)  
{  
    volatile double y, z;  
    y = 1 + x;  
    z = y - 1;  
    return log(y) - (z - x) / y ;  
}  


double log2 (const double x)
{
    return log (x) / log (2.0);  
}

double acosh (const double x)  
{  
    if (x > 1.0 / GSL_SQRT_DBL_EPSILON)  
    {  
        return log (x) + M_LN2;  
    }  
    else if (x > 2)  
    {  
        return log (2 * x - 1 / (sqrt (x * x - 1) + x));  
    }  
    else if (x > 1)  
    {  
        double t = x - 1;  
        return log1p (t + sqrt (2 * t + t * t));  
    }  
    else if (x == 1)  
    {  
        return 0;  
    }  
    else  
    {  
        return nan();  
    }  
}  

double asinh (const double x)  
{  
    double a = fabs (x);  
    double s = (x < 0) ? -1 : 1;  

    if (a > 1 / GSL_SQRT_DBL_EPSILON)  
    {  
        return s * (log (a) + M_LN2);  
    }  
    else if (a > 2)  
    {  
        return s * log (2 * a + 1 / (a + sqrt (a * a + 1)));  
    }  
    else if (a > GSL_SQRT_DBL_EPSILON)  
    {  
        double a2 = a * a;  
        return s * log1p (a + a2 / (1 + sqrt (1 + a2)));  
    }  
    else  
    {  
        return x;  
    }  
}  

double atanh (const double x)  
{  
    double a = fabs (x);  
    double s = (x < 0) ? -1 : 1;  

    if (a > 1)  
    {  
        return nan();  
    }  
    else if (a == 1)  
    {  
        return (x < 0) ? neginf() : posinf();  
    }  
    else if (a >= 0.5)  
    {  
        return s * 0.5 * log1p (2 * a / (1 - a));  
    }  
    else if (a > GSL_DBL_EPSILON)  
    {  
        return s * 0.5 * log1p (2 * a + 2 * a * a / (1 - a));  
    }  
    else  
    {  
        return x;  
    }  
}  
***/