#ifndef CUSTOMUI_H
#define CUSTOMUI_H

#include <QWidget>
#include <QFrame>
#include <QTranslator>
#include "OS_DEFINE.h"
#include "videoscope.h"
#include "imageviewer.h"
class ExtendStackWidget;
class ConfigView;
class DebugView;
class IlluminationControlDlg;

class CustomUi : public QWidget
{
    Q_OBJECT

public:
    explicit CustomUi(QWidget *parent = 0);
    ~CustomUi();

    bool init();
    bool deinit();
    bool showWindow(QWidget* parent, QRect& rect,bool bPopFlag = false);
    bool hideWindow();
    void fitSize(const QRect& rect);
    bool notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam);
    void SetLanguage(QString strlanguage);
	DebugView* getDebugView();
private:
    ConfigView* m_pConfigui;
    DebugView* m_pDebugView;
    //IlluminationControlDlg* m_pIlluminationControlDlg;
    QTranslator m_translator;
    ExtendStackWidget *m_pExtendStackWidget;
    ExtendStackWidget *m_pECamTool;
	videoscope *m_pItem;
	ImageViewer *m_Image;
	int m_iMode;

};

#endif // CUSTOMUI_H
