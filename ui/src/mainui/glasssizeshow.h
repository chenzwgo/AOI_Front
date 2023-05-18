#ifndef GLASSSIZESHOW_H
#define GLASSSIZESHOW_H

#include <QWidget>
#include <map>

using namespace std;

namespace Ui {
class glasssizeshow;
}

class GlassSizeShow : public QWidget
{
    Q_OBJECT

public:
    explicit GlassSizeShow(QWidget *parent = 0);
    ~GlassSizeShow();

	// …Ë÷√≥ﬂ¥Áƒ⁄»›
	void setValues(map<string,double> values);

private:
    Ui::glasssizeshow *ui;
};

#endif // GLASSSIZESHOW_H
