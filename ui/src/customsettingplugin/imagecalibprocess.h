#ifndef IMAGECALIBPROCESS_H
#define IMAGECALIBPROCESS_H

#include <QWidget>

namespace Ui {
class ImageCalibProcess;
}

class ImageCalibProcess : public QWidget
{
    Q_OBJECT

public:
    explicit ImageCalibProcess(QWidget *parent = 0);
    ~ImageCalibProcess();

private:
    void init();

public slots:
    void on_pushButton_base_browse_clicked ( bool checked = false );
    void on_pushButton_source_browse_clicked ( bool checked = false );
    void on_pushButton_exe_clicked ( bool checked = false );
    void on_pushButton_save_clicked ( bool checked = false );
    void on_pushButton_base_clicked ( bool checked = false );
    void on_pushButton_source_clicked ( bool checked = false );
    void on_pushButton_correction_clicked ( bool checked = false );
    void on_pushButton_union_clicked ( bool checked = false );
    void on_spinBox_th_min_valueChanged(int value);
    void on_spinBox_th_max_valueChanged(int value);

private:
    Ui::ImageCalibProcess *ui;


};

#endif // IMAGECALIBPROCESS_H
