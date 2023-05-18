#ifndef DEFECTEXPRESSION_H
#define DEFECTEXPRESSION_H

#include <QWidget>

namespace Ui {
class DefectExpression;
}

class DefectExpression : public QWidget
{
    Q_OBJECT

public:
    explicit DefectExpression(QWidget *parent = 0);
    ~DefectExpression();
private:
    void initUI();
private slots:
    void on_pushButton_Expression_clicked();

private:
    Ui::DefectExpression *ui;

};

#endif // DEFECTEXPRESSION_H
