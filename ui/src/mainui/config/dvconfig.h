#ifndef DVCONFIG_H
#define DVCONFIG_H

#include "rapidjson/document.h"
#include "rapidjson/Prettywriter.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

#include <string>
using namespace std;
#include <QWidget>

namespace Ui {
class dvconfig;
}

class dvconfig : public QWidget
{
    Q_OBJECT

public:
    explicit dvconfig(QWidget *parent = 0);
    ~dvconfig();

    static dvconfig* GetInstance(QWidget* pParant = NULL);
    static void ReleaseInstance();

protected slots:
    void on_pushButton_recover_clicked();
    void on_pushButton_save_clicked();
    void on_pushButton_cancel_clicked();

    void on_pushButton_Edge_Defect_Symbol_clicked();
    void on_pushButton_Edge_Defect_Param_clicked();
    void on_pushButton_Edge_Classify_Symbol_clicked();
    void on_pushButton_Edge_Classify_Param_clicked();
    void on_pushButton_Window_Defect_Symbol_clicked();
    void on_pushButton_Window_Defect_Param_clicked();
    void on_pushButton_Window_Classify_Symbol_clicked();
    void on_pushButton_Window_Classify_Param_clicked();
    void on_pushButton_Window_Classify_Symbol_Sec_clicked();
    void on_pushButton_Window_Classify_Param_Sec_clicked();

    void closeEvent(QCloseEvent *event);
private:
    void initDvConfigUI();
    void saveDvConfig();

    void updateBoolValue(const char *pKey, Value& itemParent, bool bValue);
    void updateIntValue(const char *pKey, Value& itemParent, int iValue);
    void updateDoubleValue(const char *pKey, Value& itemParent, double dValue);
    void updateStringValue(const char *pKey, Value& itemParent, const char * szUIValue, int length);

    QString dvconfig::GetFileName(QString strName, QString strDes = "");
private:
    Ui::dvconfig *ui;

    static dvconfig* m_pInstance;
    bool   m_bIsNeedSave;
};

#endif // DVCONFIG_H
