#ifndef FORM_H
#define FORM_H

#include <QWidget>

namespace Ui {
class Form;
}

struct stImageInfo;
class IArithmeticStatic;
class IArithmeticStaticMgr;
class IArithmeticDynamicMag;
class IArithmeticDynamic;
class IArithmeticSignalFilterMgr;
class IArithmeticSignalFilter;
class IFileVariable;
class ILineCamera;

class Form : public QWidget
{
    Q_OBJECT

public:
    explicit Form(QWidget *parent = 0);
    ~Form();
private:
    void init();
public:
    void openWindows(int width, int height);
    private slots :
        void on_pushButton_get_online_pic_clicked(bool checked);
private:
    stImageInfo   *m_imageInfoBuffer;
    IFileVariable                *m_pCfgFile;
    ILineCamera                  *m_pCurrentCamera;
    Ui::Form *ui;
};

#endif // FORM_H
