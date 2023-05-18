#include "notitledlg.h"
#include "ui_notitledlg.h"

NoTitleDlg::NoTitleDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NoTitleDlg)
{
    ui->setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint | Qt::Tool);
	setAttribute(Qt::WA_TranslucentBackground);
	m_bPressed = false;
}

NoTitleDlg::~NoTitleDlg()
{
    delete ui;
}

void NoTitleDlg::setAreaMovable(const QRect rt)
{
	if (m_areaMovable != rt)
	{
		m_areaMovable = rt;
	}
}

void NoTitleDlg::mousePressEvent(QMouseEvent *e)
{
	//ЪѓБъзѓМќ
	if (e->button() == Qt::LeftButton)
	{
		m_ptPress = e->pos();
		m_bPressed = m_areaMovable.contains(m_ptPress);
		setAreaMovable(this->rect());
	}
}

void NoTitleDlg::mouseMoveEvent(QMouseEvent *e)
{
	if (m_bPressed)
	{
		this->move(pos() + e->pos() - m_ptPress);
	}
}

void NoTitleDlg::mouseReleaseEvent(QMouseEvent *e)
{
	m_bPressed = false;
}
void NoTitleDlg::showEvent(QShowEvent *)
{
	setAreaMovable(this->rect());
}
