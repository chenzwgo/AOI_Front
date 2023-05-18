#ifndef MAINFRAME_H
#define MainFrame_H

#include <QWidget>
#include <QMap>
#include "inotify.h"

namespace Ui {
class MainFrame;
}

class HomeView;
class MainFrame : public QWidget
{
    Q_OBJECT

public:
    explicit MainFrame(QWidget *parent = 0);
    ~MainFrame();
public:
	bool init();
	bool deinit();
	bool showWindow(QWidget* parent, QRect& rect, bool bPopFlag = false);
	bool hideWindow();
	void fitSize(const QRect& rect);

	bool notify(QString event, OS_LPARAM wparam, OS_LPARAM lparam);
	void SetLanguage(QString strlanguage);
public:
	static MainFrame* GetInstance();
	static void DestroyInstance();
private slots:
	void slotToolBarClicked(int iToolBtnIndex);
	void slotModelChange(int iModel);
private:
	Ui::MainFrame *ui;
	static MainFrame* m_pInstance;
	HomeView* m_pHomeView;
	QMap<QWidget*, QWidget*> m_mapPages;
};

#endif // MAINFRAME_H
