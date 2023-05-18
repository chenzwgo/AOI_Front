#ifndef KEYFEATURESWIDGET_H
#define KEYFEATURESWIDGET_H

#include <QWidget>

namespace Ui {
class KeyFeaturesWidget;
}

class KeyFeaturesWidget : public QWidget
{
    Q_OBJECT

public:
    explicit KeyFeaturesWidget(QWidget *parent = 0);
    ~KeyFeaturesWidget();

private:
    Ui::KeyFeaturesWidget *ui;
};

#endif // KEYFEATURESWIDGET_H
