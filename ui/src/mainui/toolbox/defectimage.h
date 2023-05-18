#ifndef DEFECTIMAGE_H
#define DEFECTIMAGE_H

#include <QWidget>

namespace Ui {
class DefectImage;
}

class DefectImage : public QWidget
{
    Q_OBJECT

public:
    explicit DefectImage(QWidget *parent = 0);
    ~DefectImage();

    void setExpandSize(int width, int height);
    void setWidgetSize(int width, int height);
    void setImageBuffer(unsigned char *szBuf, int width, int height);
    void setImageBuffer(unsigned char *szSourceBuf, int width, int height, int xBegin, int yBegin, int swidth, int sheight);
private:
    Ui::DefectImage *ui;
    int m_expandWidth;
    int m_expandHeight;
};

#endif // DEFECTIMAGE_H
