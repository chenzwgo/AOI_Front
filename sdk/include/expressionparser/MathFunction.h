#ifndef     _MATHFUNCTION_H_
#define     _MATHFUNCTION_H_

#include "tokentype.h"
#include <string>
#include <vector>

class MathFunction
{
public:
    MathFunction(string strFunType,const vector<Token>& args);
    ~MathFunction(void);
    bool parser();
    bool evaluation(double &results);  

private:
    vector<Token> m_argsToken;
    vector<double> m_args;
    string m_FunType;
};

#endif  // _MATHFUNCTION_H_
