#include "computationalexpressions.h"
#include <QDebug>

ComputationalExpressions* ComputationalExpressions::m_pInstance = NULL;



ComputationalExpressions* ComputationalExpressions::getInstance()
{
    if (m_pInstance == NULL)
    {
        m_pInstance = new ComputationalExpressions();
    }
    return m_pInstance;
}

void ComputationalExpressions::releaseInstance()
{
    if (m_pInstance != NULL)
    {
        delete m_pInstance ;
        m_pInstance = NULL;
    }
    return ;
}


ComputationalExpressions::ComputationalExpressions(QObject *parent) :
    QObject(parent)
{
    init();
}

void ComputationalExpressions::init()
{
    if(!m_expStack.isEmpty())
        m_expStack.clear();
    if(!m_opStack.isEmpty())
        m_opStack.clear();
    if(!m_postfix.isEmpty())
        m_postfix.clear();

    m_opStack.push("#");
    m_lefts = 0;
}

int ComputationalExpressions::getLevel(const QString &oper)
{
    //运算符由低到高：
    if (oper == "#" || oper == "(")
        return 0;
    if (oper == "|" || oper == "&")
        return 1;
    if (oper == "=" || oper == "!=")
        return 2;
    if (oper == ">" || oper == ">=" || oper == "<" || oper == "<=")
        return 3;
    if (oper == "+" || oper == "-")
        return 4;
    if (oper == "*" || oper == "/" || oper == "%")
        return 5;
    if (oper == "!")
        return 6;

    return 0;
}

int ComputationalExpressions::toPostfix()
{
    //对中缀表达式处理
    for (int i = 0; i < m_exp.size(); ++i)
    {
        if (m_exp[i] == '(')
            m_lefts++;
        if (m_exp[i] == ')')
            m_lefts--;

        if ( (m_exp[i] == '-') && (m_exp[i + 1] == '(')
             && ( (i == 0)
                  || ((i > 0) && !m_exp[i - 1].isDigit() && (m_exp[i - 1] != ')'))  )   )
        {
            m_exp.replace(i, 1, "-1*");
        }
        else if (isOperator(m_exp[i]) && isOperator(m_exp[i + 1]))
        {
            if (m_exp.mid(i, 2) == "--")
            {
                if (i == 0)
                    m_exp.remove(0, 2);
                else
                    m_exp.replace(i, 2, "+");
            }
            else if (m_exp.mid(i, 2) == "!!")
            {
                m_exp.remove(i, 2);
                continue;
            }
            //运算符错误，有2个连续的错误运算符
            else if (!m_exp.mid(i, 2).contains("!") )
            {
                return En_OPERATORERROR;
            }
            //三个连续的运算符肯定错误
            else if (isOperator(m_exp[i + 2]))
                return En_OPERATORERROR;
        }
        else if (i > 0 && m_exp[i] == '(' && m_exp[i - 1].isDigit())
        {
            m_exp.replace(i, 1, "*(");
            continue;
        }
        else if (i > 0 && m_exp[i] == ')' && m_exp[i + 1].isDigit())
        {
            m_exp.replace(i, 1, ")*");
            continue;
        }
    }

    //括号不匹配
    if (m_lefts != 0)
    {
        return En_DOESNOTMATCHTHEPARENTHESES;
    }

    qDebug() << QString::fromLocal8Bit("中缀表达式为：" )<< m_exp;

    bool isNegative = false;
    for(int i = 0; i < m_exp.length(); i++)
    {
        if (isNegative && isOperator(m_exp[i]))
        {
            isNegative = false;
        }
        //是数字
        if(m_exp[i].isDigit() || m_exp[i] == '.')
        {
            if ((m_postfix.right(1) != " ") &&
                (!m_postfix.right(1)[0].isDigit()) &&
                (!isNegative) &&
                (m_postfix.right(1) != "."))
            {
                if (i > 0)
                {
                    qDebug() << "数字前加空格";
                    m_postfix.push_back(' ');
                }
            }

            m_postfix.push_back(m_exp[i]);
            qDebug() << m_postfix;
        }
        else if(m_exp[i] == '(')
        {
            m_opStack.push(m_exp.mid(i, 1));
        }
        else if(m_exp[i] == ')')
        {
            while(m_opStack.top() != "(")
            {
                m_postfix.push_back(' ');
                m_postfix.push_back(m_opStack.pop());
            }
            m_opStack.pop();
        }
        else if ( (m_exp[i] == '-') &&
                  ( ((i > 0) && !m_exp[i - 1].isDigit() && (m_exp[i - 1] != ')')) || (i == 0) ) &&
                  (m_exp[i + 1].isDigit()) )
            //是负号
        {
            qDebug() << "是负号";
            isNegative = true;
            if (i != 0)
                m_postfix.push_back(' ');
            m_postfix.push_back(m_exp[i]);
            qDebug() << m_postfix;
            continue;
        }
        else if(getLevel(m_exp.mid(i, 1)) > getLevel(m_opStack.top()))
        {
            if (i != 0)
                m_postfix.push_back(' ');
            m_opStack.push(m_exp.mid(i, 1));
            qDebug() << m_postfix;
        }
        else
        {
            while(getLevel(m_exp.mid(i, 1)) <= getLevel(m_opStack.top()))
            {
                if (i != 0)
                    m_postfix.push_back(' ');
                m_postfix.push_back(m_opStack.pop());
            }
            m_opStack.push(m_exp.mid(i, 1));
            qDebug() << m_postfix;
        }
    }

    while(m_opStack.top() != "#")
    {
        m_postfix.push_back(' ');
        m_postfix.push_back(m_opStack.pop());
    }

    //去除前后的所有空格，将中间的多余空格全部转换为一个空格
    m_postfix = m_postfix.simplified();
    qDebug() << "后缀表达式为：" << m_postfix;
    return En_NOERROR;
}

