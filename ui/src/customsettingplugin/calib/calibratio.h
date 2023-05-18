#ifndef CALIBRATIO_H
#define CALIBRATIO_H
#include <QWidget>

class stImageInfo;
namespace Ui {
class CalibRatio;
}
class CalibRatio : public QWidget
{
    Q_OBJECT

public:
    explicit CalibRatio(QWidget *parent = 0);
    ~CalibRatio();

private:
    void init();
    void showImage();
public slots:
    void on_pushButton_browse_clicked(bool checked = false);
    void on_pushButton_calc_clicked(bool checked = false);
    void on_pushButton_save_clicked(bool checked = false);
    void on_spinBox_th_min_valueChanged(int value);
    void on_spinBox_th_max_valueChanged(int value);
private:
    Ui::CalibRatio *ui;
    stImageInfo *m_pImageData;
};

#endif // CALIBRATIO_H
