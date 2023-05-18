#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QSplitter>
#include <QTabWidget>
#include <QGroupBox>

class MainWidget:public QWidget
{
public:
    MainWidget(QWidget* parent = 0);
    ~MainWidget();

    void addWidgetToTopLeftArea(QWidget* pWidget);
    void addWidgetToTopRightArea(QWidget* pWidget);    
    void addWidgetToLeftArea(QWidget* pWidget);
    void addWidgetToRightTopArea(QWidget* pWidget,QString widgetname);
    void addWidgetToRightBottomArea(QWidget* pWidget,QString widgetname);

private:
    void createMainArea();
    void createTopArea();
    void createLeftArea();
	void creteRightArea();
    void createRighttopArea();
    void createRightBottomArea();

    QVBoxLayout* m_pMainLayout;

    QSplitter* m_pMainSplitter;
    QSplitter* m_pRightSpltter;

    QWidget* m_pTopWidget;
    QHBoxLayout* m_pTopLayout;
    QHBoxLayout* m_pTopLeftLayout;
    QHBoxLayout* m_pTopRightLayout;

    QWidget     *m_pleftWidget;
    QVBoxLayout *m_pleftLayout;
	QWidget     *m_pRightWidget;
	QVBoxLayout *m_pRightLayout;
    QTabWidget* m_pRightTopWidget;
    QTabWidget* m_pRightBottomWidget;
};

#endif // MAINWIDGET_H
