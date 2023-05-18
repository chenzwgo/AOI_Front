#ifndef TOOlBUTTON_H
#define TOOlBUTTON_H

#include <QObject>
#include <QToolButton>
#include <QMouseEvent>
#include <QPainter>
#include <QList>
/*
 * ToolButton: 可以动态改变未选择，鼠标经过，选择时的Icon.
 * ToolButtonGroup: 每个Group中只能最多一个Button被选中
*/
class ToolButtonGroup;

class ToolButton : public QToolButton
{

public:
    explicit ToolButton(QWidget *parent = 0);
	~ToolButton();
	void setMousePress(bool mouse_press);
    virtual void	setIcon ( const QIcon & icon );

    void setPicname(const QString &picname);
    void setMouseoverPicname(const QString &picname);
    void setPressedPicname(const QString &picname);

    void setButtonIcon();

    ToolButtonGroup* toolButtonGroup() const;
    void setTooButtonGroup(ToolButtonGroup* val);

	bool isMousePressed();
protected:

	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);
	void mousePressEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *event);
	void painterInfo(int top_color, int middle_color, int bottom_color);

    QString m_picName;
    QString m_picName_mouseover;
    QString m_picName_pressed;
private:

	bool mouse_over; //鼠标是否移过
	bool mouse_press; //鼠标是否按下

    ToolButtonGroup* m_tooButtonGroup;
};


class ToolButtonGroup
{

public:
    bool SetToolButtonPressed(ToolButton* pressedTool);
    void addToolButton(ToolButton* button);
    void removeToolButton(ToolButton* button);
public:
    ToolButtonGroup();

    QList<ToolButton*> m_toolbutton_list;
    
};
#endif //TOOlBUTTON_H
