#ifndef RecipesUI_H
#define RecipesUI_H

#include <QDialog>
#include <QTranslator>
#include "OS_DEFINE.h"

namespace Ui {
class RecipesUI;
}

class ExtendStackWidget;
class ProductParamUI;
class RecipesParamUI;

class RecipesUI : public QDialog
{
    Q_OBJECT
    
private:
    explicit RecipesUI(QWidget *parent = 0);
    ~RecipesUI();

    void readAboutInfo();
protected:
    void keyPressEvent(QKeyEvent *e);

public:
    static RecipesUI* getInstance();
    static void releaseInstance();

    bool init();
    bool deinit();
    bool showWindow(QWidget* parent, QRect& rect,bool bPopFlag = false);
    bool hideWindow();
    void setPluginContainer(void *pPluginContainer);
    void fitSize(const QRect& rect);
    
    bool notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam);
     void SetLanguage(QString strlanguage);
protected:
	void showEvent(QShowEvent *pEvent);

private:
    static RecipesUI * m_pInstance;    
    ExtendStackWidget *m_pExtendStackWidget;
    ProductParamUI* m_pProductParamUI;
    RecipesParamUI* m_pRecipesParamUI;
    QTranslator m_translator;
};

#endif // RecipesUI_H
