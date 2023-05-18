#include "clock.h"
#include "ui_clock.h"

Clock::Clock(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Clock),
	m_pTimer(NULL)
{
    ui->setupUi(this);

	m_pTimer = new QTimer(this);
	m_pTimer->start(500);//一秒钟
	connect(m_pTimer,SIGNAL(timeout()),this,SLOT(update()));

	QFont font;
	font.setPointSize(8);//字体大小设置为10
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
	painter.setWindow(0, 0, side, side);/*设置(0，0)为新的坐标原点*/

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
	};//秒针


	QPen thickPen(palette().foreground(), 2);//设置刻度线为粗黑
	QPen thinPen(palette().foreground(), 1);//设置刻度线为灰

	QString strdatetime = QDateTime::currentDateTime().toString("hh:mm:ss");
	QStringList listdatetime = strdatetime.split(":");
	int iHour = listdatetime.at(0).toInt();
	int iMin = listdatetime.at(1).toInt();
	int iSec = listdatetime.at(2).toInt();
	painter->translate(side/2,side/2);//重新设定坐标原点

	painter->setRenderHint(QPainter::Antialiasing);//消锯齿
	painter->setBrush(Qt::red);
	painter->setPen(Qt::red);
	painter->save();//保存坐标系，防止坐标系跑偏了
	painter->rotate(6.0*iSec);//注意是6.0，不是6
	painter->drawConvexPolygon(secondHand,4);
	painter->restore();//复位之前的坐标系

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

	/*画刻度还有数字*/
	for (int i = 1; i <=60; ++i) {
		painter->save();
		painter->rotate(6*i);//坐标轴旋转6度
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
     * 参数二：半径
     * 参数三：开始的角度
     * 参数四：指扫取的角度-顺时针（360度 / 8 = 45度）
     * 参数五：圆环的高度
     * 参数六：填充色
    **/
	gradientArc(painter, iHeight/2, 0, 360, 3, qRgb(0, 0, 0));
	gradientArc(painter, iHeight/2 - 4, 0, 360, 1, qRgb(0, 0, 0));
	painter->restore();//复位之前的坐标系
}

void Clock::gradientArc(QPainter *painter, int radius, int startAngle, int angleLength, int arcHeight, QRgb color)
{
	// 渐变色
	QRadialGradient gradient(0, 0, radius);
	gradient.setColorAt(0, Qt::white);
	gradient.setColorAt(1.0, color);
	painter->setBrush(gradient);

	// << 1（左移1位）相当于radius*2 即：150*2=300
	//QRectF(-150, -150, 300, 300)
	QRectF rect(-radius, -radius, radius << 1, radius << 1);
	QPainterPath path;
	path.arcTo(rect, startAngle, angleLength);

	// QRectF(-120, -120, 240, 240)
	QPainterPath subPath;
	subPath.addEllipse(rect.adjusted(arcHeight, arcHeight, -arcHeight, -arcHeight));

	// path为扇形 subPath为椭圆
	path -= subPath;

	painter->setPen(Qt::NoPen);
	painter->drawPath(path);
}
