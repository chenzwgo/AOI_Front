#ifndef DEFECTCLASSIFICATIONUI_H
#define DEFECTCLASSIFICATIONUI_H

#include <QDialog>
#include <QTranslator>
#include "OS_DEFINE.h"

namespace Ui {
class DefectClassificationUI;
}

class  ExtendStackWidget;

class DefectClassificationUI : public QDialog
{
    Q_OBJECT
    
public:
    explicit DefectClassificationUI(QWidget *parent = 0);
    ~DefectClassificationUI();
    bool init();
    bool deinit();

protected:
    void keyPressEvent(QKeyEvent *e);

private:
    ExtendStackWidget *m_pExtendStackWidget;
};

#endif // DEFECTCLASSIFICATIONUI_H
