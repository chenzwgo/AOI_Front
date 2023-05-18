#include "tool_button.h"

ToolButton::ToolButton(QWidget *parent)
	:QToolButton(parent),
    m_tooButtonGroup(NULL)
{
	//设置文本颜色
    QPalette text_palette = palette();
    text_palette.setColor(QPalette::ButtonText, QColor(255, 255, 255));
   // setPalette(text_palette);

	//设置文本粗体
	QFont &text_font = const_cast<QFont &>(font());
	text_font.setWeight(QFont::Bold);

	setToolButtonStyle(Qt::ToolButtonIconOnly);

	//设置大小
	setAutoRaise(true);
    setObjectName("transparentToolButton");
    setCheckable(true);
	mouse_over = false;
	mouse_press = false;
    //this->setStyleSheet("QToolButton {background:rgb(234, 234, 235);border-radius:6px;}"
    //    "QToolButton:hover{background:rgb(228, 228, 229);border-radius:6px;}"
    //    "QToolButton:checked{background-color: rgb(176, 216, 155); border-radius:6px;}"
    //    "QToolButton:pressed{background:rgb(228, 228, 229);border-radius:6px;padding-top :1px;}");
	//this->setCheckable(true);
	//this->setChecked(false);
	//this->setStyleSheet(
	//	"QToolButton:hover{background:transparent;}"
	//	);
	//this->setStyleSheet(
	//	"QToolButton {background:rgb(237, 238, 239);;border-radius:6px;}QToolButton:hover{background:rgb(228, 228, 229);border-radius:6px;}QToolButton:checked{background-color: rgb(174, 218, 79);border-radius:6px;}QToolButton:pressed{background:rgb(176, 216, 155);border-radius:6px;padding-top :1px;}");
}

ToolButton::~ToolButton()
{

}

void ToolButton::enterEvent(QEvent *event)
{
	mouse_over = true;
    setButtonIcon();
}

void ToolButton::leaveEvent(QEvent *)
{
	mouse_over = false;
    setButtonIcon();
}

void ToolButton::mousePressEvent(QMouseEvent *event)
{
	if(event->button() == Qt::LeftButton)
	{
        if (m_tooButtonGroup != NULL)
        {
            m_tooButtonGroup->SetToolButtonPressed(this);
        }
		emit clicked();
	}
}

void ToolButton::setMousePress(bool _mouse_press)
{
	this->mouse_press = _mouse_press;
    setButtonIcon();
	this->setChecked(_mouse_press);
	update();
}

void ToolButton::paintEvent(QPaintEvent *event)
{
	if(mouse_over)
	{
		//绘制鼠标移到按钮上的按钮效果
		painterInfo(0, 100, 150);
	}
	else
	{
		if(mouse_press)
		{
			painterInfo(0, 100, 150);
        }
	}

//    QPainter painter(this);
//    QPen pen(Qt::NoBrush, 1);
//    painter.setPen(pen);
//    QLinearGradient linear(rect().topLeft(), rect().bottomLeft());
//    linear.setColorAt(0, QColor(0, 184, 89, 0));
//    linear.setColorAt(0.5, QColor(0, 184, 89, 100));
//    linear.setColorAt(1, QColor(0, 184, 89, 150));
//    painter.setBrush(linear);
//    painter.drawRect(rect());
    QToolButton::paintEvent(event);
}

void ToolButton::painterInfo(int top_color, int middle_color, int bottom_color)
{
	//QPainter painter(this);
	//QPen pen(Qt::NoBrush, 1);
	//painter.setPen(pen);
	//QLinearGradient linear(rect().topLeft(), rect().bottomLeft());
	//linear.setColorAt(0, QColor(0, 184, 89, top_color));
	//linear.setColorAt(0.5, QColor(0, 184, 89, middle_color));
	//linear.setColorAt(1, QColor(0, 184, 89, bottom_color));
	//painter.setBrush(linear);
	//painter.drawRect(rect());
}

void ToolButton::setIcon( const QIcon & icon )
{
    QToolButton::setIcon(icon);
}

void ToolButton::setPicname( const QString &picname )
{
    m_picName = picname;
}

void ToolButton::setMouseoverPicname( const QString &picname )
{
    m_picName_mouseover = picname;
}

void ToolButton::setPressedPicname( const QString &picname )
{
    m_picName_pressed = picname;
}

void ToolButton::setButtonIcon()
{
    return;
    if(mouse_over)
    {
        //绘制鼠标移到按钮上的按钮效果
        if(mouse_press)
        {
			setIcon(QIcon(m_picName_pressed));
        }
        else
        {
			setIcon(QIcon(m_picName_mouseover));
        }
    }
    else
    {
        if(mouse_press)
        {
			setIcon(QIcon(m_picName_pressed));
        }
        else
        {
			setIcon(QIcon(m_picName));
        }
    }
    update();
}

void ToolButton::setTooButtonGroup( ToolButtonGroup* val )
{
    m_tooButtonGroup = val;
}

ToolButtonGroup* ToolButton::toolButtonGroup() const
{
    return m_tooButtonGroup;
}

bool ToolButton::isMousePressed()
{
	return mouse_press;
}

bool ToolButtonGroup::SetToolButtonPressed( ToolButton* pressedTool )
{
    foreach(ToolButton* toolButton,m_toolbutton_list)
    {
        if (toolButton == NULL)
        {
            continue;
        }
        if (toolButton == pressedTool)
        {
            toolButton->setMousePress(true);
        }
        else
        {
            toolButton->setMousePress(false);
        }
    }
    return true;
}

ToolButtonGroup::ToolButtonGroup()
{

}

void ToolButtonGroup::addToolButton( ToolButton* button )
{
    if (button != NULL && (m_toolbutton_list.isEmpty() || !m_toolbutton_list.contains(button)))
    {
        m_toolbutton_list.append(button);
        ToolButtonGroup* group = button->toolButtonGroup();
        if ( group != this)
        {
            button->setTooButtonGroup(this);
        }
    }
}

void ToolButtonGroup::removeToolButton( ToolButton* button )
{
    if (button != NULL && m_toolbutton_list.contains(button))
    {
        m_toolbutton_list.removeAll(button);
        button->setTooButtonGroup(NULL);
    }
}
