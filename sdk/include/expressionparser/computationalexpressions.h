#ifndef COMPUTATIONALEXPRESSIONS_H
#define COMPUTATIONALEXPRESSIONS_H

#include <QObject>
#include <QStack>

enum En_ERROR
{
    En_NOERROR = 0,
    En_DIVIDENDISZERO = 1,
    En_OPERATORERROR = 2,
    En_DOESNOTMATCHTHEPARENTHESES = 3
};

class ComputationalExpressions : public QObject
{
    Q_OBJECT
public:
    explicit ComputationalExpressions(QObject *parent = 0);
    static ComputationalExpressions* getInstance();
    static void releaseInstance();


    int evaluation(double &results);                  //求值

    int parser(const QString &exp);

private:
    bool isOperator(const QCharRef &oper);
    void init();
    int getLevel(const QString &oper);      //得到运算符等级
    int toPostfix();                        //转换成后缀表达式


    QStack<QString> m_expStack;     //后缀表达式栈
    QStack<QString> m_opStack;      //运算符栈
    QString m_postfix;            //存储后缀表达式
    int m_lefts;                  //左括号个数
    QString m_exp;
    static ComputationalExpressions* m_pInstance;
};

#endif // COMPUTATIONALEXPRESSIONS_H
