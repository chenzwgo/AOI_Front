#include "clock.h"
#include "ui_clock.h"

Clock::Clock(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Clock),
	m_pTimer(NULL)
{
    ui->setupUi(this);

	m_pTimer = new QTimer(this);
	m_pTimer->start(500);//һ����
	connect(m_pTimer,SIGNAL(timeout()),this,SLOT(update()));

	QFont font;
	font.setPointSize(8);//�����С����Ϊ10
	setFont(font);
}

Clock::~Clock()
{
    delete ui;
}

void Clock::paintEvent( QPaintEvent *event )
{
	QPainter painter(this);
	painter.setRenderHint(QPainter::Antialiasing, true);

	int side = qMin(width(), height());

	painter.setViewport((width() - side) / 2, (height() - side) / 2,
		side, side);
	painter.setWindow(0, 0, side, side);/*����(0��0)Ϊ�µ�����ԭ��*/

	draw(&painter);
}

void Clock::draw( QPainter *painter )
{
	int side = qMin(width(), height());
	int iWidth = side;
	int iHeight = side;
	static const QPoint hourHand[4] = {
		QPoint(2, 4),
		QPoint(0, 10),
		QPoint(-2, 4),
		QPoint(0, -(side/2 - 25))
	};
	static const QPoint minuteHand[4] = {
		QPoint(2, 4),
		QPoint(0, 10),
		QPoint(-2, 4),
		QPoint(0, -(side/2 - 18))
	};
	static const QPoint secondHand[4] = {
		QPoint(2, 8),
		QPoint(0, 10),
		QPoint(-2, 4),
		QPoint(0, -(side/2 - 10))
	};//����


	QPen thickPen(palette().foreground(), 2);//���ÿ̶���Ϊ�ֺ�
	QPen thinPen(palette().foreground(), 1);//���ÿ̶���Ϊ��

	QString strdatetime = QDateTime::currentDateTime().toString("hh:mm:ss");
	QStringList listdatetime = strdatetime.split(":");
	int iHour = listdatetime.at(0).toInt();
	int iMin = listdatetime.at(1).toInt();
	int iSec = listdatetime.at(2).toInt();
	painter->translate(side/2,side/2);//�����趨����ԭ��

	painter->setRenderHint(QPainter::Antialiasing);//�����
	painter->setBrush(Qt::red);
	painter->setPen(Qt::red);
	painter->save();//��������ϵ����ֹ����ϵ��ƫ��
	painter->rotate(6.0*iSec);//ע����6.0������6
	painter->drawConvexPolygon(secondHand,4);
	painter->restore();//��λ֮ǰ������ϵ

	painter->setBrush(Qt::blue);
	painter->setPen(Qt::blue);
	painter->save();
	painter->rotate(6.0*(iMin+iSec/60.0));
	painter->drawConvexPolygon(minuteHand,4);
	painter->restore();

	painter->setBrush(Qt::black);
	painter->setPen(Qt::black);
	painter->save();
	painter->rotate(30.0*(iHour+iMin/60.0));
	painter->drawConvexPolygon(hourHand,4);
	painter->restore();

	QBrush CenterBrush(QColor(255, 215, 0));
	painter->setBrush(CenterBrush);
	painter->setPen(QColor(255, 215, 0));
	painter->setPen(Qt::NoPen);
	painter->save();
	painter->drawEllipse(QPoint(0, 0), 1, 1);
	painter->restore();

	/*���̶Ȼ�������*/
	for (int i = 1; i <=60; ++i) {
		painter->save();
		painter->rotate(6*i);//��������ת6��
		if (i % 5 == 0) {
			painter->setPen(thickPen);
			painter->drawLine(0, -(iWidth/2-6), 0, -(iWidth/2-10));
			painter->drawText(-13, -(iHeight/2-13), 24, 24,
				Qt::AlignHCenter | Qt::AlignTop,
				QString::number(i/5));
		} else {
			painter->setPen(thinPen);
			painter->drawLine(0, -(iWidth/2-6), 0, -(iWidth/2-8));
		}
		//painter->rotate(-DegreesPerMinute);
		painter->restore();
	}

	painter->restore();
	painter->setRenderHint(QPainter::Antialiasing, true);
    /**
     * ���������뾶
     * ����������ʼ�ĽǶ�
     * �����ģ�ָɨȡ�ĽǶ�-˳ʱ�루360�� / 8 = 45�ȣ�
     * �����壺Բ���ĸ߶�
     * �����������ɫ
    **/
	gradientArc(painter, iHeight/2, 0, 360, 3, qRgb(0, 0, 0));
	gradientArc(painter, iHeight/2 - 4, 0, 360, 1, qRgb(0, 0, 0));
	painter->restore();//��λ֮ǰ������ϵ
}

void Clock::gradientArc(QPainter *painter, int radius, int startAngle, int angleLength, int arcHeight, QRgb color)
{
	// ����ɫ
	QRadialGradient gradient(0, 0, radius);
	gradient.setColorAt(0, Qt::white);
	gradient.setColorAt(1.0, color);
	painter->setBrush(gradient);

	// << 1������1λ���൱��radius*2 ����150*2=300
	//QRectF(-150, -150, 300, 300)
	QRectF rect(-radius, -radius, radius << 1, radius << 1);
	QPainterPath path;
	path.arcTo(rect, startAngle, angleLength);

	// QRectF(-120, -120, 240, 240)
	QPainterPath subPath;
	subPath.addEllipse(rect.adjusted(arcHeight, arcHeight, -arcHeight, -arcHeight));

	// pathΪ���� subPathΪ��Բ
	path -= subPath;

	painter->setPen(Qt::NoPen);
	painter->drawPath(path);
}
