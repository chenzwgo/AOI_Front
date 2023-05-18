#ifndef CLINECAMERALIST_H
#define CLINECAMERALIST_H

#include <QDialog>

namespace Ui {
class CLineCameraList;
}

class ILineCameraFactoryManage;
class QListWidgetItem;
class CLineCameraList : public QDialog
{
    Q_OBJECT

public:
    explicit CLineCameraList(QWidget *parent = 0);
    ~CLineCameraList();
public:
    void init();
public slots:
    void on_pushButton_add_clicked();
    void on_pushButton_delete_clicked();
    void on_pushButton_update_clicked();
    void on_pushButton_save_clicked();
    void on_listWidget_cam_list_itemClicked(QListWidgetItem *item);
private:
    Ui::CLineCameraList *ui;
    ILineCameraFactoryManage *m_pCameraManager;
};

#endif // CLINECAMERALIST_H