int ComputationalExpressions::evaluation(double &results)
{
    QString tem;
    QStack<double> ans;

    for(int i = 0; i < m_postfix.size(); i++)
    {
        qDebug() << m_postfix[i] << i;
        if((m_postfix[i] <= '9' && m_postfix[i] >= '0') || m_postfix[i] == '.')
            tem.push_back(m_postfix[i]);

        else if ( (m_postfix[i] == '-')
                    && ( ((i > 0) && (m_postfix[i - 1] == ' ') && (m_postfix[i + 1].isDigit()))
                         || (m_postfix[i + 1].isDigit() && (i == 0)) ) )
        {
            //是负号
            tem.push_back(m_postfix[i]);
            continue;
        }

        else if(m_postfix[i] == ' ')
        {
            if(!tem.isEmpty())
            {
                ans.push(tem.toDouble());
                tem.clear();
            }
            qDebug() << ans.top() << tem.isEmpty();
        }
        else
        {
            double a, b;
            a = ans.pop();
            qDebug() << "a:" << a ;

            if (m_postfix[i] != '!')
            {
                b = ans.pop();
                qDebug() << "b:" << b ;
            }

            if (m_postfix.mid(i, 1) == "+")
                ans.push(b + a);
            if (m_postfix.mid(i, 1) == "-")
                ans.push(b - a);
            if (m_postfix.mid(i, 1) == "*")
                ans.push(b * a);
            if (m_postfix.mid(i, 1) == "/")
            {
                if (a == 0)
                    return En_DIVIDENDISZERO;
                else
                    ans.push(b / a);
            }
            if (m_postfix.mid(i, 1) == "%")
                ans.push((int)b % (int)a);
            if (m_postfix.mid(i, 1) == "!")
                ans.push(!a);
            if (m_postfix.mid(i, 1) == "|")
                ans.push(a || b);
            if (m_postfix.mid(i, 1) == "&")
                ans.push(a && b);
            if (m_postfix.mid(i, 1) == "=")
                ans.push(a == b);
            if (m_postfix.mid(i, 2) == "!=")
                ans.push(a != b);
            if (m_postfix.mid(i, 1) == ">")
                ans.push(b > a);
            if (m_postfix.mid(i, 2) == ">=")
                ans.push(b >= a);
            if (m_postfix.mid(i, 1) == "<")
                ans.push(b < a);
            if (m_postfix.mid(i, 2) == "<=")
                ans.push(b <= a);

            qDebug() << "result:" << ans.top();
            results = ans.top();
        }
    }

    return En_NOERROR;
}

//返回值为错误提示(枚举中)，参数results保存计算的最终结果
int ComputationalExpressions::parser(const QString &exp)
{
    init();
    m_exp = exp;
    int iResult = toPostfix();
    return iResult;
}

bool ComputationalExpressions::isOperator(const QCharRef &oper)
{
    if (!oper.isDigit() && oper != '(' && oper != ')')
        return true;
    else
        return false;
}
